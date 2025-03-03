#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// 设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: 初始化和配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw 窗口创建
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "创建 GLFW 窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "初始化 GLAD 失败" << std::endl;
        return -1;
    }

    // 构建并编译着色器程序
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // 你可以随意命名你的着色器文件

    // 设置顶点数据（和缓冲对象）并配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
        // 顶点位置          // 颜色
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下角
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // 顶部
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 先绑定顶点数组对象，再绑定并设置顶点缓冲区，最后配置顶点属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // 顶点颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 你可以解除绑定 VAO，以免其他 VAO 调用意外修改此 VAO，但这种情况很少发生。
    // 修改其他 VAO 时也需要调用 glBindVertexArray，所以通常没有必要解除绑定 VAO（或 VBO）。
    // glBindVertexArray(0);


    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        // -----
        processInput(window);

        // 渲染指令
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染三角形
        ourShader.use();

        float offset = 0.5f;
        ourShader.setFloat("xOffset", offset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: 交换缓冲并轮询 IO 事件（按键、鼠标移动等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选：在资源不再需要后释放所有资源：
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: 终止，清理所有之前分配的 GLFW 资源。
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// 处理所有输入：查询 GLFW 是否在此帧按下/释放相关按键，并作出相应反应
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小改变（由操作系统或用户调整大小）时，会执行此回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // 确保视口与新窗口尺寸匹配；请注意，在 Retina 显示器上宽度和高度会显著大于指定值。
    glViewport(0, 0, width, height);
}
