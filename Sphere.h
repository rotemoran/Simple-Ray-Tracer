#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp> 
#include "Object3D.h"

class Sphere: public Object3D{
private:
    const double r;

public:
    // Constructor
    Sphere(const glm::vec3 parameters, const glm::vec3 color, const double shininess, const std::string state, const double r);

    double getRadius() const;

    std::string toString() const override;
        std::string getType() const override;
    glm:: vec3 getNormal(glm::vec3 intersectionPosition)const override;
    IntersectionPoint intersection(const Ray& ray) const override;

    IntersectionPoint farIntersection(const Ray& ray) const;
};

#endif // SPHERE_H