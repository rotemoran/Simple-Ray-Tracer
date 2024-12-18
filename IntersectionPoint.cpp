#include "IntersectionPoint.h"


// Constructor
IntersectionPoint::IntersectionPoint(const glm::vec3 position,
                                    const double distance,
                                    const Object3D* object)
    : position(position), distance(distance), object(object){}

const IntersectionPoint IntersectionPoint::NO_INTERSECTION = IntersectionPoint(glm::vec3(0,0,0), -1, nullptr);

// Getters
glm::vec3 IntersectionPoint::getPosition() const {
    return position;
}

double IntersectionPoint::getDistance() const {
    return distance;
}

const Object3D* IntersectionPoint::getObject() const {
    return object;
}

bool IntersectionPoint::isValid() const {
    return distance > 0.001; 
}

std::string IntersectionPoint::toString() const {
    std::ostringstream oss;
    oss << "IntersectionPoint(";
    oss << "Position: (" << position.x << ", " << position.y << ", " << position.z << "), ";
    oss << "Distance: " << distance << ", ";
    oss << "Object: " << (object ? object->toString() : "nullptr");
    oss << ")";
    return oss.str();
}

