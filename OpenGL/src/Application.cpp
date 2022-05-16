#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Textutre.h"
#include "Shader.h"
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit()!= GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    /* Loop until the user closes the window */
    float position[] = {
       -0.5f, -0.5f,  0.0f, 0.0f,  //0
        0.5f, -0.5f,  1.0f, 0.0f, //1
        0.5f,  0.5f,  1.0f, 1.0f,  //2
       -0.5f,  0.5f,  0.0f, 1.0f  //3

    };

    // Index Buffer Object
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3 , 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


    // Creating Vertex Array Explicitly for core Version
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Creating Vertex Array
    VeretexArray va;
    // Creating Vertex Buffer
    VertexBuffer vb(position, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);


    // Creating Index Buffer
    IndexBuffer ib(indicies, 6);
    Shader shader("res/shaders/Basic.Shader");
    shader.Bind();

    //shader.ParseShader("res/shaders/Basic.Shader");
    //ShaderProgramSource source = ParseShader();
    //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //GLCall(glUseProgram(shader));


  /*  GLCall(int location = glGetUniformLocation(shader, "u_color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.4f, 0.3f, 0.8f, 0.1));*/

    // Unbinding buffers

    //GLCall(glUseProgram(0));
    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    Texture texture("res/textures/dice.png");
    texture.Bind();
    shader.SetUniform1i("u_textutre", 0);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();
    Renderer renderer;
    //Animation
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 6);



        //GLCall(glUseProgram(shader));
        //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        shader.Bind();
     
        // checking Error 
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        renderer.Draw(va, ib, shader);
        /* Swap front and back buffers */

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if ( r < 0.0f){
            increment = 0.05f;
        }

        r += increment;
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}