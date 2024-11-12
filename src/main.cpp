#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <optional>
#include <stdio.h>

#include "image.h"
#include "card.h"

static constexpr float card_size_screen_portion = 0.05;
static std::optional<size_t> current_card_id{};
static glm::vec2 current_card_size;

struct CardRepresentation {
    std::optional<Card> card{};
    glm::vec2 pos;
    int player = 0;
};

static constexpr int player_count = 5;
static CardRepresentation cards[2 * player_count + 5]{};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    current_card_id = {};
    glm::vec2 pos { x / display_w, 1.0 - (y / display_h) };
    glm::vec2 rel_card_size = current_card_size / glm::vec2(display_w, display_h);
    std::cout << std::endl;
    for (size_t i = 0; i < 2 * player_count + 5; i++) {
        if (pos.x < cards[i].pos.x || pos.x > cards[i].pos.x + rel_card_size.x)
            continue;
        if (pos.y < cards[i].pos.y || pos.y > cards[i].pos.y + rel_card_size.y)
            continue;
        current_card_id = i;
        break;
    }
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::vec2 card_size(105.0f, 147.0f);
    CardAtlas atlas(
        "resources/cards.jpg",
        glm::vec2(19.0f, 31.0f),
        card_size,
        glm::vec2(112.0f, 153.0f),
        1.0f
    );
    CardAtlas table(
        "resources/table.jpg",
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1920.0f, 720.0f),
        glm::vec2(0.0f, 0.0f),
        1.0f
    );

    float left_padding = (1.0f - 2.0f * card_size_screen_portion - 0.01f) * 0.5f;
    cards[0].pos = { left_padding, 0.01f };
    cards[1].pos = { left_padding + card_size_screen_portion + 0.01f, 0.01f };
    cards[0].player = 0;
    cards[1].player = 0;

    float opponent_y = 1.0f - card_size_screen_portion - 0.05f;
    float padding_between_players = 0.03f;
    float padding_between_cards = 0.01f;
    left_padding = 
        (1.0f 
         - (player_count - 2) * padding_between_players
         - 2 * (player_count - 1) * card_size_screen_portion
         - (player_count - 1) * padding_between_cards
        ) * 0.5f;
    for (size_t i = 1; i < player_count; i++) {
        float x = left_padding + (i - 1) * (2.0f * card_size_screen_portion + padding_between_cards + padding_between_players);
        cards[2 * i + 0].pos = { x, opponent_y };
        cards[2 * i + 1].pos = { x + card_size_screen_portion + padding_between_cards, opponent_y };
        cards[2 * i + 0].player = i;
        cards[2 * i + 1].player = i;
    }

    left_padding = 
        (1.0f 
         - 5 * card_size_screen_portion
         - 4 * padding_between_cards
        ) * 0.5f;

    for (size_t i = 0; i < 5; i++) {
        float x = left_padding + i * (padding_between_cards + card_size_screen_portion);
        cards[2 * player_count + i].pos = {x, 0.5f + card_size_screen_portion};
        cards[2 * player_count + i].player = -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (current_card_id.has_value()) {
            auto &current_card = cards[*current_card_id].card;
            if (cards[*current_card_id].player >= 0) {
                ImGui::Text(
                    "Select card %d for player %d", 
                    (int)(*current_card_id) % 2,
                    cards[*current_card_id].player
                );
            } else {
                ImGui::Text(
                    "Select card %d on the table",
                    (int)(*current_card_id) - 2 * player_count
                );
            }
            bool known = current_card.has_value();
            ImGui::Checkbox("Known", &known); 
            if (!known) {
                current_card = {};
            } else if (!current_card.has_value()) {
                current_card = { 2, Suit::Clubs };
            }

            if (current_card.has_value()) {
                ImGui::SliderInt(
                    "Card power",
                    &current_card->power,
                    2, 14,
                    "%d",
                    ImGuiSliderFlags_AlwaysClamp
                );

                const int current_suit = (int)current_card->suit;
                if (ImGui::BeginCombo("Card suit", suit_names[current_suit])) {
                    for (size_t i = 0; i < 4; i++) {
                        if (ImGui::Selectable(suit_names[i], i == current_suit)) {
                            current_card->suit = (Suit)i;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        glm::vec2 scr_size(display_w, display_h);
        atlas.scale = (scr_size * card_size_screen_portion / card_size).x;
        table.m_Size = scr_size;

        table.Render(0, 0, scr_size, glm::vec2(0, 0));
        current_card_size = card_size * atlas.scale;
        for (size_t i = 0; i < 2 * player_count + 5; i++) {
            const auto card = cards[i].card;
            int row, column;
            if (card.has_value()) {
                column = card->power - 2;
                row = (int)card->suit;
            } else {
                column = 13;
                row = 2;
            }
            atlas.Render(
                row, column, scr_size,
                cards[i].pos * scr_size
            );
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
