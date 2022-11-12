

// #ifndef TEXT_RENDERER_H
// #define TEXT_RENDERER_H

#include <map>
#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/engine.h"

namespace gfxc
{
    /// Holds all state information relevant to a character as loaded using FreeType

    // A renderer class for rendering text displayed by a font loaded using the
    // FreeType library. A single font is loaded, processed into a list of Character
    // items for later rendering.
    class TextRenderer
    {
    };
}


#pragma once

#include "components/simple_scene.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

namespace m1
{

    struct Character
    {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
        GLuint Advance;     // Horizontal offset to advance to next glyph
    };

    class Lab3 : public gfxc::SimpleScene
    {
    public:
        Lab3();
        ~Lab3();

        void Init() override;
        // void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void DrawHUD();

    protected:
        float cx, cy, bx, by, hx, hy, kx, ky, wx_1, wy_1, wx_2, wy_2;
        glm::mat3 modelBody, modelHead, modelWing1, modelWing2, modelBeak, modelHealth, modelHealth1, modelHealth2;
        glm::mat3 modelBullet1, modelBullet2, modelBullet3, modelScore, modelScoreBlock1, modelScoreBlock2, modelScoreBlock3, modelScoreBlock4, modelScoreBlock5;

        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float keepX, keepY;

        float start_x, start_y;
        float head_diff_x, head_diff_y, beak_diff_x, beak_diff_y;
        float wing1_diff_x, wing1_diff_y, wing2_diff_x, wing2_diff_y;
        float set_anugular_step;
        float set_anugular_step_wing2;
        float incline_wing2;

        bool SPAWN = 0;
        bool ACTIVE = 0;
        bool KILLED = 0;
        bool TO_KILL = 0;
        bool TO_ESCAPE = 0;
        bool ESCAPED = 0;
        float degrees;
        float incline;

        bool MOVE_UP = 0;
        bool MOVE_DOWN = 0;
        bool CHECK_UP = 1;
        bool CHECK_RIGHT = 1;
        bool CHECK_DOWN = 1;
        bool CHECK_LEFT = 1;

        bool CORNER_LEFT_DOWN = 0;
        bool CORNER_LEFT_UP = 0;
        bool CORNER_RIGHT_UP = 0;
        bool CORNER_RIGHT_DOWN = 0;

        float wing_angle1;
        float wing_angle2;
        bool sense_wing1;
        bool sense_wing2;
        int MOVE_ORDER;

        int lives;
        int bullets;
        int score;
        int level = 0;

        float t;
        bool LEVEL_UP =0;

        float t_text=0;

    public:
        // Holds a list of pre-compiled Characters
        std::map<GLchar, Character> Characters;

        // Shader used for text rendering
        Shader *m_textShader;

    public:
        // Constructor
        void TextRenderer(const std::string &selfDir, GLuint width, GLuint height);

        // Pre-compiles a list of characters from the given font
        void Load(std::string font, GLuint fontSize);

        // Renders a string of text using the precompiled list of characters
        void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

    private:
        // Render state
        GLuint VAO, VBO;

        // TODO(student): If you need any other class variables, define them here.
    };
} // namespace m1

#endif
