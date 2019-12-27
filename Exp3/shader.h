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


//#include <OpenGL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�

class Shader
{
public:
    // ����ID
    GLuint Program;
    // ��������ȡ��������ɫ��
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // ʹ�ó���
    void Use();
};

#endif


#endif /* Shader_h */
