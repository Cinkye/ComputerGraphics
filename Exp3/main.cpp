#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cmath>
#include "shader.h"
#include <GL/glfw3.h>
#include <GL/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

void keyboard(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

using namespace std;

float vertices[] = {
    // position, texture coordinate, normal
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
};
float vertices2[] = {
    // position, texture coordinate, normal
    1.5f, 1.5f, 1.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    2.5f, 1.5f, 1.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    2.5f, 2.5f, 1.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    2.5f, 2.5f, 1.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    1.5f, 2.5f, 1.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    1.5f, 1.5f, 1.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    1.5f, 1.5f, 2.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    2.5f, 1.5f, 2.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    2.5f, 2.5f, 2.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    2.5f, 2.5f, 2.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    1.5f, 2.5f, 2.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    1.5f, 1.5f, 2.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    1.5f, 2.5f, 2.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    1.5f, 2.5f, 1.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    1.5f, 1.5f, 1.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    1.5f, 1.5f, 1.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    1.5f, 1.5f, 2.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    1.5f, 2.5f, 2.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

    2.5f, 2.5f, 2.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    2.5f, 2.5f, 1.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    2.5f, 1.5f, 1.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    2.5f, 1.5f, 1.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    2.5f, 1.5f, 2.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    2.5f, 2.5f, 2.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    1.5f, 1.5f, 1.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    2.5f, 1.5f, 1.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    2.5f, 1.5f, 2.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    2.5f, 1.5f, 2.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    1.5f, 1.5f, 2.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    1.5f, 1.5f, 1.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    1.5f, 2.5f, 1.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    2.5f, 2.5f, 1.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    2.5f, 2.5f, 2.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    2.5f, 2.5f, 2.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    1.5f, 2.5f, 2.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    1.5f, 2.5f, 1.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,

};
GLuint indices[]= {
    0,1,3,
    1,2,3
};

glm::vec3 cubePosition = glm::vec3( 2.0f,  5.0f, -15.0f);

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
GLfloat rotateY = 0.0f;
GLfloat rotateX = 0.0f;
GLfloat opacity = 0.5f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
glm::vec3 objectColor = glm::vec3(1.0f,0.5f,0.31f);
glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);

int main()
{
    // Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    //创建并调用shader
    Shader shaders("./vShader.glsl","./fShader.glsl");
    Shader light("./vShader.glsl","fShader2.glsl");

    //创建VAO
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    //创建VBO并绑定
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    //unsigned int EBO;
    //glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    //把vertices写到VBO里,indices -> EBO
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //创建Vertex attribute pointer
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5*sizeof(float)));
    glEnableVertexAttribArray(0); // Enable layout 0
    glEnableVertexAttribArray(1); // Enable layout 1
    glEnableVertexAttribArray(2); // Enable layout 2

    glBindBuffer(GL_ARRAY_BUFFER,0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); // Unbind EBO

    //创建VAO
    unsigned int VAO2;
    glGenVertexArrays(1,&VAO2);
    //创建VBO并绑定
    unsigned int VBO2;
    glGenBuffers(1,&VBO2);
    //unsigned int EBO;
    //glGenBuffers(1,&EBO);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    //把vertices写到VBO里,indices -> EBO
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //创建Vertex attribute pointer
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5*sizeof(float)));
    glEnableVertexAttribArray(0); // Enable layout 0
    glEnableVertexAttribArray(1); // Enable layout 1
    glEnableVertexAttribArray(2); // Enable layout 2

    glBindBuffer(GL_ARRAY_BUFFER,0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO

    unsigned int lightVAO;
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //Bind texture
    unsigned int texture1;
    glGenTextures(1,&texture1);

    // Texture 1
    glBindTexture(GL_TEXTURE_2D,texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    int width,height,nrChannels;
    unsigned char *data = stbi_load("container.jpg",&width,&height,&nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Texture 2
    unsigned int texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    data = stbi_load("awesomeface.png",&width,&height,&nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    //render
    while(!glfwWindowShouldClose(window))
    {
        // input
        // -----
        keyboard(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // process render time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        shaders.Use();
        glm::vec3 lightPos(1.2f,1.0f,2.0f);
        lightColor = glm::vec3(1.0f,1.0f,1.0f);
        glUniform3fv(glGetUniformLocation(light.Program,"lightPos"),1,glm::value_ptr(lightPos));
        glUniform1f(glGetUniformLocation(shaders.Program,"opacity"),opacity);
        glUniform1i(glGetUniformLocation(shaders.Program,"texture1"),0);
        glUniform3fv(glGetUniformLocation(shaders.Program,"lightColor"),1,glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaders.Program,"objectColor"),1,glm::value_ptr(objectColor));
        glUniform3fv(glGetUniformLocation(shaders.Program,"viewPos"),1,glm::value_ptr(-cameraPos));
        glBindVertexArray(VAO);
        //model matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view;
        //view matrix
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view = glm::rotate(view,glm::radians(rotateY),glm::vec3(0.0f,1.0f,0.0f));
        view = glm::rotate(view,glm::radians(rotateX),glm::vec3(1.0f,0.0f,0.0f));
        //projection matrix
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
        //send matrix uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaders.Program,"model"),1,GL_FALSE,glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaders.Program,"view");
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shaders.Program,"projection");
        glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
        int opacityLoc = glGetUniformLocation(shaders.Program,"opacity");
        glUniform1f(projectionLoc,opacity);
        //send material info
        int ambientLoc = glGetUniformLocation(shaders.Program,"material.ambient");
        glUniform3fv(ambientLoc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
        int diffuseLoc = glGetUniformLocation(shaders.Program,"material.diffuse");
        glUniform3fv(diffuseLoc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
        int specularLoc = glGetUniformLocation(shaders.Program,"material.specular");
        glUniform3fv(specularLoc,1,glm::value_ptr(glm::vec3(0.5f,0.5f,0.5f)));
        int shininessLoc = glGetUniformLocation(shaders.Program,"material.shininess");
        glUniform1f(shaders.Program,32.0f);
        glDrawArrays(GL_TRIANGLES,0,36);
        model = glm::translate(model,cubePosition);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES,0,36);

        // Draw the light
        light.Use();
        glUniform3fv(glGetUniformLocation(light.Program,"lightColor"),1,glm::value_ptr(lightColor));
        model = glm::mat4();
        model = glm::translate(model,lightPos);
        model = glm::scale(model,glm::vec3(0.02f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view = glm::rotate(view,glm::radians(rotateY),glm::vec3(0.0f,1.0f,0.0f));
        view = glm::rotate(view,glm::radians(rotateX),glm::vec3(1.0f,0.0f,0.0f));
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
        light.Use();
        glUniformMatrix4fv(glGetUniformLocation(light.Program,"model"),1,GL_FALSE,glm::value_ptr(model));
        viewLoc = glGetUniformLocation(light.Program,"view");
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        projectionLoc = glGetUniformLocation(light.Program,"projection");
        glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyboard(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        opacity += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(opacity >= 1.0f)
            opacity = 1.0f;
        std::cout << opacity << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        opacity -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (opacity <= 0.0f)
            opacity = 0.0f;
        std::cout << opacity << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotateY = (rotateY - 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotateY = (rotateY + 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotateX = (rotateX + 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotateX = (rotateX - 0.1f);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}
