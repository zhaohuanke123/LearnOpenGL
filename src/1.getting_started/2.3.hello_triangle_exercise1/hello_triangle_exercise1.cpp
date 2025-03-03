#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: 初始化并配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "创建 GLFW 窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化 GLAD 失败" << std::endl;
        return -1;
    }


    // 构建并编译着色器程序
    // ------------------------------------
    // 顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检查顶点着色器编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检查片段着色器编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 链接着色器程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置顶点数据（以及缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
    // 添加一组新的顶点以构成第二个三角形（共 6 个顶点）；顶点属性配置保持不变（每个顶点依然是一个包含 3 个浮点数的位置向量）
    float vertices[] = {
        // 第一个三角形
        -0.9f, -0.5f, 0.0f,  // 左侧
        -0.0f, -0.5f, 0.0f,  // 右侧
        -0.45f, 0.5f, 0.0f,  // 顶部
        // 第二个三角形
         0.0f, -0.5f, 0.0f,  // 左侧
         0.9f, -0.5f, 0.0f,  // 右侧
         0.45f, 0.5f, 0.0f   // 顶部
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 先绑定顶点数组对象，再绑定和设置顶点缓冲区，然后配置顶点属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 注意，这样是允许的，调用 glVertexAttribPointer 时注册了 VBO 作为顶点属性绑定的顶点缓冲区对象，因此之后可以安全解绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 之后可以解绑 VAO，这样其他 VAO 调用就不会意外修改此 VAO，但这种情况很少发生。
    // 修改其他 VAO 仍然需要调用 glBindVertexArray，所以一般情况下我们不会在没有必要的情况下解绑定 VAO（也不会解绑定 VBO）。
    glBindVertexArray(0);


    // 取消下面的注释以绘制线框多边形。
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 输入处理
        // -----
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制两个三角形（总共 6 个顶点）
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // 因为这里只有一个 VAO，所以每次绑定不是必须的，但为了保持代码条理我们还是绑定
        glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制 6 个顶点构成的两个三角形
        // glBindVertexArray(0); // 每次绘制后无需解绑

        // glfw: 交换缓冲区并轮询 IO 事件（按键按下/释放、鼠标移动等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选：在资源不再需要时释放所有资源
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: 结束，释放之前分配的所有 GLFW 资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// 处理所有输入：查询 GLFW 是否有相关按键在当前帧被按下或释放，并作出相应响应
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小变化（由操作系统或用户调整）时执行此回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保视口与窗口的新尺寸相匹配；注意在 Retina 显示器上，宽度和高度会远大于所指定的尺寸。
    glViewport(0, 0, width, height);
}
