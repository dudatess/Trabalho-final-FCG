#include "bezier_curve.h"
#include <iostream>


BezierCurve::BezierCurve(float totalDuration, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4)
{
    this->totalDuration = totalDuration;
    this->timer = 0;
    this->isIncreasing = true;

    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
}

void BezierCurve::Update(float deltaTime) {

    if (timer + deltaTime < totalDuration) {
        timer += deltaTime;

    } 
    else
    {
        isIncreasing = false;
    }
    
}

glm::vec4 BezierCurve::GetPoint() {
    float t = timer / totalDuration;
    glm::vec4 c12 = p1 + t * (p2 - p1);
    glm::vec4 c23 = p2 + t * (p3 - p2);
    glm::vec4 c34 = p3 + t * (p4 - p3);
    glm::vec4 c123 = c12 + t * (c23 - c12);
    glm::vec4 c234 = c23 + t * (c34 - c23);
    return c123 + t * (c234 - c123);
}

void BezierCurve::ResetCurve() {
    timer = 0.0f;
}

bool BezierCurve::HasFinished(){

    if(isIncreasing == false) {
        return true;
    }
    else {
        return false;
    }
}
