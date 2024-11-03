#pragma once
#include <glm/glm.hpp>
#include <string>

class CardAtlas {
 private:
    unsigned int m_ShaderProgram;
    unsigned int m_VAO, m_VBO;
    unsigned int m_TextureID;
    float m_Scale;
    int m_Width, m_Height;
    int m_RowCnt, m_ColumnCnt;
    glm::vec2 m_Start;
    unsigned int m_RowPadding, m_ColumnPadding;

 public:
    CardAtlas() = default;
    CardAtlas(
        std::string, 
        unsigned int row_cnt, unsigned int column_cnt,
        unsigned int row_padding, unsigned int column_padding,
        glm::vec2 start, float scale
    );
    void Render(unsigned int card_row, unsigned int card_column, glm::vec2 scr_size, glm::vec2 pos);
};
