#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void print_mat4x4(const mat4x4 M);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void load_text(char *filename, char *buf, int max_size);

vec3 camPos = {0.0f, 1.0f , 1.0f};
vec3 camTarget = {0.0f, 0.0f , -1.0f};
vec3 camFront = {0.0f, 0.0f, -1.0f};
vec3 tempUp = {0.0f, 1.0f, 0.0f};
vec3 camDir = {0.0f, 0.0f, -1.0f};
vec3 camRight;

vec3 direction;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

int main(void) {
    GLint mvp_location;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // loading opengl functions to GLAD so we can use them
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    // Vsync option
    glfwSwapInterval(1);

    glViewport(0,0,800,600);
    // callback to change render size on window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
    // front side (red, z-axis static)
        // top triangle                                // normals
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        // bottom triangle
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
    // back side (red, z-axis static)
        // top triangle
        0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top right
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom right
        -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top left
        // bottom triangle
        -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top left
        -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom right
    // left side (green, x-axis static)
        // top triangle
        0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top right
        0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom right
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top left
        // bottom triangle
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top left
        0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left
        0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom right
    // right side (green, x-axis static)
        // top triangle
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        // bottom triangle
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    // top side (blue, x-axis static)
        // top triangle
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top left
        // bottom triangle
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    // bottom side (blue, y-axis static)
        // top triangle
        0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top right
        0.5f, -0.5f, -0.5f, .5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.5, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top left
        // bottom triangle
        -0.5f, -0.5f, 0.5f, .5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top left
        -0.5f, -0.5f, -0.5f, .5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom left
        0.5f, -0.5f, -0.5f, .5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom right
    };

    vec3 cubePositions[] = {
        { -3.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f, 2.0f }
    };


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // create a textured VAO

    unsigned int VAO_textured;
    glGenVertexArrays(1, &VAO_textured);
    glBindVertexArray(VAO_textured);

    // telling OpenGL how to interpret the data from vertex buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // create a VBO without textures
    unsigned int VAO_untextured;
    glGenVertexArrays(1, &VAO_untextured);
    glBindVertexArray(VAO_untextured);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(9*sizeof(float)));
    glEnableVertexAttribArray(2);

    // create a VBO without textures
    unsigned int VAO_lightSource;
    glGenVertexArrays(1, &VAO_lightSource);
    glBindVertexArray(VAO_lightSource);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // Creating the shaders for textured cube
    unsigned int vertexShader_textured;
    vertexShader_textured = glCreateShader(GL_VERTEX_SHADER);

    // load vertex shader file
    char *buf = malloc(8192);
    load_text("texture.vert", buf, 8192);
    //printf("%s\n", buf);

    glShaderSource(vertexShader_textured, 1, (const GLchar**)&buf, NULL);
    glCompileShader(vertexShader_textured);

    // checking if it compiled succesfully
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader_textured, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader_textured, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        return -1;
    }

    // create shader object and compile fragment shader code into it
    unsigned int fragmentShader_textured;
    fragmentShader_textured = glCreateShader(GL_FRAGMENT_SHADER);

    // load fragment shader file
    load_text("texture.frag", buf, 8192);
    //printf("%s\n", buf);

    glShaderSource(fragmentShader_textured, 1, (const GLchar**)&buf, NULL);
    glCompileShader(fragmentShader_textured);

    // check for errors
    glGetShaderiv(fragmentShader_textured, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader_textured, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        return -1;
    }

    // attaching and linking the shaders to a shader program
    unsigned int shaderProgram_textured;
    shaderProgram_textured = glCreateProgram();
    glAttachShader(shaderProgram_textured, vertexShader_textured);
    glAttachShader(shaderProgram_textured, fragmentShader_textured);
    glLinkProgram(shaderProgram_textured);
    glGetProgramiv(shaderProgram_textured, GL_LINK_STATUS, &success);
        if(!success) {
        glGetProgramInfoLog(shaderProgram_textured, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::FAILED\n");
        return -1;
    }

    // Creating light source shader
    unsigned int vertexShader_lightSource;
    vertexShader_lightSource = glCreateShader(GL_VERTEX_SHADER);

    // load vertex shader file
    load_text("light_source.vert", buf, 8192);
    //printf("%s\n", buf);

    glShaderSource(vertexShader_lightSource, 1, (const GLchar**)&buf, NULL);
    glCompileShader(vertexShader_lightSource);

    // checking if it compiled succesfully
    glGetShaderiv(vertexShader_lightSource, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader_lightSource, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        return -1;
    }

    // create shader object and compile fragment shader code into it
    unsigned int fragmentShader_lightSource;
    fragmentShader_lightSource = glCreateShader(GL_FRAGMENT_SHADER);

    // load fragment shader file
    load_text("light_source.frag", buf, 8192);
    //printf("%s\n", buf);

    glShaderSource(fragmentShader_lightSource, 1, (const GLchar**)&buf, NULL);
    glCompileShader(fragmentShader_lightSource);

    // check for errors
    glGetShaderiv(fragmentShader_textured, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader_lightSource, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        return -1;
    }

    // attaching and linking the shaders to a shader program
    unsigned int shaderProgram_lightSource;
    shaderProgram_lightSource = glCreateProgram();
    glAttachShader(shaderProgram_lightSource, vertexShader_lightSource);
    glAttachShader(shaderProgram_lightSource, fragmentShader_lightSource);
    glLinkProgram(shaderProgram_lightSource);
    glGetProgramiv(shaderProgram_lightSource, GL_LINK_STATUS, &success);
        if(!success) {
        glGetProgramInfoLog(shaderProgram_lightSource, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::FAILED\n");
        return -1;
    }

    // delete shader objects
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader); 
    glEnable(GL_DEPTH_TEST);


    // texture loading
    unsigned int texture, texture2;
    // first texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // loading image
    int width, height, nrChannels;
    // flip vertically
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/wall.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture 1\n");
        return -1;
    }
    stbi_image_free(data);

    // second texture
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // loading image
    // flip vertically
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("./assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture 2\n");
        return -1;
    }
    stbi_image_free(data);

    // bind shader MVP variable to mvp_location variable in OpenGL
    mvp_location = glGetUniformLocation(shaderProgram_textured, "MVP");
    GLint mvp_location_lightSource = glGetUniformLocation(shaderProgram_lightSource, "MVP");

    // camera view variable declaration
    mat4x4 LookAt;
    vec3_sub(camDir, camPos, camTarget);
    vec3_norm(camDir, camDir);
    vec3_mul_cross(camRight, tempUp, camDir);
    vec3_norm(camRight, camRight);

    vec3 camUp;
    vec3_mul_cross(camUp, camDir, camRight);
    vec3_norm(camUp,camUp);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        float ratio;
        int width, height;
        mat4x4 p,vp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // create an identity matrix and save into variable m
        //mat4x4_rotate_X(m, m, (float)glfwGetTime());
        //mat4x4 temp;
        //mat4x4_translate(temp, sin((float)glfwGetTime()), 0.0f, 0.0f);
        //mat4x4_mul(m, temp, m);
        //mat4x4_rotate_Y(m, m, (float)glfwGetTime());
        // move it left to right
        // orthographic projection
        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 0.1f, 100.0f);
        // perspective projection
        mat4x4_perspective(p, (float)3.14/4, ratio, 0.1f, 100.0f);

        // creating a view
        //vec3_sub(camDir, camPos, camTarget);
        vec3_norm(camDir, camTarget);
        vec3_norm(camDir, camDir);
        vec3_mul_cross(camRight, tempUp, camDir);
        vec3_norm(camRight, camRight);
        vec3_mul_cross(camUp, camDir, camRight);
        vec3_norm(camUp,camUp);

        LookAt[0][0] =  camRight[0];
        LookAt[0][1] =  camUp[0];
        LookAt[0][2] =  camDir[0];
        LookAt[0][3] =  0.f;

        LookAt[1][0] =  camRight[1];
        LookAt[1][1] =  camUp[1];
        LookAt[1][2] =  camDir[1];
        LookAt[1][3] =  0.f;

        LookAt[2][0] =  camRight[2];
        LookAt[2][1] =  camUp[2];
        LookAt[2][2] =  camDir[2];
        LookAt[2][3] =  0.f;

        LookAt[3][0] =  0.f;
        LookAt[3][1] =  0.f;
        LookAt[3][2] =  0.f;
        LookAt[3][3] =  1.f;

        mat4x4_translate_in_place(LookAt, -camPos[0], -camPos[1], -camPos[2]);

        mat4x4_mul(vp, p, LookAt);
        
        for (int i = 0; i < 3; i++)
        {
            mat4x4 m, mvp;

            mat4x4_identity(m);
            mat4x4_translate_in_place(m,
                cubePositions[i][0],
                cubePositions[i][1],
                cubePositions[i][2]);

            if (i == 0) {
                glBindTexture(GL_TEXTURE_2D, texture);
                glUseProgram(shaderProgram_textured);
                glBindVertexArray(VAO_textured);

                mat4x4_mul(mvp, vp, m);
                glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const float*)mvp);
            } else if (i == 2) {
                // light source
                glUseProgram(shaderProgram_lightSource);
                glBindVertexArray(VAO_lightSource);
                mat4x4 temp;

                mat4x4_identity(temp);

                mat4x4_scale_aniso(temp, temp, 0.4f, 0.4f, 0.4f);
                mat4x4_mul(m, m, temp);

                mat4x4_mul(mvp, vp, m);
                glUniformMatrix4fv(mvp_location_lightSource, 1, GL_FALSE, (const float*)mvp);
            } else {
                glBindTexture(GL_TEXTURE_2D, texture2);
                glUseProgram(shaderProgram_textured);
                glBindVertexArray(VAO_textured);

                mat4x4_mul(mvp, vp, m);
                glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const float*)mvp);
            }

            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/(6*sizeof(float)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void print_mat4x4(const mat4x4 M) {
    printf("Matrix:\n");
    for(int i = 0; i < 4; i++) {
        printf("[ %f %f %f %f ]\n",
            M[i][0], M[i][1], M[i][2], M[i][3]);
    }
}

void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //printf("W\n");
        vec3 temp;
        vec3_scale(temp, camDir, cameraSpeed);
        vec3_sub(camPos, camPos, temp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //printf("S\n");
        vec3 temp;
        vec3_scale(temp, camDir, -cameraSpeed);
        vec3_sub(camPos, camPos, temp);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //printf("A\n");
        vec3 temp;
        vec3_scale(temp, camRight, cameraSpeed);
        vec3_sub(camPos, camPos, temp);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //printf("D\n");
        vec3 temp;
        vec3_scale(temp, camRight, -cameraSpeed);
        vec3_sub(camPos, camPos, temp);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    if (yaw > 360.0f)
        yaw -= 360.0f;
    else if (yaw < 0.0f)
        yaw += 360.0f;

    //conversion to radians
    float radYaw = yaw * (M_PI / 180.0f);
    float radPitch = pitch * (M_PI / 180.0f);

    //printf("Yaw in radians: %f\n", radYaw);
    
    vec3 direction;
    direction[0] = cosf(radYaw) * cosf(radPitch);
    direction[1] = sinf(radPitch);
    direction[2] = sinf(radYaw) * cosf(radPitch);
    vec3_norm(camTarget, direction);
    //printf("Direction x: %f, y: %f, z: %f\n",direction[0], direction[1], direction[2]);
}

void load_text(char *filename, char *buf, int max_size)
{
    FILE *file_ptr;

    // Character buffer that stores the read character
    // till the next iteration
    char ch;

    // Opening file in reading mode
    file_ptr = fopen(filename, "r");

    int i = 0;
    // character by character using loop.
    while ((ch = fgetc(file_ptr)) != EOF && i<max_size)
        buf[i++] = ch;

    buf[i] = '\0';

    // Closing the file
    fclose(file_ptr);
}