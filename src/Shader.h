#ifndef SHADER_H
#define SHADER_H

#include "../lib/glad.h"
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public: 
    GLuint ID;

    Shader(const char *vertexPath, const char *fragmentPath)
    {
        using namespace std;
        string vertexCode;
        string fragmentCode;
        ifstream vertexShaderFile;
        ifstream fragmentShaderFile;

        vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

        try {
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);

            stringstream vertexShaderStream;
            stringstream fragmentShaderStream;

            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();


        }
        catch(ifstream::failure e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }

        const char *vertexShaderCode = vertexCode.c_str();
        const char *fragmentShaderCode = fragmentCode.c_str();

        GLuint vertex, fragment;
        int success;
        char errLog[512];

        // Create Shader Objects
        vertex = glCreateShader(GL_VERTEX_SHADER);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);


        // Comiler and Check Vertex Shader
        glShaderSource(vertex, 1, &vertexShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertex, 512, NULL, errLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: \n" << vertexPath << '\n' << errLog << endl;
        }

        // Comiler and Check Fragment Shader
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragment, 512, NULL, errLog);
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: \n" << fragmentPath << '\n' << errLog << endl;
        }

        // Link Shaders
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(ID, 512, NULL, errLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);   
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

};

#endif