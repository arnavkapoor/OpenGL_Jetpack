#include "main.h"

#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bounding_box_t box;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dir);
    double speedx;
    double speedy;
    double accy;

private:
    VAO *object;
};

#endif // BALL_H


#ifndef FLOOR_H
#define FLOOR_H

class Floor {
public:
    Floor() {}
    Floor(color_t color);
    float rotation;
    glm::vec3 position;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // FLOOR_H

#ifndef COIN_H
#define COIN_H

class Coins {
public:
    Coins() {}
    Coins(float x,float y,float radius,color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    bounding_box_t box;    
    void set_position(float x, float y);
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *object;
};

#endif //COIN_H

#ifndef BEAM_H
#define BEAM_H

class Beams {
public:
    Beams() {}
    Beams(float x,float y,float rotation,color_t color);
    glm::vec3 position;
    float rotation;
    bounding_box_t box;    
    void set_position(float x, float y);
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif 

#ifndef BALLOON_H
#define BALLO0N_H

class Balloons {
public:
    Balloons() {}
    Balloons(float x,float y,float radius,color_t color);
    glm::vec3 position;
    float radius;
    float rotation;
    double speedx;
    double speedy;
    double accy;
    bounding_box_t box;    
    void set_position(float x, float y);
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *object;
};

#endif //BALLOON_H