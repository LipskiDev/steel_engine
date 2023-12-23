#ifndef SHADER_H
#define SHADER_H

#include "../lib/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public: 
    GLuint ID;

    // constructor that reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);


    // activates the shader
    void use();

    // set uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};

#endif