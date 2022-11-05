#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateBody(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length + length , 0), color),
    };

    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        body->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    body->InitFromData(vertices, indices);
    return body;
}

Mesh* object2D::CreateHead(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;

    Mesh* head = new Mesh(name);
    std::vector<unsigned int> indices;

    float d = 2 * 3.14159f / 100;

    for (unsigned int i = 0; i <= 100; i++)
	{
			vertices.emplace_back(corner + glm::vec3(cos(d * i) * 25, sin(d * i) * 25, 0), color);
			indices.push_back(i);
	}

    head->SetDrawMode(GL_TRIANGLE_FAN);
    head->InitFromData(vertices, indices);
    return head;
}

Mesh* object2D::CreateWing(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length/2 , length , 0), color),
    };

    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        body->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    body->InitFromData(vertices, indices);
    return body;
}