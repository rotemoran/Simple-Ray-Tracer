#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Object3D.h"

class Plane: public Object3D{
private:
    const double d;
    glm::vec3 getPointOnPlane() const;

public:
    // Constructor
    Plane(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double r);

    double getD() const;
    glm::vec3 checkerboardColor(glm::vec3 rgbColor, glm::vec3 hitPoint)const;
    
    std::string toString() const override;
    std::string getType() const override;
    glm:: vec3 getNormal(glm::vec3 intersectionPosition)const override;
    IntersectionPoint intersection(const Ray& ray) const override;
};

#endif // Plane_H