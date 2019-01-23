#include "player.h"
#include "main.h"

Floor::Floor(color_t color) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        -4500.0f,-4500.0f, 0.0f,
        -4500.0f, -45.0f, 0.0f, 
        4500.0f,-4500.0f, 0.0f, // triangle 1 : end
       
        -4500.0f,-45.0f, 0.0f,
        4500.0f,-45.0f, 0.0f,
        4500.0f, -4500.0f, 0.0f, // triangle 2 : end
        };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
   // printf("test\n");    
}
void Floor::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

