#include "player.h"
#include "main.h"

Beams::Beams(float x, float y,float rotation,color_t color,int movement) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    this->movement = movement;

    bounding_box_t box_cover;
    
    box_cover.x = x;
    box_cover.y = y;
    box_cover.width = 25.0f;
    box_cover.height = 420.0f;
    box_cover.angle = this->rotation*M_PI/180.0f;

    this->box = box_cover;

    static const GLfloat vertex_buffer_data[] = {
        -10.0f,-200.0f, 0.0f,
        -10.0f, 200.0f, 0.0f, // triangle 3 : end
        10.0f,-200.0f, 0.0f,
        10.0f,-200.0f, 0.0f,
        -10.0f,200.0f, 0.0f,
        10.0f, 200.0f, 0.0f, // triangle 2 : end
        };
    this->object1 = create3DObject(GL_TRIANGLES,  2*3, vertex_buffer_data, color, GL_FILL);
    
    int n = 100;
    int radius = 20.0f;
    float deg = (360.0/n)*(M_PI/180);
	
    GLfloat g_vertex_buffer_data1[9*n];
	for (int i = 0; i < 9*n; i+=3)
	 {
	 	if(i%9==0)
	 	{
		 	g_vertex_buffer_data1[i]=0.0f;
		 	g_vertex_buffer_data1[i+1]=200.0f;
		 	g_vertex_buffer_data1[i+2]=0.0f;
			continue;
		}
		 	g_vertex_buffer_data1[i]=0.0f+(radius*(cos(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data1[i+1]=200.0f+(radius*(sin(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data1[i+2]=0.0f;
	 } 
    
    GLfloat g_vertex_buffer_data2[9*n];
	for (int i = 0; i < 9*n; i+=3)
	 {
	 	if(i%9==0)
	 	{
		 	g_vertex_buffer_data2[i]=0.0f;
		 	g_vertex_buffer_data2[i+1]=-200.0f;
		 	g_vertex_buffer_data2[i+2]=0.0f;
			continue;
		}
		 	g_vertex_buffer_data2[i]=0.0f+(radius*(cos(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data2[i+1]=-200.0f+(radius*(sin(((i/9)+(i%9)/3.0)*deg)));
		 	g_vertex_buffer_data2[i+2]=0.0f;
	 }

    this->object2 = create3DObject(GL_TRIANGLES,  n*3, g_vertex_buffer_data1, COLOR_BLACK, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES,  n*3, g_vertex_buffer_data2, COLOR_BLACK, GL_FILL);

}

void Beams::draw(glm::mat4 VP) {
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
    draw3DObject(this->object3);

}

void Beams::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Beams::tick(int type) {
    
    if(type == DOUBLE_BOTTOM)
    {
        if(this->position.y >= 550 or this->position.y <= 0) 
            this->movement *= -1;
    }

    if(type == DOUBLE_TOP)
    {
        if(this->position.y >= 750 or this->position.y <= 200)
            this->movement *= -1;
    }


    this->position.x -= 3;
    this->position.y += this->movement*1;


    this->box.x = this->position.x;
    this->box.y = this->position.y;
}

