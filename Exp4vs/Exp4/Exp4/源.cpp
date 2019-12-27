// 尚未完成:地板shader的阴影相关uniform传递、shader的阴影部分
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// light
struct Light {
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;

    float constant;
    float linear;
    float quadratic;
};
void sendLightInfo(Shader shader, Light light)
{
    shader.setVec3("light.position", light.position);
    shader.setVec3("light.ambient", light.ambient);
    shader.setVec3("light.diffuse", light.diffuse);
    shader.setVec3("light.specular", light.specular);
    shader.setVec3("light.color", light.color);
    shader.setFloat("light.constant", light.constant);
    shader.setFloat("light.linear", light.linear);
    shader.setFloat("light.quadratic", light.quadratic);
};

// model display control
int display = 3;

// shadows
int shadows = 1;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// floor
float floorData[] = { -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,         // floor part
                      -5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                       5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                       5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                      -5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                       5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                      -5.0f, 6.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,         // far part
                      -5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                       5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                      -5.0f, 6.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                       5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                       5.0f, 6.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                      -5.0f, 6.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,         // ceiling part
                      -5.0f, 6.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                       5.0f, 6.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                       5.0f, 6.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                      -5.0f, 6.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                       5.0f, 6.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                      -5.0f, 6.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,         // left
                      -5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                      -5.0f, 6.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                      -5.0f, 6.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
                      -5.0f, 0.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
                      -5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                       5.0f, 6.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,         // right
                       5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                       5.0f, 6.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                       5.0f, 6.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                       5.0f, 0.0f,  5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                       5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, };

// light
Light light = {
    glm::vec3(0.0f,3.0f,3.0f),
    glm::vec3(0.5f,0.5f,0.5f),
    glm::vec3(0.5f,0.5f,0.5f),
    glm::vec3(0.7f,0.7f,0.7f),
    glm::vec3(1.0f,1.0f,1.0f),
    1.0f,0.0f,0.0f
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // buffer floor data
    // -----------------------------
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorData), floorData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // floor texture
    unsigned int floorTexture;
    glGenTextures(1, &floorTexture);

    // Texture 1
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../../resources/textures/floor.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // wall texture
    unsigned int wallTexture;
    glGenTextures(1, &wallTexture);

    // Texture 2
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("../../resources/textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // sky texture
    unsigned int skyTexture;
    glGenTextures(1, &skyTexture);

    // Texture 2
    glBindTexture(GL_TEXTURE_2D, skyTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("../../resources/textures/far.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // shadows
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
    Shader floorShader("floor.vs", "floor.fs");
    Shader simpleDepthShader("depth.vs","depth.fs");

    // load models
    // -----------
    Model skull("../../resources/objects/Skull/12140_Skull_v3_L2.obj");
    Model ourModel("../../resources/objects/nanosuit/nanosuit.obj");


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render shadow first
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
        lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // - now render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 shadowModel = glm::mat4(1.0f);
        shadowModel = glm::translate(shadowModel, glm::vec3(0.0f, -1.75f, 0.0f));
        simpleDepthShader.setMat4("model", shadowModel);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 30);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shadowModel = glm::mat4(1.0f);
        shadowModel = glm::translate(shadowModel, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        shadowModel = glm::scale(shadowModel, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        simpleDepthShader.setMat4("model", shadowModel);
        ourModel.Draw(simpleDepthShader);
        shadowModel = glm::mat4(1.0f);
        shadowModel = glm::translate(shadowModel, glm::vec3(3.0f, -0.75f, 0.0f)); // translate it down so it's at the center of the scene
        shadowModel = glm::scale(shadowModel, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
        shadowModel = glm::rotate(shadowModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        simpleDepthShader.setMat4("model", shadowModel);
        skull.Draw(simpleDepthShader);

        // render
        // ------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        ourShader.setInt("shadows", shadows);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);


        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        sendLightInfo(ourShader, light);
        if(display / 2 == 1)
            ourModel.Draw(ourShader);

        glm::mat4 skullModel = glm::mat4(1.0f);
        skullModel = glm::translate(skullModel, glm::vec3(3.0f, -0.75f, 0.0f)); // translate it down so it's at the center of the scene
        skullModel = glm::scale(skullModel, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
        skullModel = glm::rotate(skullModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", skullModel);
        if(display % 2 == 1)
            skull.Draw(ourShader);

        // render the floor
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        floorShader.use();
        glBindVertexArray(VAO);
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);
        glm::mat4 floorModel = glm::mat4(1.0f);
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -1.75f, 0.0f));
        floorShader.setMat4("model", floorModel);

        floorShader.setVec3("viewPos", camera.Position);
        sendLightInfo(floorShader, light);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glDrawArrays(GL_TRIANGLES, 12, 30);
        glBindTexture(GL_TEXTURE_2D, skyTexture);
        glDrawArrays(GL_TRIANGLES, 6, 12);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        display = display ^ 1;    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        display = display ^ 2;
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS && light.color.r > 0.0f)
        light.color.r -= 0.0001;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS && light.color.g > 0.0f)
        light.color.g -= 0.0001;
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS && light.color.b > 0.0f)
        light.color.b -= 0.0001;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS && light.color.r < 1.0f)
        light.color.r += 0.0001;
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS && light.color.g < 1.0f)
        light.color.g += 0.0001;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS && light.color.b < 1.0f)
        light.color.b += 0.0001;
  
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}