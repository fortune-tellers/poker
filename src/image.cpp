#include <glad/glad.h>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "image.h"
#include "stb_image.h"

CardAtlas::CardAtlas(
    std::string path,
    unsigned int row_cnt, unsigned int column_cnt,
    unsigned int row_padding, unsigned int column_padding,
    glm::vec2 start, float scale
) : m_Scale(scale), m_RowCnt(row_cnt), m_ColumnCnt(column_cnt), m_RowPadding(row_padding), m_ColumnPadding(column_padding) {
    int nrComponents;
    unsigned char *data = reinterpret_cast<unsigned char*>(
    stbi_load(path.c_str(), &m_Width, &m_Height, &nrComponents, 0));

    m_Start = start / glm::vec2(m_Width, m_Height);

    if (!data) {
      stbi_image_free(data);
      std::cout << "Failed to open card atlas at " << path.c_str() << std::endl;
      return;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const auto createShader = [&](std::string path, unsigned int type) -> unsigned int {
        std::ifstream shaderFile(path);
        if (!shaderFile.good()) {
            std::cout << "Could not read shader: " << path.c_str() << std::endl;
            return -1;
        }
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        unsigned int shader = glCreateShader(type);
        std::string textStr = shaderStream.str();
        const char *text = textStr.c_str();
        glShaderSource(shader, 1, &text, NULL);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Failed to compile shader: " << infoLog << std::endl;
        }
        return shader;
    };
    const unsigned int vShader = createShader("resources/image.vert", GL_VERTEX_SHADER);
    const unsigned int fShader = createShader("resources/image.frag", GL_FRAGMENT_SHADER);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vShader);
    glAttachShader(m_ShaderProgram, fShader);
    glLinkProgram(m_ShaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cout << "Filed to link shader program: " << infoLog << std::endl;
        assert(false);
    }
}

void CardAtlas::Render(
    unsigned int card_row, unsigned int card_column,
    glm::vec2 scr_size, glm::vec2 pos
) {
    assert(card_row >= 0 && card_row < m_RowCnt && "Row id is out of bounds");
    assert(card_column >= 0 && card_column < m_ColumnCnt && "Column id is out of bounds");

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_ShaderProgram);
    glm::mat4 projection = glm::ortho(0.0f, scr_size.x, 0.0f, scr_size.y);
    glUniformMatrix4fv(
        glGetUniformLocation(m_ShaderProgram, "projection"),
        1, GL_FALSE, glm::value_ptr(projection));
    const glm::vec3 color(1);
    glUniform3fv(
        glGetUniformLocation(m_ShaderProgram, "color"),
        1, glm::value_ptr(color));
    glUniform1i(
        glGetUniformLocation(m_ShaderProgram, "text"),
        0
    );

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    float xpos = pos.x;
    float ypos = pos.y;
    float w = m_Width * m_Scale / m_ColumnCnt;
    float h = m_Height * m_Scale / m_RowCnt;
    unsigned row_height = m_RowPadding + (m_Height - (m_RowCnt + 1) * m_RowPadding) / m_RowCnt;
    unsigned column_width = m_ColumnPadding + (m_Width - (m_ColumnCnt + 1) * m_ColumnPadding) / m_ColumnCnt;
    glm::vec2 uv_start = glm::vec2(
        column_width * card_column / (float)m_Width,
        row_height * card_row / (float)m_Height
    ) + m_Start;

    glm::vec2 uv_end = glm::vec2(
        (column_width * (card_column + 1) - m_ColumnPadding - 1) / (float)m_Width,
        (row_height * (card_row + 1) - m_RowPadding - 1) / (float)m_Height
    ) + m_Start;

    float vertices[6][4] = {
    { xpos,     ypos + h,   uv_start.x, uv_start.y },
    { xpos,     ypos,       uv_start.x, uv_end.y },
    { xpos + w, ypos,       uv_end.x, uv_end.y },

    { xpos,     ypos + h,   uv_start.x, uv_start.y },
    { xpos + w, ypos,       uv_end.x, uv_end.y },
    { xpos + w, ypos + h,   uv_end.x, uv_start.y }
    };

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
