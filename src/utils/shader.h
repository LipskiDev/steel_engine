#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

class Shader
{
public:
    enum class Type : GLenum
    {
        Vertex = GL_VERTEX_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval = GL_TESS_EVALUATION_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Compute = GL_COMPUTE_SHADER,
    };

    Shader(const std::string &shader_source_code, Type type);
    Shader(const Shader &) = delete;
    Shader(Shader &&shader) noexcept;
    Shader &operator=(const Shader &) = delete;
    Shader &operator=(Shader &&shader) noexcept;
    ~Shader();

    std::uint32_t ShaderId() const;

private:
    std::uint32_t shaderId_ {0};

    static const std::string &ShaderTypename(Type type);
};

class ShaderProgram
{
public:
    ShaderProgram() = default;
    explicit ShaderProgram(std::initializer_list<std::pair<std::string_view, Shader::Type>> initializer);
    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram(ShaderProgram &&other) noexcept;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram &operator=(ShaderProgram &&other) noexcept;
    ~ShaderProgram();

    void Use();
    void SetBoolUniform(const std::string &uniform_name, bool value);
    void SetIntUniform(const std::string &uniform_name, int value);
    void SetIntArrayUniform(const std::string &uniform_name, const int *value, GLsizei count);
    void SetFloatUniform(const std::string &uniform_name, float value);
    void SetFloatArrayUniform(const std::string &uniform_name, const float *value, GLsizei count);
    void SetVec2Uniform(const std::string &uniform_name, float x, float y);
    void SetVec2Uniform(const std::string &uniform_name, const glm::vec2 &vector);
    void SetVec2ArrayUniform(const std::string &uniform_name, const std::vector<glm::vec2> &value, GLsizei count);
    void SetVec3Uniform(const std::string &uniform_name, float x, float y, float z);
    void SetVec3Uniform(const std::string &uniform_name, const glm::vec3 &vector);
    void SetVec4Uniform(const std::string &uniform_name, const glm::vec4 &vector);
    void SetMat4Uniform(const std::string &uniform_name, const glm::mat4 &transform);
    std::uint32_t programId_ {0};

private:
    std::unordered_map<std::string, std::uint32_t> uniform_locations_ {};

    void RetrieveUniforms();
};

// Auxiliary free functions
void CheckShaderCompilation(std::uint32_t shader_id, std::string_view shader_type);
Shader LoadShaderFromFile(std::string_view filepath, Shader::Type type);
void CheckShaderProgramLinkStatus(std::uint32_t shader_program_id,
    std::initializer_list<std::pair<std::string_view, Shader::Type>> shader_data);
std::string ProcessShaderInclude(std::string shader_source, std::filesystem::path shader_path);

template <typename T>
constexpr std::underlying_type_t<T> ToUnderlying(T enumerator) noexcept
{
    return static_cast<std::underlying_type_t<T>>(enumerator);
}

#endif // SHADER_HPP