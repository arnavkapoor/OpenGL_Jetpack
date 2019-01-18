#include "main.h"

#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
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

#endif // BALL_H

#ifndef COIN_H
#define COIN_H

class Coins {
public:
    Coins() {}
    Coins(float x,float y,float radius,color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    void set_position(float x, float y);
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *object;
};

#endif 
