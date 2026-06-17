#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Defining shaders. Vertex shader runs once per vertex and decides where on screen it goes
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";
// Fragment shader runs once per pixel and decides what color it is
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0);
    }
)";

int visualizationMain() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Chess", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //compiling shaders
    // 1. Create a shader object
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // 2. Attach your source string to it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // 3. Compile it
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // 4. Check if it worked
    int vertexSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (!vertexSuccess) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "Vertex Shader error: " << log << std::endl;
    }
    int fragmentSuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!fragmentSuccess) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cout << "Fragment Shader error: " << log << std::endl;
    }

    //Link the 2 shaders to one program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Check for errors with the program
    int programSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
    if (!programSuccess) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cout << "shader Program error: " << log << std::endl;
    }

    //deletes the fragment and vertex as they are now in program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram); //Tells GPU which shaders to use

    //Open Window
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();   // process keyboard/mouse/window events
        glClearColor(1,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);  // show the rendered frame

        
    }



    glfwTerminate();
    return 0;
}