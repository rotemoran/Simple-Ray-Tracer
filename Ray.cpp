# include "Ray.h"

// Constructor
Ray::Ray(const glm::vec3 direction, const glm::vec3 position): 
        direction(direction), position(position) {};

glm::vec3 Ray::getDirection() const {
    return direction;
}

glm::vec3 Ray::getPosition() const {
    return position;
}

