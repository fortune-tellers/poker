#pragma once
#include "glm/glm.hpp"
#include <string>

class CardAtlas {
 private:
    unsigned int m_ShaderProgram;
    unsigned int m_VAO, m_VBO;
    unsigned int m_TextureID;
    glm::vec2 m_Start, m_Stride;
    int m_Width, m_Height;

 public:
    glm::vec2 m_Size;
    float scale;
    CardAtlas() = default;
    CardAtlas(
        std::string, 
        glm::vec2 start,
        glm::vec2 size,
        glm::vec2 stride,
        float scale
    );
    void Render(unsigned int card_row, unsigned int card_column, glm::vec2 scr_size, glm::vec2 pos);
};
