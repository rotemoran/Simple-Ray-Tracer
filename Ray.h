#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <string>
#include <glm/glm.hpp>

class Ray{
private:
    glm::vec3 direction;
    glm::vec3 position;

public:
   Ray(const glm::vec3 direction, const glm::vec3 position);
   
   glm::vec3 getDirection() const;
   glm::vec3 getPosition() const;

};

#endif // RAY_H