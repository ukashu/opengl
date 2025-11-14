#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void print_mat4x4(const mat4x4 M);


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
        // top triangle
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// top left
        // bottom triangle
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// top left
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // bottom right
    // back side (red, z-axis static)
        // top triangle
        0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f, // top right
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5f, // bottom right
        -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f,// top left
        // bottom triangle
        -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f,// top left
        -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5f,// bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.5, // bottom right
    // left side (green, x-axis static)
        // top triangle
        0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, // top right
        0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f, // bottom right
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.5f,// top left
        // bottom triangle
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.5f,// top left
        0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.5f,// bottom left
        0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f, // bottom right
    // right side (green, x-axis static)
        // top triangle
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// top left
        // bottom triangle
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// top left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    // top side (blue, x-axis static)
        // top triangle
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top right
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// top left
        // bottom triangle
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,// top left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// bottom left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
    // bottom side (blue, y-axis static)
        // top triangle
        0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, // top right
        0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 1.0f, // bottom right
        -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f,// top left
        // bottom triangle
        -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f,// top left
        -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 1.0f,// bottom left
        0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 1.0f, // bottom right
    };
    

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex shader GLSL code
    const char *vertexShaderSource = "#version 330\n"
    "uniform mat4 MVP;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    " gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "ourColor = aColor;\n"
    "}\0";

    // create shader object and compile vertex shader code into it
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // checking if it compiled succesfully
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        return -1;
    }

    // fragment shader GLSL code
    const char *fragmentShaderSource = "#version 330\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(ourColor, 1.0f);\n"
    "}\0";

    // create shader object and compile fragment shader code into it
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        return -1;
    }

    // attaching and linking the shaders to a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::FAILED\n");
        return -1;
    }

    // delete shader objects
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader); 
    glEnable(GL_DEPTH_TEST);

    // telling OpenGL how to interpret the data from vertex buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind shader MVP variable to mvp_location variable in OpenGL
    mvp_location = glGetUniformLocation(shaderProgram, "MVP");

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m,p,mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // create an identity matrix and save into variable m
        mat4x4_identity(m);
        //printf("After identity:\n");
        //print_mat4x4(m);
        mat4x4_translate(m, sin((float)glfwGetTime()), 0.0f, 0.0f);
        mat4x4_rotate_X(m, m, (float)glfwGetTime());
        //printf("After rotate X:\n");
        //print_mat4x4(m);
        mat4x4_rotate_Y(m, m, (float)glfwGetTime());
        //printf("After rotate Y:\n");
        //print_mat4x4(m);
        // move it left to right
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        //printf("After ortho:\n");
        //print_mat4x4(p);
        mat4x4_mul(mvp, p, m);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/(6*sizeof(float)));

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