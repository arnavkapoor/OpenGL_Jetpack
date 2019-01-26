#include "player.h"
#include "main.h"

Semicircle::Semicircle(float x, float y, float radius,color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    
    bounding_box_t box_cover;
    
    box_cover.x = x;
    box_cover.y = y + radius/2;
    box_cover.width = 2*radius;
    box_cover.height = radius;
    box_cover.angle = this->rotation*M_PI/180.0f;

    this->box = box_cover;
    
    int n = 300;
    float deg = (360.0/n)*(M_PI/180);
	
    GLfloat g_vertex_buffer_data1[(9*n)/2];
    GLfloat g_vertex_buffer_data2[(9*n)/2];

	for (int i = 0; i < (9*n)/2; i+=3)
	 {
	 	if(i%9==0)
	 	{
		 	g_vertex_buffer_data1[i]=0.0f;
		 	g_vertex_buffer_data1[i+1]=0.0f;
		 	g_vertex_buffer_data1[i+2]=0.0f;
            
            g_vertex_buffer_data2[i]=0.0f;
		 	g_vertex_buffer_data2[i+1]=0.0f;
		 	g_vertex_buffer_data2[i+2]=0.0f;		
            continue;
		}
		 	g_vertex_buffer_data1[i]=0.0f+(radius*(cos(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data1[i+1]=0.0f+(radius*(sin(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data1[i+2]=0.0f;
            
            g_vertex_buffer_data2[i]=0.0f+((radius-5.0)*(cos(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data2[i+1]=0.0f+((radius-5.0)*(sin(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data2[i+2]=0.0f;
	 } 

    this->object1 = create3DObject(GL_TRIANGLES,  (n/2)*3, g_vertex_buffer_data1, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES,  (n/2)*3, g_vertex_buffer_data2, COLOR_BACKGROUND, GL_FILL);
}

void Semicircle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}

void Semicircle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Semicircle::tick() {
    this->position.x -= 3;
    this->box.x = this->position.x;
}

