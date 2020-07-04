#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource ParseShader(std::string filepath) {
    std::fstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;

            }
        } else if(type != ShaderType::NONE) {
            ss[(int)type] << line << "\n";
        }

    }
    return { ss[0].str(), ss[1].str() };

}
static int CompileShader(unsigned int type, const std::string& source) {
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));


    // Errror Handling of Shader
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to Compile " << ((type == GL_VERTEX_SHADER) ? "VertexShader" : "fragmentShader") << message;
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}
static unsigned int  CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));


    GLCall(glDeleteShader(vs));  // Delete intermideiate object created while compling shader source cod)e
    GLCall(glDeleteShader(fs));

    return program;
}
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
       -0.5f, -0.5f, //0
        0.5f, -0.5f, //1
        0.5f,  0.5f, //2
       -0.5f,  0.5f, //3

    };

    // Index Buffer Object
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3 , 0
    };


    // Creating Vertex Array Explicilty fotr core Version
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Creating Vertex Array
    VeretexArray va;
    // Creating Vertex Buffer
    VertexBuffer vb(position, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);


    // Creating Index Buffer
    IndexBuffer ib(indicies, 6);
    ShaderProgramSource source = ParseShader("res/shaders/Basic.Shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.4f, 0.3f, 0.8f, 0.1));


    // Unbinding buffers

    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //Animation
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 6);



        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        va.Bind();
        ib.Bind();

        // Cheking Error 
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
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
    GLCall(glDeleteProgram(shader));
    glfwTerminate();
    return 0;
}