#ifndef Shader_h
#define Shader_h

#include <stdio.h>

#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//#include <OpenGL/glew.h> // 包含glew来获取所有的必须OpenGL头文件

class Shader
{
public:
    // 程序ID
    GLuint Program;
    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // 使用程序
    void Use();
};

#endif


#endif /* Shader_h */
