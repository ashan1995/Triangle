#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

enum class ShaderType {
    NONE=-1,VERTEX=0,FRAGMENT=1
};

static void ParseShader(const std:: string& filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream,line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("#vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("#fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
}

static unsigned int CompileShader(unsigned int type,const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader !" << std::endl;
        std::cout << message << std::endl;
    }

    return id;

}

static unsigned int CreateShader(const std::string& vertexShader,const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void){ 

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    std::cout<< glGetString(GL_VERSION) <<std::endl;

    float positions[6] = { //create an array for vertex positions
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f,
    };

    unsigned int buffer;    //identifier
    glGenBuffers(1, &buffer);    //create buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  //select buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //copying the array to buffer

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2, 0); //setting up vertex attribute and layout

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

       /* glBegin(GL_TRIANGLES);

        glVertex2f(-0.5f,-0.5f);
        glVertex2f( 0.0f, 0.5f);
        glVertex2f( 0.5f, -0.5f);
        
        glEnd();*/

        glDrawArrays(GL_TRIANGLES, 0, 3);  //The draw call

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}