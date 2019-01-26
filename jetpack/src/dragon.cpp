#include "player.h"
#include "main.h"

Dragon::Dragon(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0.0f;
    this->lives = 3;
    bounding_box_t box_cover;
    
    box_cover.x = x;
    box_cover.y = y;
    box_cover.width = 2*40.0f;
    box_cover.height = 2*40.0f;
    box_cover.angle = this->rotation*M_PI/180.0f;

    this->box = box_cover;
    
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data1[] = {
        -45.0f,-45.0f, 0.0f,
        -45.0f, 45.0f, 0.0f, // triangle 3 : end
        45.0f,-45.0f, 0.0f,
        
        45.0f,-45.0f, 0.0f,
        -45.0f,45.0f, 0.0f,
        45.0f, 45.0f, 0.0f, // triangle 2 : end
        };

    static const GLfloat vertex_buffer_data2[] = {
        -35.0f,-35.0f, 0.0f,
        -35.0f, 35.0f, 0.0f, // triangle 3 : end
        35.0f,-35.0f, 0.0f,
        
        35.0f,-35.0f, 0.0f,
        -35.0f,35.0f, 0.0f,
        35.0f, 35.0f, 0.0f, // triangle 2 : end
        };
        
    // int n = 4;
    // float deg = (360.0/n)*(M_PI/180);
	// GLfloat g_vertex_buffer_data[9*n];
	// for (int i = 0; i < 9*n; i+=3)
	//  {
	//  	if(i%9==0)
	//  	{
	// 	 	g_vertex_buffer_data[i]=x;
	// 	 	g_vertex_buffer_data[i+1]=y;
	// 	 	g_vertex_buffer_data[i+2]=0.0f;
	// 		continue;
	// 	}
	// 	 	g_vertex_buffer_data[i]=x+(this->box.width*(1/1.414)*(cos(((i/9)+(i%9)/3.0)*deg)));
	// 	 	g_vertex_buffer_data[i+1]=y+(this->box.width*(1/1.414)*(sin(((i/9)+(i%9)/3.0)*deg)));
	// 	 	g_vertex_buffer_data[i+2]=0.0f;
	//  } 

    this->object1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data1, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data2, COLOR_BACKGROUND, GL_FILL);

}

void Dragon::draw(glm::mat4 VP) {
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

void Dragon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Dragon::tick() {
    //this->rotation += speed;
   if(this->position.x >= 600)
        this->position.x -= 3;
   
}

