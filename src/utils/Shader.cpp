#include "Shader.h"

#include <array>
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &shader_source_code, Type type) : identifier_ {glCreateShader(to_underlying(type))}
{
    const char *source_code_ptr = shader_source_code.c_str();
    glShaderSource(identifier_, 1, &source_code_ptr, nullptr);
    glCompileShader(identifier_);
    check_shader_compilation(identifier_, shader_typename(type));
}

void check_shader_compilation(std::uint32_t shader_id, std::string_view shader_type)
{
    int compilation_successful = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_successful);

    if(!compilation_successful)
    {
        std::array<char, 1024> error_log {};
        glGetShaderInfoLog(shader_id, static_cast<GLsizei>(error_log.size()), nullptr, error_log.data());
        std::stringstream stream;
        stream << shader_type << " Shader compilation error:\n" << error_log.data() << "\n";
        throw std::runtime_error(stream.str());
    }
}

Shader::Shader(Shader &&shader) noexcept : identifier_ {shader.identifier_}
{
    shader.identifier_ = 0;
}

Shader &Shader::operator=(Shader &&shader) noexcept
{
    std::swap(identifier_, shader.identifier_);
    return *this;
}

Shader::~Shader()
{
    glDeleteShader(identifier_);
}

std::uint32_t Shader::identifier() const
{
    return identifier_;
}

const std::string &Shader::shader_typename(Shader::Type type)
{
    static const std::unordered_map<Shader::Type, std::string> shader_types = {
        {Shader::Type::Vertex, "Vertex"},
        {Shader::Type::TessControl, "Tessellation Control"},
        {Shader::Type::TessEval, "Tessellation Evaluation"},
        {Shader::Type::Fragment, "Fragment"},
        {Shader::Type::Compute, "Compute"},
    };
    return shader_types.at(type);
}

ShaderProgram::ShaderProgram(std::initializer_list<std::pair<std::string_view, Shader::Type>> initializer) :
    programId {glCreateProgram()}
{
    std::vector<Shader> shaders;
    shaders.reserve(initializer.size());
    for(const auto &[filepath, shader_type] : initializer)
    {
        shaders.emplace_back(load_shader_from_file(filepath, shader_type));
        glAttachShader(programId, shaders.back().identifier());
    }

    glLinkProgram(programId);
    check_shader_program_link_status(programId, initializer);

    for(const auto &shader : shaders)
    {
        glDetachShader(programId, shader.identifier());
    }

    retrieve_uniforms();
}

Shader load_shader_from_file(std::string_view filepath, Shader::Type type)
{
    std::ifstream shader_file {filepath.data()};
    if(!shader_file.is_open())
    {
        std::stringstream error_log_stream;
        error_log_stream << "File " << filepath << " could not be opened";
        throw std::runtime_error(error_log_stream.str());
    }

    std::stringstream source_code_stream;
    source_code_stream << shader_file.rdbuf();
    return Shader {process_shader_include(source_code_stream.str(), std::filesystem::path{filepath}), type};
}

std::string process_shader_include(std::string shader_source, std::filesystem::path shader_path)
{
    std::string_view pattern = "#include ";
    auto include_position = shader_source.find(pattern);
    if(include_position != std::string::npos)
    {
        auto file_name_start_pos = include_position + pattern.size();
        auto end = shader_source.find('\n', file_name_start_pos);
        std::string filename = shader_source.substr(file_name_start_pos, end - file_name_start_pos - 1);
        std::filesystem::path include_path {shader_path.parent_path()};
        include_path.append(filename);
        std::ifstream shader_file {include_path};
        std::stringstream source_code_stream;
        source_code_stream << shader_file.rdbuf();
        auto include_pattern = shader_source.substr(include_position, end - include_position);
        return std::regex_replace(shader_source, std::regex(include_pattern), source_code_stream.str());
    }

    return shader_source;
}

void check_shader_program_link_status(std::uint32_t shader_program_id,
    std::initializer_list<std::pair<std::string_view, Shader::Type>> shader_data)
{
    int linking_success {0};
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &linking_success);
    if(!linking_success)
    {
        std::array<char, 1024> error_log {};
        glGetProgramInfoLog(shader_program_id, static_cast<GLsizei>(error_log.size()), nullptr, error_log.data());
        std::stringstream stream;
        stream << "Shader program linking error:\n" << error_log.data() << "\nShader Program Files: ";
        for(const auto &[filepath, shader_type] : shader_data)
        {
            stream << filepath << " ";
        }
        stream << "\n";
        throw std::runtime_error(stream.str());
    }
}

