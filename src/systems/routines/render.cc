#include "render.hh"
#include "ecs/entity.hh"
#include "components/mesh_2d.hh"
#include "components/rigid_transform_2d.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

using namespace newt;
using namespace ecs;

bool systems::routines::render(const database& db, GLFWwindow* win, GLuint program_id, GLuint vertex_id) {
    glUseProgram(program_id);
    for (auto& mesh : db.mesh_2d_set) {
        auto& entity = db.entity_set.at(mesh.entity_index);
        if (entity.has_component<components::rigid_transform_2d>()) {
            auto& rtf = entity.get_component<components::rigid_transform_2d>(db);

            auto& triangles = mesh.resource->triangles();
            
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
            glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(newt::resources::mesh_2d::triangle), triangles.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            
            glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);

            glDisableVertexAttribArray(0);
            
            // glEnableVertexAttribArray(1);
            // glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
            // glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(newt::resources::mesh_2d::triangle), triangles.data(), GL_STATIC_DRAW);
            // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            
            // glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);

            // glDisableVertexAttribArray(1);

        }
    }

    glfwSwapBuffers(win);
    
    glfwPollEvents();

    if (glfwWindowShouldClose(win)) {
        return false;
    }
    return true;
}