#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>
#include <cstdlib>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;

#define PI 3.1415f
#define MAX_ANGLE 1.2f
#define MIN_ANGLE -1.2f

#define MAX_LEN_SPAWN 1100
#define MIN_LEN_SPAWN 100



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

bool pulse =true;
bool comeback = true;


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 60;
    float l = 60; // body
    float l_beak=30;
    float l_wing = 50;

    cx = corner.x ; // 75
    cy = corner.y ; // 75

    //body center
    bx = l / 2;
    by = 2*l / 3;

    //head center
    hx = 0;
    hy = 4*l /3;

    //beak center
    kx= l_beak/2;
    ky = 4*l/3;

    //wing1 center
    wx_1 = 20; 
    wy_1 = 0;

    //wing2 center
    wx_2 = 70; 
    wy_2 = 0;

    //spawn variables for differt start positions
    //100 -1100 x
    start_x=500, start_y=100;
    // srand(time(NULL)); // Seed the time
    int Num = rand()%(MAX_LEN_SPAWN-MIN_LEN_SPAWN+1)+MIN_LEN_SPAWN;
    cout << Num << "\n";
    // start_x = (float) Num;

    keepX = start_x + l/2;
    keepY = start_y + 2*l /3;

    // keepX = 0;
    // keepY = 0;

    //difernces to add for rand spawn 
    head_diff_x = l/2 , // 30
    head_diff_y = l*2 -5, //115
    beak_diff_x = l_beak/2, //15
    beak_diff_y =  2*l -5, // 115
    wing1_diff_x = l_wing,
    wing1_diff_y = l_wing-10,
    wing2_diff_x = l_wing-10,
    wing2_diff_y = l_wing-10; 


    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    // set_anugular_step = 0;


    // set_anugular_step = MIN_ANGLE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(MAX_ANGLE-MIN_ANGLE)));
    set_anugular_step = 1.1f;//de evitat 0.1 si 0.2 
    
    //avoid the straight ways 
    // if (   set_anugular_step < 0.2f && set_anugular_step > -0.2f ){
    //     printf("Modified!");
    //     set_anugular_step += 0.6f;
    // }
    // printf("%f" ,set_anugular_step);


    degrees = (float) set_anugular_step * (180.0f/3.141592653589793238463f); //convert to degress
    incline = (float)tan(degrees);                                           //find incline

    ACTIVE=1;
    SPAWN =1;

    MOVE_UP = 1;
    MOVE_DOWN = 0;


    Mesh* body = object2D::CreateBody("body", corner, l, glm::vec3(0.137255, 0.556863 , 0.137255), true);
    AddMeshToList(body);

    // Mesh* wing = object2D::CreateWing("wing", corner, l_wing, glm::vec3(0.196078, 0.8 , 0.6 ), true);
    Mesh* wing = object2D::CreateWing("wing", corner, l_wing, glm::vec3(1, 1 , 0 ), true);
    AddMeshToList(wing);

    Mesh* head = object2D::CreateHead("head", corner, glm::vec3(0.196078, 0.8 , 0.6), true);
    AddMeshToList(head);

    Mesh* beak = object2D::CreateBody("beak", corner, l_beak, glm::vec3(1, 1 , 0), true);
    AddMeshToList(beak);

}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!

    // modelBody = glm::mat3(1);
    // modelBody *= transform2D::Translate(540, 150);
    // if (comeback == true) {
    //     translateY += 5;
    // }
    // if (translateY > 350) {
    //     comeback = false;
    // }

    // if (translateY <= 0) {
    //     comeback = true;
    // }

    // modelMatrix *= transform2D::Translate(translateX, translateY);
    // if (comeback == false) {
    //     translateY -= 5;
    // }
    



    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    //////////////////////////////////////////////////////////////// IMPLEMENTATIONS
    //body
    if(SPAWN == 1) {

    modelBody = glm::mat3(1);
    modelBody *= transform2D::Translate(start_x, start_y);


    //head
    modelHead = glm::mat3(1);
    modelHead *= transform2D::Translate( start_x + head_diff_x ,start_y + head_diff_y); 

    //beak
    modelBeak = glm::mat3(1);
    modelBeak *= transform2D::Translate(start_x + beak_diff_x , start_y + beak_diff_y); 

    //wing1
    modelWing1 = glm::mat3(1);
    modelWing1 *= transform2D::Translate(start_x + wing1_diff_x ,start_y + wing1_diff_y); 

    //wing2
    modelWing2 = glm::mat3(1);
    modelWing2 *= transform2D::Translate(start_x-wing2_diff_x,start_y + wing2_diff_y); 
    }
    //////////////////////////////////////////////////////////////// SPAWN ROTATE ALL (angularStep [-1.2f , 1.2f])

    if (SPAWN == 1){
    modelBody *= transform2D::Translate(bx, by);
    modelBody *= transform2D::Rotate(set_anugular_step);
    modelBody *= transform2D::Translate(-bx, -by);

    modelHead *= transform2D::Translate(-hx, -hy);
    modelHead *= transform2D::Rotate(set_anugular_step);
    modelHead *= transform2D::Translate(hx,hy);

    modelBeak *= transform2D::Translate( kx, -ky);
    modelBeak *= transform2D::Rotate(set_anugular_step);
    modelBeak *= transform2D::Translate(-kx,ky);

    modelWing1 *= transform2D::Translate(-wx_1 ,0); 
    modelWing1 *= transform2D::Rotate(set_anugular_step);
    modelWing1 *= transform2D::Translate(  + wx_1 ,0);

    modelWing2 *= transform2D::Translate(wx_2 ,0); // 50 + 20
    modelWing2 *= transform2D::Rotate(set_anugular_step);
    modelWing2 *= transform2D::Translate( -wx_2 ,0);
    }
    SPAWN=0;
    //////////////////////////////////////////////////////////////// MOVEMENT
    if (ACTIVE == 1 && MOVE_UP == 1 && MOVE_DOWN == 0){
        modelBody *= transform2D::Translate(translateX, translateY);
        modelHead *= transform2D::Translate(translateX, translateY);
        modelBeak *= transform2D::Translate(translateX, translateY);
        modelWing1 *= transform2D::Translate(translateX, translateY);
        modelWing2 *= transform2D::Translate(translateX, translateY);
        translateX += 0.002f;
        if (incline < 0){ //right
            translateY -= incline * translateX ;
        }else //left
        {
            translateY += incline * translateX ;
        }
            
           
    }
    //   MOVE DOWN
      if (ACTIVE == 1 && MOVE_DOWN == 1 && MOVE_UP == 0){
        modelBody *= transform2D::Translate(translateX, translateY);
        modelHead *= transform2D::Translate(translateX, translateY);
        modelBeak *= transform2D::Translate(translateX, translateY);
        modelWing1 *= transform2D::Translate(translateX, translateY);
        modelWing2 *= transform2D::Translate(translateX, translateY);
        translateX += 0.002f;
        if (incline < 0){
            translateY -= incline * translateX ;
        }else
        {
            translateY += incline * translateX ;
        }

        
    }

    // cout << "translateX = " << translateX << "\n";
    // cout << "translateY = " << translateY << "\n";

    // cout << "keepX = " << keepX << "\n";
    // cout << "keepY = " << keepY << "\n";

    cout<<modelBody[2][0] << "\n"; //tx
    // cout<<modelBody[2][1] << "\n"; //ty



    //  if(modelBody[2][1] > 720 || translateY < 0 ){
    //         translateX=0;
    //         translateY=0;
    //         printf("Out of the zone!\n");

    //         if(set_anugular_step > 0){

    //         }

    //         set_anugular_step = set_anugular_step * -1;
    //         modelBody *= transform2D::Translate(bx, by);
    //         modelBody *= transform2D::Rotate(set_anugular_step);
    //         modelBody *= transform2D::Translate(-bx, -by);

    //         modelHead *= transform2D::Translate(-hx, -hy);
    //         modelHead *= transform2D::Rotate(set_anugular_step);
    //         modelHead *= transform2D::Translate(hx,hy);

    //         modelBeak *= transform2D::Translate( kx, -ky);
    //         modelBeak *= transform2D::Rotate(set_anugular_step);
    //         modelBeak *= transform2D::Translate(-kx,ky);

    //         modelWing1 *= transform2D::Translate(-wx_1 ,0); 
    //         modelWing1 *= transform2D::Rotate(set_anugular_step);
    //         modelWing1 *= transform2D::Translate(  + wx_1 ,0);

    //         modelWing2 *= transform2D::Translate(wx_2 ,0); // 50 + 20
    //         modelWing2 *= transform2D::Rotate(set_anugular_step);
    //         modelWing2 *= transform2D::Translate( -wx_2 ,0);
    //     }
        
        
        
        
        //LEFT
        if(modelBody[2][0] < 60 && CHECK_LEFT == 1 ){
            printf("Out left!\n");
            set_anugular_step = 2*PI  - 2* set_anugular_step ;
            degrees = (float) (set_anugular_step * (180.0f/3.141592653589793238463f));
            incline =(float)tan(degrees); 

            modelBody *= transform2D::Translate(bx, by);
            modelBody *= transform2D::Rotate(set_anugular_step);
            modelBody *= transform2D::Translate(-bx, -by);

            modelHead *= transform2D::Translate(-hx, -hy);
            modelHead *= transform2D::Rotate(set_anugular_step);
            modelHead *= transform2D::Translate(hx,hy);

            modelBeak *= transform2D::Translate( kx, -ky);
            modelBeak *= transform2D::Rotate(set_anugular_step);
            modelBeak *= transform2D::Translate(-kx,ky);

            modelWing1 *= transform2D::Translate(-wx_1 ,0); 
            modelWing1 *= transform2D::Rotate(set_anugular_step);
            modelWing1 *= transform2D::Translate(  + wx_1 ,0);

            modelWing2 *= transform2D::Translate(wx_2 ,0); // 50 + 20
            modelWing2 *= transform2D::Rotate(set_anugular_step);
            modelWing2 *= transform2D::Translate( -wx_2 ,0);

            translateX=0;
            translateY=0;

            MOVE_DOWN =0;
            MOVE_UP=1;

            SPAWN=0;

            CHECK_UP=1;
            CHECK_LEFT=0;
            CHECK_DOWN=1;
            CHECK_RIGHT=1;
        }


        //RIGHT
        if(modelBody[2][0] > 1200 && CHECK_RIGHT == 1 ){
            printf("Out right!\n");
            cout << set_anugular_step << "\n";

            set_anugular_step = PI  -  set_anugular_step ;
            degrees = (float) (set_anugular_step * (180.0f/3.141592653589793238463f));
            incline =(float)tan(degrees); 

            modelBody *= transform2D::Translate(bx, by);
            modelBody *= transform2D::Rotate(set_anugular_step);
            modelBody *= transform2D::Translate(-bx, -by);

            modelHead *= transform2D::Translate(-hx, -hy);
            modelHead *= transform2D::Rotate(set_anugular_step);
            modelHead *= transform2D::Translate(hx,hy);

            modelBeak *= transform2D::Translate( kx, -ky);
            modelBeak *= transform2D::Rotate(set_anugular_step);
            modelBeak *= transform2D::Translate(-kx,ky);

            modelWing1 *= transform2D::Translate(-wx_1 ,0); 
            modelWing1 *= transform2D::Rotate(set_anugular_step);
            modelWing1 *= transform2D::Translate(  + wx_1 ,0);

            modelWing2 *= transform2D::Translate(wx_2 ,0); // 50 + 20
            modelWing2 *= transform2D::Rotate(set_anugular_step);
            modelWing2 *= transform2D::Translate( -wx_2 ,0);

            translateX=0;
            translateY=0;

            MOVE_DOWN =1;
            MOVE_UP=0;

            SPAWN=0;

            CHECK_UP=1;
            CHECK_LEFT=1;
            CHECK_DOWN=1;
            CHECK_RIGHT=0;
        }


        //UP
        if( modelBody[2][1] > 720 && CHECK_UP == 1 ){
            printf("Out top!\n");
            cout << set_anugular_step << "\n";

            if(set_anugular_step > 3.12f){
                set_anugular_step = PI - set_anugular_step  ;
                degrees = (float) (set_anugular_step * (180.0f/3.141592653589793238463f));
                incline =(float)tan(degrees); 
            }else
            {
                set_anugular_step = PI - set_anugular_step*2 ;
                degrees = (float) (set_anugular_step * (180.0f/3.141592653589793238463f));
                incline =(float)tan(degrees); 
            }

            modelBody *= transform2D::Translate(bx, by);
            modelBody *= transform2D::Rotate(set_anugular_step);
            modelBody *= transform2D::Translate(-bx, -by);

            modelHead *= transform2D::Translate(-hx, -hy);
            modelHead *= transform2D::Rotate(set_anugular_step);
            modelHead *= transform2D::Translate(hx,hy);

            modelBeak *= transform2D::Translate( kx, -ky);
            modelBeak *= transform2D::Rotate(set_anugular_step);
            modelBeak *= transform2D::Translate(-kx,ky);

            modelWing1 *= transform2D::Translate(-wx_1 ,0); 
            modelWing1 *= transform2D::Rotate(set_anugular_step);
            modelWing1 *= transform2D::Translate(  + wx_1 ,0);

            modelWing2 *= transform2D::Translate(wx_2 ,0); // 50 + 20
            modelWing2 *= transform2D::Rotate(set_anugular_step);
            modelWing2 *= transform2D::Translate( -wx_2 ,0);

            translateY =0;
            translateX =0;

            cout << "After translate:\n";
            cout << "translateX = " << translateX << "\n";
            cout << "translateY = " << translateY << "\n";

            MOVE_DOWN =1;
            MOVE_UP=0;
            SPAWN=0;
            CHECK_UP=0;
            CHECK_DOWN=1;
            CHECK_LEFT=1;
            CHECK_RIGHT=1;
        }


    //////////////////////////////////////////////////////////////// WING ANIMATIONS

    // modelWing2 *= transform2D::Translate(50 ,0); 
    // modelWing2 *= transform2D::Translate( wx_2, -wy_2);
    // modelWing2 *= transform2D::Rotate(angularStep);
    // modelWing2 *= transform2D::Translate(wx_1,wy_1);

    //////////////////////////////////////////////////////////////// RENDER MESHES
    RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelWing1); 
    RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelWing2);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelHead);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelBody);  
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelBeak);  
     

    angularStep += deltaTimeSeconds;
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

 //   RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix); 

//    modelMatrix = glm::mat3(1);

//    if (scaleX <= 1.5f && pulse == true) {
//       scaleX += deltaTimeSeconds;
//        scaleY += deltaTimeSeconds;
//        if (scaleX >= 1.5f) pulse = false;
//    }
//    else {
//        scaleX -= deltaTimeSeconds;
//        scaleY -= deltaTimeSeconds;
//        if (scaleX <= .5f) pulse = true;
//    }
   
//    modelMatrix *= transform2D::Translate(650, 250);
//    modelMatrix *= transform2D::Translate(cx, cy);
//    modelMatrix *= transform2D::Scale(scaleX, scaleY);
//    modelMatrix *= transform2D::Translate(-cx, -cy);




    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

//    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
