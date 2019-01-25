#include "player.h"
#include "main.h"

Bonus::Bonus(float x, float y,float radius,int type,color_t color) {
    
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    
    bounding_box_t box_cover;
    
    this->speedx = -4.0-type;
    this->speedy = 12.0+type;
    this->accy = -0.2;
    this->score = type;

    box_cover.x = x;
    box_cover.y = y;
    box_cover.width = radius;
    box_cover.height = radius;
    box_cover.angle = this->rotation;

    this->box = box_cover;

    int n = 4+type;
    float deg = (360.0/n)*(M_PI/180);
	GLfloat g_vertex_buffer_data[9*n];

	for (int i = 0; i < 9*n; i+=3)
	 {
	 	if(i%9==0)
	 	{
		 	g_vertex_buffer_data[i]=0.0f;
		 	g_vertex_buffer_data[i+1]=0.0f;
		 	g_vertex_buffer_data[i+2]=0.0f;
			continue;
		}
		 	g_vertex_buffer_data[i]=0.0f+(radius*(cos(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data[i+1]=0.0f+(radius*(sin(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data[i+2]=0.0f;
	 } 

    this->object = create3DObject(GL_TRIANGLES,  n*3, g_vertex_buffer_data, color, GL_FILL);
}

void Bonus::draw(glm::mat4 VP) {
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

void Bonus::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Bonus::tick() {
    if(this->position.x <= 750)
    {
        this->position.x += speedx;
        this->position.y += speedy;
        this->speedy += accy;
    }
    
    this->position.x -= 3;
    
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}


