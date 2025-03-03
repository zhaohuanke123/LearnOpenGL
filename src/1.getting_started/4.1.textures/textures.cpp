#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>

#include <iostream>
#include <chrono> // 引入 chrono 库

std::chrono::time_point start = std::chrono::high_resolution_clock::now();
// 宏定义用于测量时间
#define MEASURE_TIME_START  start = std::chrono::high_resolution_clock::now();

#define MEASURE_TIME_END(msg) \
    do { \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::cout << msg << ": " \
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() \
                  << " 微秒" << std::endl; \
    } while (0)

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// 设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 存储我们看到的两个纹理的混合值
float mixValue = 0.2f;

int main() {
    // glfw：初始化并配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw：创建窗口
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "创建 GLFW 窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad：加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "初始化 GLAD 失败" << std::endl;
        return -1;
    }

    // 构建并编译我们的着色器程序
    // ------------------------------------
    MEASURE_TIME_START
    Shader ourShader("4.5.texture.vs", "4.5.texture.fs");
    MEASURE_TIME_END("Shader 编译时间");

    // 设置顶点数据（以及缓冲区）和配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
        // 顶点位置          // 颜色              // 纹理坐标
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上角
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // 左上角
    };
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 测量 glBindBuffer 和 glBufferData 的时间
    MEASURE_TIME_START

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    MEASURE_TIME_END("VBO 数据加载时间");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    MEASURE_TIME_START
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    MEASURE_TIME_END("顶点属性设置时间");

    // 加载并创建纹理
    // -------------------------
    unsigned int texture1, texture2;
    // 纹理 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理环绕为 GL_REPEAT（默认环绕方式）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 测量加载纹理的时间
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // 告诉 stb_image.h 在加载的纹理上进行 y 轴翻转
    unsigned char *data = nullptr;

    MEASURE_TIME_START
    data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height,
                     &nrChannels, 0);
    MEASURE_TIME_END("纹理 1 加载时间");

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "加载纹理失败" << std::endl;
    }
    stbi_image_free(data);

    // 纹理 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 设置纹理环绕参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理环绕为 GL_REPEAT（默认环绕方式）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 测量加载纹理的时间
    MEASURE_TIME_START
    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels,
                     0);
    MEASURE_TIME_END("纹理 2 加载时间");

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "加载纹理失败" << std::endl;
    }
    stbi_image_free(data);

    // 告诉 OpenGL 每个采样器所属的纹理单元（只需设置一次）
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // 别忘了在设置 uniform 之前激活/使用着色器!

    // 测量 glUniform1i 的时间
    MEASURE_TIME_START
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    MEASURE_TIME_END("设置 texture1 uniform 时间");

    MEASURE_TIME_START
    ourShader.setInt("texture2", 1);
    MEASURE_TIME_END("设置 texture2 uniform 时间");

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // 输入
        // -----
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 在对应的纹理单元上绑定纹理
        MEASURE_TIME_START
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        MEASURE_TIME_END("绑定纹理时间");

        // 在着色器中设置纹理混合值
        ourShader.setFloat("mixValue", mixValue);

        // 渲染容器
        ourShader.use();
        glBindVertexArray(VAO);

        // 测量绘制时间
        MEASURE_TIME_START
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        MEASURE_TIME_END("绘制时间");

        // glfw：交换缓冲区并轮询 IO 事件（按键按下/松开、鼠标移动等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选：在资源不再使用后释放所有资源
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw：终止，清除所有之前分配的 GLFW 资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// 处理所有输入：询问 GLFW 当前帧中相关按键是否被按下/松开，并做出相应处理
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.01f; // 根据系统硬件调整这个值（可能会太慢或太快）
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue -= 0.01f; // 根据系统硬件调整这个值（可能会太慢或太快）
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}

// glfw：当窗口大小发生变化（由操作系统或用户调整）时，此回调函数将被执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // 确保视口与新的窗口尺寸相匹配；注意，在 Retina 显示器上宽度和高度可能远大于指定值
    glViewport(0, 0, width, height);
}
