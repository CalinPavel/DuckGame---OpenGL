#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab3 : public gfxc::SimpleScene
    {
     public:
        Lab3();
        ~Lab3();

        void Init() override;

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

     protected:
        float cx , cy , bx , by ,hx ,hy ,kx,ky , wx_1 , wy_1 , wx_2 , wy_2;
        glm::mat3 modelBody , modelHead , modelWing1 , modelWing2 , modelBeak;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float keepX , keepY;
      
         float start_x , start_y;
         float head_diff_x ,head_diff_y, beak_diff_x,beak_diff_y;
         float wing1_diff_x ,wing1_diff_y ,wing2_diff_x ,wing2_diff_y;
         float set_anugular_step;

         bool SPAWN = 0;
         bool ACTIVE = 0;
         bool KILLED = 0;
         bool ESCAPED = 0;
         float degrees;
         float incline;

         bool MOVE_UP = 0;
         bool MOVE_DOWN=0;
         bool CHECK_UP=1;
         bool CHECK_RIGHT=1;
         bool CHECK_DOWN=1;
         bool CHECK_LEFT=1;



        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
