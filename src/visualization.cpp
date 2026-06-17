#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Defining shaders. Vertex shader runs once per vertex and decides where on screen it goes. After step 5 vertex got a color to pass along to fragment
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;
    layout(location = 2) in float aPiece;
    layout(location = 3) in float aPieceColor;
    layout(location = 4) in vec2 aUV;

    out vec3 vertexColor;
    out vec2 uv;
    out float piece;
    out float pieceColor;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        vertexColor = aColor;
        piece = aPiece;
        pieceColor = aPieceColor;
        uv = aUV - vec2(0.5, 0.5);
    }
)";
// Fragment shader runs once per pixel and decides what color it is
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 vertexColor;
    in vec2 uv;
    in float piece;
    in float pieceColor;
    void main() {
        if (piece > 0.5) {
            // check distance from center of square
            float dist = length(uv);
            if (dist > 0.4) {
                FragColor = vec4(vertexColor, 1.0);
                return;
            }

            // pieceColor 0.0 = white piece, 1.0 = black piece
            if (pieceColor < 0.5) {
                FragColor = vec4(1.0, 1.0, 1.0, 1.0); // white piece
            } else {
                FragColor = vec4(0.1, 0.1, 0.1, 1.0); // black piece
            }
        } else {
            FragColor = vec4(vertexColor, 1.0); // no piece, just square color
        }
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

    // Registers a callback whenever the window is resized to avoid it looked off
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
    int size = std::min(width, height);
    int x = (width - size) / 2;
    int y = (height - size) / 2;
    glViewport(x, y, size, size);
    });

    // force it to run once for the initial window size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    int size = std::min(width, height);
    glViewport((width - size) / 2, (height - size) / 2, size, size);

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
    float vertices[64 * 4 * 9];
    // 64 squares, 6 indices each
    unsigned int indices[64 * 6];

    float squareSize = 2.0f / 8.0f;
    int vertexOffset = 0;
    int indexOffset = 0;

    // 0 = empty, 1 = white piece, 2 = black piece.
    int board[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 1}, // row 0 - white back rank
        {1, 1, 1, 1, 1, 1, 1, 1}, // row 1 - white pawns
        {0, 0, 0, 0, 0, 0, 0, 0}, // row 2 - empty
        {0, 0, 0, 0, 0, 0, 0, 0}, // row 3 - empty
        {0, 0, 0, 0, 0, 0, 0, 0}, // row 4 - empty
        {0, 0, 0, 0, 0, 0, 0, 0}, // row 5 - empty
        {2, 2, 2, 2, 2, 2, 2, 2}, // row 6 - black pawns
        {2, 2, 2, 2, 2, 2, 2, 2}, // row 7 - black back rank
    };


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

            //Flag = 0 empty square, Flag = 1 draw circle, Col = 0 white, Col = 1 black
            float pieceFlag = (board[row][col] != 0) ? 1.0f : 0.0f;
            float pieceCol  = (board[row][col] == 2) ? 1.0f : 0.0f;

            int base = vertexOffset; //Where we are before writing the square's vertices

            // 4 corners of this square, each with position (x,y) + color (rgb). vertexOffset++ just increments so they are placed in the next one
            // bottom left
            vertices[vertexOffset++] = x;
            vertices[vertexOffset++] = y;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;
            vertices[vertexOffset++] = pieceFlag;
            vertices[vertexOffset++] = pieceCol;
            vertices[vertexOffset++] = 0.0f; // u (bottom-left corner of UV)
            vertices[vertexOffset++] = 0.0f; // v

            // bottom right (the same but now x+squaresize so we move from left to right corner)
            vertices[vertexOffset++] = x + squareSize;
            vertices[vertexOffset++] = y;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;
            vertices[vertexOffset++] = pieceFlag;
            vertices[vertexOffset++] = pieceCol;
            vertices[vertexOffset++] = 1.0f; // u (bottom-left corner of UV)
            vertices[vertexOffset++] = 0.0f; // v

            // top right (both x and y is increased)
            vertices[vertexOffset++] = x + squareSize;
            vertices[vertexOffset++] = y + squareSize;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;
            vertices[vertexOffset++] = pieceFlag;
            vertices[vertexOffset++] = pieceCol;
            vertices[vertexOffset++] = 1.0f; // u (bottom-left corner of UV)
            vertices[vertexOffset++] = 1.0f; // v

            // top left
            vertices[vertexOffset++] = x;
            vertices[vertexOffset++] = y + squareSize;
            vertices[vertexOffset++] = r;
            vertices[vertexOffset++] = g;
            vertices[vertexOffset++] = b;
            vertices[vertexOffset++] = pieceFlag;
            vertices[vertexOffset++] = pieceCol;
            vertices[vertexOffset++] = 0.0f; // u (bottom-left corner of UV)
            vertices[vertexOffset++] = 1.0f; // v

            // 2 triangles using the 4 corners above
            // base is where this square's vertices start in the array, but each vertex is 9 floats so divide by 9 to convert. Then + the corners of the square
            // triangle 1
            indices[indexOffset++] = base / 9 + 0;
            indices[indexOffset++] = base / 9 + 1;
            indices[indexOffset++] = base / 9 + 2;
            // triangle 2
            indices[indexOffset++] = base / 9 + 2;
            indices[indexOffset++] = base / 9 + 3;
            indices[indexOffset++] = base / 9 + 0;
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
    // position — location 0, 2 floats, offset 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color — location 1, 3 floats, offset after 2 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // piece flag — location 2, 1 float, offset after 5 floats
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // piece color — location 3, 1 float, offset after 6 floats
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // uv — location 4, 2 floats, offset after 7 floats
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(4);


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