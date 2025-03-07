#ifndef IMGUICONTEXT_H
#define IMGUICONTEXT_H
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

class IMGUIContext {
    public:
        IMGUIContext(GLFWwindow *window){
            //--------------imgui设置--------------
            // 创建GUI上下文
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void) io;
            // 设置imgui主题颜色
            ImGui::StyleColorsDark(); // 暗色   //ImGui::StyleColorsClassic();//经典皮肤
            // 设置平台
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            const char *glsl_version = "#version 330";
            ImGui_ImplOpenGL3_Init(glsl_version);
         }

        ~IMGUIContext() = default;
//        void init();
//        void shutdown();
        void newFrame(){
            // 创建imgui窗口
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        };

        void render() {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

};



#endif //IMGUICONTEXT_H
