#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <cstdlib>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;
using namespace gfxc;

#define PI 3.1415f
#define MAX_ANGLE RADIANS(70)
#define MIN_ANGLE RADIANS(35)

#define MAX_ANGLE_1 RADIANS(145)
#define MIN_ANGLE_1 RADIANS(120)

#define MAX_LEN_SPAWN 1100
#define MIN_LEN_SPAWN 100

tema1::tema1()
{
}

tema1::~tema1()
{
}

void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 60;
    // body dimensions
    float l = 60;
    float l_beak = 30;
    float l_wing = 50;

    cx = corner.x; 
    cy = corner.y;

    // body center
    bx = l / 2;
    by = 2 * l / 3;

    // head center
    hx = 0;
    hy = 4 * l / 3;

    // beak center
    kx = l_beak / 2;
    ky = 4 * l / 3;

    // wing1 center
    wx_1 = 20;
    wy_1 = 0;

    // wing2 center
    wx_2 = 70;
    wy_2 = 0;

    // set Y for spawn position 
    start_x = 300, start_y = 100;

    keepX = start_x + l / 2;
    keepY = start_y + 2 * l / 3;


    // difernces to add for  spawn
    head_diff_x = 2 * l,     
    head_diff_y = 0,     
    beak_diff_x = 2 * l, 
    beak_diff_y = 0,     

    wing1_diff_x = 2 * l / 3,  
    wing1_diff_y = l / 2 - 10,

    wing2_diff_x = l * 2 / 3,
    wing2_diff_y = -l / 2 + 10;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    t = 0;
    // start wing angles
    wing_angle1 = 0.2f;
    wing_angle2 = -0.2f;
    sense_wing1 = 0;
    sense_wing2 = 0;

    lives = 3;
    bullets = 3;
    score = 0;

    ACTIVE = 1;
    SPAWN = 1;

    MOVE_UP = 1;
    MOVE_DOWN = 0;

    MOVE_ORDER = 1;
    const glm::vec3 bodyColor = NormalizedRGB(56, 161, 74);
    Mesh *body = object2D::CreateBody("body", corner, l, glm::vec3(0.137255, 0.556863, 0.137255), true);
    AddMeshToList(body);

    const glm::vec3 wingColor = NormalizedRGB(181, 203, 186);
    Mesh *wing1 = object2D::CreateWing1("wing1", glm::vec3(wing1_diff_x, 0, 0), l_wing, wingColor, true);
    AddMeshToList(wing1);

    Mesh *wing2 = object2D::CreateWing2("wing2", glm::vec3(wing2_diff_x, 0, 0), l_wing,wingColor, true);
    AddMeshToList(wing2);

    const glm::vec3 headColor = NormalizedRGB(17, 239, 180);
    Mesh *head = object2D::CreateHead("head", corner + glm::vec3(head_diff_x, head_diff_y, 0), headColor , true);
    AddMeshToList(head);

    Mesh *beak = object2D::CreateBeak("beak", corner + glm::vec3(beak_diff_x, beak_diff_y, 0), l_beak, glm::vec3(1, 1, 0), true);
    AddMeshToList(beak);

    Mesh *health = object2D::CreateHealth("health", corner, glm::vec3(1, 0, 0), true);
    AddMeshToList(health);

    Mesh *health1 = object2D::CreateHealth("health1", corner, glm::vec3(1, 0, 0), true);
    AddMeshToList(health1);

    Mesh *health2 = object2D::CreateHealth("health2", corner, glm::vec3(1, 0, 0), true);
    AddMeshToList(health2);

    const glm::vec3 bulletColor = NormalizedRGB(185, 117, 15);
    Mesh *bullet1 = object2D::CreateBullet("bullet1", corner, 20, bulletColor, true);
    AddMeshToList(bullet1);

    Mesh *bullet2 = object2D::CreateBullet("bullet2", corner, 20, bulletColor, true);
    AddMeshToList(bullet2);

    Mesh *bullet3 = object2D::CreateBullet("bullet3", corner, 20, bulletColor, true);
    AddMeshToList(bullet3);

    const glm::vec3 scoreColor = NormalizedRGB(11, 147, 196);
    Mesh *score = object2D::CreateScore("score", corner, 40, scoreColor, false);
    AddMeshToList(score);

    Mesh *scoreBlock = object2D::CreateScoreBlock("scoreBlock", corner, 40, scoreColor, true);
    AddMeshToList(scoreBlock);

    const glm::vec3 grassColor = NormalizedRGB(27, 92, 7);
    Mesh *grass = object2D::CreateGrass("grass", corner, 40, grassColor, true);
    AddMeshToList(grass);
}

void tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema1::Update(float deltaTimeSeconds)
{
    //render grass
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelGrass);

    if (SPAWN == 1)
    {
        // spawn variables for differt start positions
        int Num = rand() % (MAX_LEN_SPAWN - MIN_LEN_SPAWN + 1) + MIN_LEN_SPAWN;
        start_x = (float)Num;

        spawn_counter++;

        if (spawn_counter % 2 == 0)
        {
            set_anugular_step = MIN_ANGLE + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_ANGLE - MIN_ANGLE)));
            degrees = (float)set_anugular_step * (180.0f / 3.141592653589793238463f); // convert to degress
            incline = (float)tan(degrees);
        }
        else
        {
            set_anugular_step = MIN_ANGLE_1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_ANGLE_1 - MIN_ANGLE_1)));
            degrees = (float)set_anugular_step * (180.0f / 3.141592653589793238463f); // convert to degress
            incline = (float)tan(degrees);
        }

        modelBody = glm::mat3(1);
        modelBody *= transform2D::Translate(start_x, start_y);

        // health
        modelHealth = glm::mat3(1);
        modelHealth *= transform2D::Translate(940, 670);

        modelHealth1 = glm::mat3(1);
        modelHealth1 *= transform2D::Translate(1000, 670);

        modelHealth2 = glm::mat3(1);
        modelHealth2 *= transform2D::Translate(1060, 670);

        // bullets
        modelBullet1 = glm::mat3(1);
        modelBullet1 *= transform2D::Translate(0, -20);
        modelBullet1 *= transform2D::Translate(1100, 670);

        modelBullet2 = glm::mat3(1);
        modelBullet2 *= transform2D::Translate(0, -20);

        modelBullet2 *= transform2D::Translate(1130, 670);

        modelBullet3 = glm::mat3(1);
        modelBullet3 *= transform2D::Translate(0, -20);
        modelBullet3 *= transform2D::Translate(1160, 670);

        // score
        modelScore = glm::mat3(1);
        modelScore *= transform2D::Translate(920, 590);

        modelScoreBlock1 = glm::mat3(1);
        modelScoreBlock1 *= transform2D::Translate(920, 590);

        modelScoreBlock2 = glm::mat3(1);
        modelScoreBlock2 *= transform2D::Translate(960, 590);

        modelScoreBlock3 = glm::mat3(1);
        modelScoreBlock3 *= transform2D::Translate(1000, 590);

        modelScoreBlock4 = glm::mat3(1);
        modelScoreBlock4 *= transform2D::Translate(1040, 590);

        modelScoreBlock5 = glm::mat3(1);
        modelScoreBlock5 *= transform2D::Translate(1080, 590);

        //grass
        modelGrass = glm::mat3(1);

    }
    // time variable for time actions
    t += deltaTimeSeconds;
    // time for the duck to escape
    if (t > 7)
    {
        t = 0;
        ESCAPED = 1;
        TO_ESCAPE = 1;
    }
    
    if (SPAWN == 1)
    {
        modelBody *= transform2D::Translate(bx, by);
        modelBody *= transform2D::Rotate(set_anugular_step);
        modelBody *= transform2D::Translate(-bx, -by);
    }
    SPAWN = 0;
    // movement conditions
    if (ACTIVE == 1 && MOVE_UP == 1 && MOVE_DOWN == 0)
    {
        // update duck speed according to the level dificulty
        translateX = 1.8f * level_dif;
        modelWing1 *= transform2D::Translate(translateX, translateY);
        modelWing2 *= transform2D::Translate(translateX, translateY);
        modelBody *= transform2D::Translate(translateX, translateY);
        modelHead *= transform2D::Translate(translateX, translateY);
        modelBeak *= transform2D::Translate(translateX, translateY);
    }
    // left side refection
    if (modelBody[2][0] < 60 && CHECK_LEFT == 1 && ESCAPED == 0)
    {
        while (set_anugular_step > RADIANS(360))
        {
            set_anugular_step -= RADIANS(360);
        }

        if (set_anugular_step < RADIANS(180))
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(-RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }

            set_anugular_step += PI / 2 + PI;
        }
        else
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(+RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }

            set_anugular_step += PI / 2;
        }

        degrees = (float)(set_anugular_step * (180.0f / 3.141592653589793238463f));
        incline = (float)tan(degrees);

        SPAWN = 0;

        CHECK_UP = 1;
        CHECK_LEFT = 0;
        CHECK_DOWN = 1;
        CHECK_RIGHT = 1;

        CORNER_LEFT_DOWN = 1;

    }

    // top side reflection
    if (modelBody[2][1] > 720 && CHECK_UP == 1 && ESCAPED == 0)
    {

        while (set_anugular_step > RADIANS(360))
        {
            set_anugular_step -= RADIANS(360);
        }

        if (set_anugular_step < RADIANS(90))
        {

            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(RADIANS(360 - DEGREES(2*set_anugular_step)));
                modelBody *= transform2D::Translate(-bx, -by);
            }
            set_anugular_step += 2*PI - 2*set_anugular_step;
        }
        else
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }

            set_anugular_step += PI / 2;
        }

        degrees = (float)(set_anugular_step * (180.0f / 3.141592653589793238463f));
        incline = (float)tan(degrees);

        SPAWN = 0;
        CHECK_UP = 0;
        CHECK_DOWN = 1;
        CHECK_LEFT = 1;
        CHECK_RIGHT = 1;

        CORNER_LEFT_DOWN = 1;

    }

    // right side reflection
    if (modelBody[2][0] > 1200 && CHECK_RIGHT == 1 && ESCAPED == 0)
    {

        while (set_anugular_step > RADIANS(360))
        {
            set_anugular_step -= RADIANS(360);
        }
        if (set_anugular_step < RADIANS(90))
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }
            set_anugular_step += PI / 2;
        }
        else
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(+RADIANS(270));
                modelBody *= transform2D::Translate(-bx, -by);
            }
            set_anugular_step += PI / 2 + PI;
        }

        degrees = (float)(set_anugular_step * (180.0f / 3.141592653589793238463f));
        incline = (float)tan(degrees);

        SPAWN = 0;
        CHECK_UP = 1;
        CHECK_LEFT = 1;
        CHECK_DOWN = 1;
        CHECK_RIGHT = 0;

        CORNER_LEFT_DOWN = 1;

    }

    // down side reflection
    if (modelBody[2][1] < 50 && CHECK_DOWN == 1 && ESCAPED == 0)
    {

        while (set_anugular_step > RADIANS(360))
        {
            set_anugular_step -= RADIANS(360);
        }

        if (set_anugular_step > RADIANS(270))
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }

            set_anugular_step += PI / 2;
        }
        else
        {
            {
                modelBody *= transform2D::Translate(bx, by);
                modelBody *= transform2D::Rotate(-RADIANS(90));
                modelBody *= transform2D::Translate(-bx, -by);
            }

            set_anugular_step += PI / 2 + PI;
        }

        degrees = (float)(set_anugular_step * (180.0f / 3.141592653589793238463f));
        incline = (float)tan(degrees);

        SPAWN = 0;

        CHECK_UP = 1;
        CHECK_LEFT = 1;
        CHECK_DOWN = 0;
        CHECK_RIGHT = 1;
    }
    // render head + body + beak
    if (ACTIVE == 1)
    {
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelBody);
    }

    // render wings + animations
    if (ACTIVE == 1)
    {
        if (wing_angle1 < -0.55)
        {
            sense_wing1 = 0;
        }
        if (wing_angle1 > 0.2f)
        {
            sense_wing1 = 1;
        }
        if (sense_wing1 == 0)
        {
            wing_angle1 += 0.01f;

            RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));
        }
        else if (sense_wing1 == 1)
        {
            wing_angle1 -= 0.01f;
            RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));
        }
        if (wing_angle2 < -0.2f)
        {
            sense_wing2 = 0;
        }
        if (wing_angle2 > 0.55f)
        {
            sense_wing2 = 1;
        }
        if (sense_wing2 == 0)
        {
            wing_angle2 += 0.01f;

            RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        }
        else if (sense_wing2 == 1)
        {
            wing_angle2 -= 0.01f;
            RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        }
    }

    // render  HEALTH + BULLETS + SCORE
    switch (lives)
    {
    case 3:
        RenderMesh2D(meshes["health"], shaders["VertexColor"], modelHealth);
        RenderMesh2D(meshes["health1"], shaders["VertexColor"], modelHealth1);
        RenderMesh2D(meshes["health2"], shaders["VertexColor"], modelHealth2);
        break;
    case 2:
        RenderMesh2D(meshes["health"], shaders["VertexColor"], modelHealth);
        RenderMesh2D(meshes["health1"], shaders["VertexColor"], modelHealth1);
        break;
    case 1:
        RenderMesh2D(meshes["health"], shaders["VertexColor"], modelHealth);
        break;
    }

    switch (bullets)
    {
    case 3:
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelBullet1);
        RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelBullet2);
        RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelBullet3);
        break;
    case 2:
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelBullet1);
        RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelBullet2);
        break;
    case 1:
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelBullet1);
        break;
    }

    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelScore);

    switch (score)
    {
    case 1:
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock1);
        break;
    case 2:
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock1);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock2);
        break;
    case 3:
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock1);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock2);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock3);
        break;
    case 4:
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock1);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock2);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock3);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock4);
        break;
    case 5:
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock1);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock2);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock3);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock4);
        RenderMesh2D(meshes["scoreBlock"], shaders["VertexColor"], modelScoreBlock5);
        break;
    }

    // duck killed animations
    if (ACTIVE == 0 && KILLED == 0 && TO_KILL == 1)
    {
        keepX = modelBody[2][0];
        keepY = modelBody[2][1];

        modelBody = glm::mat3(1);
        modelBody *= transform2D::Translate(keepX, keepY);

        modelBody *= transform2D::Translate(bx, by);
        modelBody *= transform2D::Rotate(RADIANS(270));
        modelBody *= transform2D::Translate(-bx, -by);
        KILLED = 1;
    }

    if (KILLED == 1)
    {
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));

        // move down after shoot
        if (modelBody[2][1] > 60)
        {
            translateX = 1.8f;
            modelWing1 *= transform2D::Translate(translateX, translateY);
            modelWing2 *= transform2D::Translate(translateX, translateY);
            modelBody *= transform2D::Translate(translateX, translateY);
            modelHead *= transform2D::Translate(translateX, translateY);
            modelBeak *= transform2D::Translate(translateX, translateY);
            t = 0;
        }
        else
        {
            bullets = 3;
            ACTIVE = 1;
            SPAWN = 1;
            KILLED = 0;
            ESCAPED = 0;
            TO_ESCAPE =0;
            TO_KILL = 0;

            CHECK_LEFT = 1;
            CHECK_UP = 1;
            CHECK_RIGHT = 1;
            CHECK_DOWN = 1;

            t=0;
        }
    }
    // duck escape 
    if (TO_ESCAPE == 1 && KILLED != 1)
    {
        keepX = modelBody[2][0];
        keepY = modelBody[2][1];

        modelBody = glm::mat3(1);
        modelBody *= transform2D::Translate(keepX, keepY);
        modelBody *= transform2D::Translate(bx, by);
        modelBody *= transform2D::Rotate(RADIANS(90));
        modelBody *= transform2D::Translate(-bx, -by);
        ESCAPED = 1;
        TO_ESCAPE = 0;
        ACTIVE = 0;
    }
    // check that duck is not killed during escaping
    if (ESCAPED == 1 && KILLED != 1)
    {

        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelBody);
        RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));
    // wing animation
    {
        if (wing_angle1 < -0.55)
        {
            sense_wing1 = 0;
        }
        if (wing_angle1 > 0.2f)
        {
            sense_wing1 = 1;
        }
        if (sense_wing1 == 0)
        {
            wing_angle1 += 0.01f;

            RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));
        }
        else if (sense_wing1 == 1)
        {
            wing_angle1 -= 0.01f;
            RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle1));
        }
        if (wing_angle2 < -0.2f)
        {
            sense_wing2 = 0;
        }
        if (wing_angle2 > 0.55f)
        {
            sense_wing2 = 1;
        }
        if (sense_wing2 == 0)
        {
            wing_angle2 += 0.01f;

            RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        }
        else if (sense_wing2 == 1)
        {
            wing_angle2 -= 0.01f;
            RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelBody * transform2D::Rotate(wing_angle2));
        }
    }
        // move to the top animation
        if (modelBody[2][1] < 850)
        {
            translateX = 1.8f * level_dif;
            modelWing1 *= transform2D::Translate(translateX, translateY);
            modelWing2 *= transform2D::Translate(translateX, translateY);
            modelBody *= transform2D::Translate(translateX, translateY);
            modelHead *= transform2D::Translate(translateX, translateY);
            modelBeak *= transform2D::Translate(translateX, translateY);
            t=0;
        }
        else
        {

            lives--;
            ACTIVE = 1;
            SPAWN = 1;
            KILLED = 0;
            ESCAPED = 0;
            bullets = 3;
            TO_ESCAPE = 0;

            CHECK_LEFT = 1;
            CHECK_UP = 1;
            CHECK_RIGHT = 1;
            CHECK_DOWN = 1;

            t=0;
        }
    }
    angularStep += deltaTimeSeconds;
    // increase level
    if (LEVEL_UP == 1)
    {
        t_text += deltaTimeSeconds;
        if (t_text > 3)
        {
            t_text = 0;
            LEVEL_UP = 0;
        }
    }
    // check for game over
    if (lives == 0)
    {
        GAME_OVER = 1;
        bullets =0;
        ACTIVE = 0;
        SPAWN = 0;
        t = 0;
    }
}
// level up text 
void tema1::DrawHUD()
{

    Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 7);
    const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
    RenderText("LEVEL UP!", 560, 340, 5, kTextColor);
}
// game over text
void tema1::DrawHUD_Lost()
{
    Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 7);

    const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);

    RenderText("<<GAME OVER>>", 510, 340, 5, glm::vec3(1, 0, 0));
}
//text appearing at the right time
void tema1::FrameEnd()
{
    if (LEVEL_UP == 1)
    {
        DrawHUD();
        DrawCoordinateSystem();
    }

    if (GAME_OVER == 1)
    {
        DrawHUD_Lost();
        DrawCoordinateSystem();
    }
}

void tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void tema1::OnKeyPress(int key, int mods)
{
}

void tema1::OnKeyRelease(int key, int mods)
{
}

void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // check the aim box
    if (((720 - (int)modelBody[2][1] - 150) < (int)mouseY && ((720 - (int)modelBody[2][1] + 150) > (int)mouseY)) &&
        (((int)modelBody[2][0] - 150) < (int)mouseX && (((int)modelBody[2][0] + 150) > (int)mouseX)) && bullets > 0)
    {
        ACTIVE = 0;
        TO_KILL = 1;
        score++;
        // check score for level up
        if (score > 5)
        {
            LEVEL_UP = 1;
            score = 0;
            level++;
            level_dif += 0.5;
        }
    }
    // decrease bullets after shooting
    bullets--;
    // animate duck escape after missing all bullets
    if (bullets == 0)
    {
        TO_ESCAPE = 1;
    }
    // check for game over conditions
    if(GAME_OVER == 1){
        bullets=0;
    }
}

void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void tema1::OnWindowResize(int width, int height)
{
}
// text render + load
void tema1::TextRenderer(const std::string &selfDir, GLuint width, GLuint height)
{
    Shader *shader = new Shader("ShaderText");
    shader->AddShader(PATH_JOIN(selfDir, RESOURCE_PATH::SHADERS, "Text.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(selfDir, RESOURCE_PATH::SHADERS, "Text.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    this->m_textShader = shader;

    int loc_projection_matrix = glGetUniformLocation(shader->program, "projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f)));

    int loc_text = glGetUniformLocation(shader->program, "text");
    glUniform1i(loc_text, 0);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void tema1::Load(std::string font, GLuint fontSize)
{
    this->Characters.clear();

    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x};

        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void tema1::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    if (this->m_textShader)
    {
        glUseProgram(this->m_textShader->program);
        CheckOpenGLError();
    }

    int loc_text_color = glGetUniformLocation(this->m_textShader->program, "textColor");
    glUniform3f(loc_text_color, color.r, color.g, color.b);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    for (auto c = text.cbegin(); c != text.cend(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 0.0},
            {xpos, ypos, 0.0, 0.0},

            {xpos, ypos + h, 0.0, 1.0},
            {xpos + w, ypos + h, 1.0, 1.0},
            {xpos + w, ypos, 1.0, 0.0}};

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
