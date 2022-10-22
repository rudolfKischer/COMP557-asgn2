//Rudolf C. Kischer
//260956107
#include "ArcBall.h"
#include "MatrixStack.h"
#include "Program.h"

#include <iostream>
#include <cmath>
#include <cassert>

#include "GLSL.h"

using namespace std;


ArcBall::ArcBall():R(glm::mat4(1.0)), Rmem(glm::mat4(1.0)), p0(glm::vec3(1.0)), p1(glm::vec3(1.0)), fit(0.5f), gain(5)
{
}

ArcBall::~ArcBall()
{
}

glm::vec3 ArcBall::computeVecFromMousePos(double mousex, double mousey, int windowWidth, int windowHeight) {
    float minScreenDim = windowWidth;
    if(windowHeight < windowWidth){
        minScreenDim = windowHeight;
    }
    float radius = minScreenDim / this->fit;
    float xpos = mousex - (windowWidth/2.0f);
    float ypos = mousey - (windowHeight/2.0f);
    glm::vec3 mouse = glm::vec3( xpos, -ypos, 0.0f);
    
    glm::vec3 direction =normalize(mouse);
    if(length(mouse) > radius){
        mouse = direction * radius;
    }
    float val = (radius*radius) - (length(mouse) * length(mouse));
    if (val < 0){
        val = 0;
    }
    mouse[2] = sqrt(val);
	//TODO: compute the projection of mouse coords on the arcball
	return mouse;
}

double computeVectorAngle(glm::vec3& v1, glm::vec3& v2) {
	double vDot = glm::dot(v1, v2);
	if (vDot < -1.0) vDot = -1.0;
	if (vDot > 1.0) vDot = 1.0;
	return((double)(acos(vDot)));
}

void ArcBall::startRotation(double mousex, double mousey, int windowWidth, int windowHeight) {
	Rmem = R;
	p0 = computeVecFromMousePos(mousex, mousey, windowWidth, windowHeight);
}

void ArcBall::updateRotation(double mousex, double mousey, int windowWidth, int windowHeight) {
	//TODO: compute the rotation update for the view camera
    p1 = computeVecFromMousePos(mousex, mousey, windowWidth, windowHeight);
    glm::vec3 p0norm = normalize(p0);
    glm::vec3 p1norm = normalize(p1);
    float angle = float(computeVectorAngle( p0norm, p1norm )) * gain;
    cout << angle << endl;
    
    glm::vec3 axis = glm::cross(p0norm, p1norm);
    printf("%f ,%f,%f\n",axis[0],axis[1],axis[2]);
    
    if (length(axis) != 0.0f){
        R = glm::rotate(glm::mat4(1.0f), angle, axis) * Rmem;
    }else{
        R = Rmem;
    }
    
    
}
