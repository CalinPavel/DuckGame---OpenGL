#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateBody(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateHead(const std::string &name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill = false);
    Mesh* CreateWing(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);



}
