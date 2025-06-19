#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <vector>
#include <string>
#include "encrypt.h"
#include "decrypt.h"
#include <thread>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint LoadTexture(const char *filename, int *width, int *height){
    int channels;
    unsigned char* data = stbi_load(filename, width, height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLint format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

const std::vector<int> KEY1 = {3, 5, 2, 6, 1, 4};
const std::vector<char> KEY2 = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
    'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void setup() {
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW!" << std::endl;
        exit(1);
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "RIP TEHNIK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creating GLFW window!" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW!" << std::endl;
        exit(1);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

static std::string text = "";
static char input_text[128] = "";

void render(GLFWwindow* window) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 10.0f;
    style.WindowRounding = 10.0f;
    style.WindowBorderSize = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.4f, 0.8f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Градиентный фон
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, display_w, display_h, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            // Верх - голубой
            glColor4f(0.4f, 0.6f, 0.9f, 1.0f);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(display_w, 0.0f);

            // Низ - белый
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glVertex2f(display_w, display_h);
            glVertex2f(0.0f, display_h);
        glEnd();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const float windowWidth = 400.0f;
        const float windowHeight = 250.0f;
        const ImVec2 windowPos = ImVec2((1280 - windowWidth) * 0.5f, (720 - windowHeight) * 0.5f);
        static bool show_result = false;
        bool cyrilic = false;

        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

        ImGui::Begin("RIP TEHNIK", nullptr,
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoTitleBar);

        ImVec2 window_size = ImGui::GetWindowSize();
        float center_x = window_size.x * 0.5f;       
        ImGui::Dummy(ImVec2(0.0f, 20.0f)); 

        const float inputWidth = 300.0f;
        ImGui::SetCursorPosX(center_x - inputWidth * 0.5f);
        ImGui::SetNextItemWidth(inputWidth);
        ImGui::InputTextWithHint("##Input", "Input",input_text, IM_ARRAYSIZE(input_text));
        for(size_t i = 0; input_text[i] != '\0'; ++i){
            unsigned char c = static_cast<unsigned char>(input_text[i]);
            if(c >= 0xD0 && c <= 0xD1){
                cyrilic = true;
                break;
            }
        }

        if(cyrilic){
            ImGui::BeginTooltip();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
            ImGui::Text("Cyrillic is not available");
            ImGui::PopStyleColor();
            ImGui::EndTooltip();
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        const ImVec2 buttonSize = ImVec2(120, 35);
        float buttonPosX = (ImGui::GetWindowSize().x - buttonSize.x) * 0.5f;

        ImGui::SetCursorPosX(buttonPosX);
        if (ImGui::Button("Encrypt", buttonSize)) {
            std::string encrypted = encrypt::trsp_encrypt(input_text, KEY1);
            std::string caesar_encrypted = encrypt::caesar_encrypt(encrypted, KEY2);
            text = caesar_encrypted;
            show_result = true;
        }

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::SetCursorPosX(buttonPosX);
        if (ImGui::Button("Decrypt", buttonSize)) {
            std::string decrypted_caesar = decrypt::decrypted_caesar(input_text, KEY2);
            std::string decrypted = decrypt::trsp_decrypt(decrypted_caesar, KEY1);
            text = decrypted;
            show_result = true;
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        float resultPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f;
        ImGui::SetCursorPosX(resultPosX);
        if (show_result) {
            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            float resultPosX = (ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f;
            ImGui::SetCursorPosX(resultPosX);
            ImGui::Text("Result: %s", text.c_str());
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

int main() {
    setup();
    GLFWwindow* window = glfwGetCurrentContext();
    render(window);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
