#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Defining shaders. Vertex shader runs once per vertex and decides where on screen it goes. After step 5 vertex got a color to pass along to fragment
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;

    out vec3 vertexColor;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        vertexColor = aColor;
    }
)";
// Fragment shader runs once per pixel and decides what color it is
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 vertexColor;
    void main() {
        FragColor = vec4(vertexColor, 1.0);
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

    //VBO - Vertex buffer object. Chunk of memory on the GPU telling where the vertex data is stored. Define 4 corners
    // float vertices[] = {
    // -0.5f, -0.5f,  // bot left 
    //  0.5f, -0.5f,  // bot right 
    //  0.5f,  0.5f,  // top right   
    // -0.5f,  0.5f   // top left  
    // };

    // //EBO - Element Buffer Object. OpenGL draws triangles, this tells which vertices for each triangle
    // unsigned int indices[] = {
    // 0, 1, 2,  // (bot-left, bot-right, top-right)
    // 2, 3, 0   // (top-right, top-left, bot-left)
    // };

    // 64 squares, 4 vertices each, 5 floats per vertex
    float vertices[64 * 4 * 5];
    // 64 squares, 6 indices each
    unsigned int indices[64 * 6];

    float squareSize = 2.0f / 8.0f;
    int vertexOffset = 0;
    int indexOffset = 0;


    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            float x = -1.0f + col * squareSize;
            float y = -1.0f + row * squareSize;

            float r, g, b;
            if ((col + row) % 2 == 0) {
                r = 0.9f; g = 0.9f; b = 0.8f; // light squares
            } else {
                r = 0.2f; g = 0.2f; b = 0.2f; // dark squares
            }

            int base = vertexOffset; //Where we are before writing the square's vertices

            // 4 corners of this square, each with position (x,y) + color (rgb). vertexOffset++ just increments so they are placed in the next one
            // bottom left
            vertices[vertexOffset++] = x;
            vertices[vertexOffset++] = y;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;

            // bottom right (the same but now x+squaresize so we move from left to right corner)
            vertices[vertexOffset++] = x + squareSize;
            vertices[vertexOffset++] = y;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;

            // top right (both x and y is increased)
            vertices[vertexOffset++] = x + squareSize;
            vertices[vertexOffset++] = y + squareSize;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;

            // top left
            vertices[vertexOffset++] = x;
            vertices[vertexOffset++] = y + squareSize;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;

            // 2 triangles using the 4 corners above
            // base is where this square's vertices start in the array, but each vertex is 5 floats so divide by 5 to convert. Then + the corners of the square
            // triangle 1
            indices[indexOffset++] = base / 5 + 0;
            indices[indexOffset++] = base / 5 + 1;
            indices[indexOffset++] = base / 5 + 2;
            // triangle 2
            indices[indexOffset++] = base / 5 + 2;
            indices[indexOffset++] = base / 5 + 3;
            indices[indexOffset++] = base / 5 + 0;
        }
    }

    //VAO - Vertex Array Object. Remembers setup/configuration. Create and bind VAO first
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    //Create VBO and upload vertex data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Create the EBO and upload index data
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. Tell OpenGL how to read the vertex data
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //Location 0, 2 values (xy), floats, dont normalize, how many bites to jump (2 float wide), data starts at beginning of buffer
    // glEnableVertexAttribArray(0);

    // position attribute — location 0, 2 floats, starts at byte 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute — location 1, 3 floats, starts after the first 2 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Open Window
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();   // process keyboard/mouse/window events
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 64 * 6, GL_UNSIGNED_INT, 0); //draw triangles using 64 * 6 indices that are unsigned ints, starting at index 0

        glfwSwapBuffers(window);  // show the rendered frame

        
    }

    glfwTerminate();
    return 0;
}