void ShaderProgram::retrieve_uniforms()
{
    int number_of_uniforms {0};
    glGetProgramInterfaceiv(programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &number_of_uniforms);
    std::array<GLenum, 2> properties {GL_NAME_LENGTH, GL_LOCATION};
    std::array<GLint, 2> results {};
    std::vector<char> uniform_name(256);
    for(int uniform = 0; uniform < number_of_uniforms; ++uniform)
    {
        glGetProgramResourceiv(programId, GL_UNIFORM, uniform, static_cast<GLsizei>(properties.size()),
            properties.data(), static_cast<GLsizei>(results.size()), nullptr, results.data());

        // Get resources (uniform name and uniform location)
        uniform_name.resize(results[0]);
        glGetProgramResourceName(programId, GL_UNIFORM, uniform, static_cast<GLsizei>(uniform_name.size()), nullptr,
            uniform_name.data());
        std::uint32_t uniform_location = results.back();

        // The name returned contains a null-terminator, so it's necessary to read uniform_name.size() - 1 characters
        uniform_locations_.emplace(std::string {uniform_name.data(), uniform_name.size() - 1}, uniform_location);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept : programId {other.programId}
{
    other.programId = 0;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept
{
    std::swap(programId, other.programId);
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programId);
}

void ShaderProgram::use()
{
    glUseProgram(programId);
}

void ShaderProgram::set_bool_uniform(const std::string &uniform_name, bool value)
{
    glProgramUniform1i(programId, uniform_locations_[uniform_name], static_cast<int>(value));
}

void ShaderProgram::set_int_uniform(const std::string &uniform_name, int value)
{
    glProgramUniform1i(programId, uniform_locations_[uniform_name], value);
}

void ShaderProgram::set_int_array_uniform(const std::string &uniform_name, const int *value, GLsizei count)
{
    glProgramUniform1iv(programId, uniform_locations_[uniform_name], count, value);
}

void ShaderProgram::set_float_uniform(const std::string &uniform_name, float value)
{
    glProgramUniform1f(programId, uniform_locations_[uniform_name], value);
}

void ShaderProgram::set_float_array_uniform(const std::string &uniform_name, const float *value, GLsizei count)
{
    glProgramUniform1fv(programId, uniform_locations_[uniform_name], count, value);
}

void ShaderProgram::set_vec2_uniform(const std::string &uniform_name, float x, float y)
{
    glProgramUniform2f(programId, uniform_locations_[uniform_name], x, y);
}

void ShaderProgram::set_vec2_uniform(const std::string &uniform_name, const glm::vec2 &vector)
{
    glProgramUniform2fv(programId, uniform_locations_[uniform_name], 1, glm::value_ptr(vector));
}

void ShaderProgram::set_vec2_array_uniform(const std::string &uniform_name, const std::vector<glm::vec2> &value,
    GLsizei count)
{
    glProgramUniform2fv(programId, uniform_locations_[uniform_name], count, glm::value_ptr(value.front()));
}

void ShaderProgram::set_vec3_uniform(const std::string &uniform_name, float x, float y, float z)
{
    glProgramUniform3f(programId, uniform_locations_[uniform_name], x, y, z);
}

void ShaderProgram::set_vec3_uniform(const std::string &uniform_name, const glm::vec3 &vector)
{
    glProgramUniform3fv(programId, uniform_locations_[uniform_name], 1, glm::value_ptr(vector));
}

void ShaderProgram::set_vec4_uniform(const std::string &uniform_name, const glm::vec4 &vector)
{
    glProgramUniform4fv(programId, uniform_locations_[uniform_name], 1, glm::value_ptr(vector));
}

void ShaderProgram::set_mat4_uniform(const std::string &uniform_name, const glm::mat4 &matrix)
{
    glProgramUniformMatrix4fv(programId, uniform_locations_[uniform_name], 1, GL_FALSE, glm::value_ptr(matrix));
}