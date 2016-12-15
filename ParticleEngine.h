//
// Created by jasillo on 29/04/16.
//
#ifndef SISPARTICULAS_PARTICLEENGINE_H
#define SISPARTICULAS_PARTICLEENGINE_H
#define GLUT_DISABLE_ATEXIT_HACK
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <random>
#include <vector>
#include "glm/glm.hpp"


using namespace std;

const float PI = 3.1415926535f;
const float GRAVITY = 9.8f;
const float k = 2; //air resintance


//Represents a single particle.
struct Particle {
    glm::vec3 pos;
    glm::vec3 velocity;
    float timeAlive; // amount of time that a particle has lived
    float timespam; // amount of time that a particle must live
    float mass;
    float size;
    glm::vec3 color;
    float alpha;
    bool alive;
};


class ParticleEngine {
private:
    vector<Particle*> myParticles;
    //the origin of  particle system
    glm::vec3 origin;
    glm::vec3 destiny;
    glm::vec3 direction;
    bool AnimationContious;
    glm::vec3 color;
    void createParticle(Particle* p);
    int typeAnimation;
    GLuint textureId;

public:
    ParticleEngine(int);
    ~ParticleEngine();
    //Draws the particle fountain.
    void draw();
    void setPoint(float ,float , float , float ,bool);
    void setDistance(float );
    //advance of one particles to next step
    void step(float);
    void setTypeAnimation(int);
    void setContinuous(bool);
    void restartParticles();
    void setColor(float,float,float);
    void setTexture(GLuint);
};


#endif //SISPARTICULAS_PARTICLEENGINE_H
