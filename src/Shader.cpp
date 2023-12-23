#include "Shader.h"


Shader::Shader(const char *vertexPath, const char *fragmentPath)
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
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errLog << endl;
    }

    // Comiler and Check Fragment Shader
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, errLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errLog << endl;
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

void Shader::use()
{
    glUseProgram(ID);
}

// Add bool uniform
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

// Add int uniform
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);

}

// Add float uniform
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);   
}
