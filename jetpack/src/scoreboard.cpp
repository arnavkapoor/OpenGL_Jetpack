#include "player.h"
#include "main.h"

Score::Score(float x, float y,float rotation) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    
     static const GLfloat g_vertex_buffer_data[] = {
        10.0f, 2.0f, 0.0f,
        -10.0f, 2.0f, 0.0f, // triangle 3 : end
        -10.0f,-2.0f, 0.0f,

        -10.0f,-2.0f, 0.0f,
        10.0f,-2.0f, 0.0f,
        10.0f, 2.0f, 0.0f, // triangle 2 : end
        };

    this->object = create3DObject(GL_TRIANGLES,  2*3, g_vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Score::draw(glm::mat4 VP) {
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

void Score::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}


