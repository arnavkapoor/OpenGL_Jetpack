#include "player.h"
#include "main.h"

Boomerang::Boomerang(float x, float y,color_t color) {
    
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    bounding_box_t box_cover;
    
    box_cover.x = x;
    box_cover.y = y;
    box_cover.width = 50;
    box_cover.height = 50;
    box_cover.angle = this->rotation;

    this->box = box_cover;

      static const GLfloat vertex_buffer_data[] = {
        -25.0f, 0.0f, 0.0f,
        25.0f, 25.0f, 0.0f, // triangle 3 : end
        0.0f, 0.0f, 0.0f,
        0.0f,0.0f, 0.0f,
        -25.0f, 0.0f, 0.0f,
        25.0f, -25.0f, 0.0f, // triangle 2 : end
        };
    
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP) {
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

void Boomerang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boomerang::tick() {

    if(this->position.x <= 750.0f)
    {
        this->position.y -= 4;
        this->position.x = (this->position.y - 350)*(this->position.y-350)*(1/4.0)*(1/40.83)-350;
    }
    else
        this->position.x -= 3;
    
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}

