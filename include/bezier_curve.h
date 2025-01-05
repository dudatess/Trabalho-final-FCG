#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include <glm/glm.hpp> 

class BezierCurve {
public:

    BezierCurve(float totalDuration, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
    bool HasFinished();

    void Update(float deltaTime);
    glm::vec4 GetPoint();
    void ResetCurve();

private:
    float totalDuration;
    float timer;
    bool isIncreasing;
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
    glm::vec4 p4; 
};

#endif // BEZIERCURVE_H
