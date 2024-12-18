#ifndef INTERSECTION_POINT_H
#define INTERSECTION_POINT_H

#include <array>
#include <memory>
#include <glm/glm.hpp>  
#include "Object3D.h"


class IntersectionPoint {
private:
    glm::vec3 position;             // The coordinates of the intersection point {x, y, z}
    double distance;                // Distance from the ray origin to the intersection point
    const Object3D* object;         // The object that the ray intersected

public:
    // Constructor
    IntersectionPoint(const glm::vec3 position,
                      const double distance,
                      const Object3D* object);

    static const IntersectionPoint NO_INTERSECTION;

    // Getters
    glm::vec3 getPosition() const;
    double getDistance() const;
    const Object3D* getObject() const;
    bool isValid() const;
    std::string toString() const;
};

#endif // INTERSECTION_POINT_H
