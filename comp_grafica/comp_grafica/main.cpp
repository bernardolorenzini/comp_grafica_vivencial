

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "glad.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();

const GLuint WIDTH = 1000, HEIGHT = 1000;

const GLchar* vertexShaderSource = "#version 410\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"

"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 410\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

bool rotateX=false, rotateY=false, rotateZ=false, lowX=false, lowY=false, lowZ=false, growX=false, growY=false, growZ=false;

int main()
{
    glfwInit();


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Bernardo Lorenzini!", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

    }

    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    GLuint shaderID = setupShader();

    GLuint VAO = setupGeometry();


    glUseProgram(shaderID);

    glm::mat4 model = glm::mat4(1);
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    //
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        float angle = (GLfloat)glfwGetTime();

        model = glm::mat4(1);
        if (rotateX)
        {
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
            
        }
        else if (rotateY)
        {
            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

        }
        else if (rotateZ)
        {
            model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

        }

        glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));
  
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

      
        
        glDrawArrays(GL_POINTS, 0, 36);
        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        rotateX = true;
        rotateY = false;
        rotateZ = false;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        rotateX = false;
        rotateY = true;
        rotateZ = false;
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        rotateX = false;
        rotateY = false;
        rotateZ = true;
    }
  


}

int setupShader()
{
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Checando erros de compilação (exibição via log no terminal)
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Checando erros de compilação (exibição via log no terminal)
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Linkando os shaders e criando o identificador do programa de shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Checando por erros de linkagem
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


int setupGeometry()
{

    
    GLfloat vertices[] = {
            //Base do cubo: face frontal
            -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // V0
            -0.5,  0.5, -0.5, 1.0, 1.0, 0.0, // V1
             0.5,  0.5, -0.5, 1.0, 1.0, 0.0, // V2

            -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // V0
             0.5,  0.5, -0.5, 1.0, 1.0, 0.0, // V2
             0.5, -0.5, -0.5, 1.0, 1.0, 0.0, // V3

            //Face traseira
             0.5, -0.5, 0.5, 0.0, 1.0, 1.0, // V4
             0.5,  0.5, 0.5, 0.0, 1.0, 1.0, // V5
            -0.5,  0.5, 0.5, 0.0, 1.0, 1.0, // V6

             0.5, -0.5, 0.5, 0.0, 1.0, 1.0, // V4
            -0.5,  0.5, 0.5, 0.0, 1.0, 1.0, // V6
            -0.5, -0.5, 0.5, 0.0, 1.0, 1.0, // V7

            //Face superior
            -0.5,  0.5, -0.5, 1.0, 0.0, 1.0, // V1
            -0.5,  0.5,  0.5, 1.0, 1.0, 1.0, // V6
             0.5,  0.5,  0.5, 1.0, 0.0, 1.0, // V5

            -0.5,  0.5, -0.5, 1.0, 0.0, 1.0, // V1
             0.5,  0.5,  0.5, 1.0, 0.0, 1.0, // V5
             0.5,  0.5, -0.5, 1.0, 0.0, 1.0, // V2

            //Face inferior
            -0.5, -0.5,  0.5, 1.0, 0.0, 1.0, // V7
            -0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // V0
             0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // V3

            -0.5, -0.5,  0.5, 1.0, 0.0, 1.0, // V7
             0.5, -0.5, -0.5, 1.0, 0.0, 1.0, // V3
             0.5, -0.5,  0.5, 1.0, 0.0, 1.0, // V4

            //Face esquerda
            -0.5, -0.5,  0.5, 0.0, 1.0, 0.0, // V7
            -0.5,  0.5,  0.5, 0.0, 1.0, 0.0, // V6
            -0.5,  0.5, -0.5, 0.0, 1.0, 0.0, // V1

            -0.5, -0.5,  0.5, 0.0, 1.0, 0.0, // V7
            -0.5,  0.5, -0.5, 0.0, 1.0, 0.0, // V1
            -0.5, -0.5, -0.5, 0.0, 1.0, 0.0, // V0

            //Face direita
             0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // V3
             0.5,  0.5, -0.5, 0.0, 0.0, 1.0, // V2
             0.5,  0.5,  0.5, 0.0, 0.0, 1.0, // V5

             0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // V3
             0.5,  0.5,  0.5, 0.0, 0.0, 1.0, // V5
             0.5, -0.5,  0.5, 0.0, 0.0, 1.0, // V4
        };


    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VAO;
}
