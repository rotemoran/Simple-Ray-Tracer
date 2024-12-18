#include "Plane.h"
#include "IntersectionPoint.h"

Plane::Plane(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double d): 
Object3D(parameters, color, shininess, state), d(d) {}

double Plane::getD() const{
    return d;
}

glm::vec3 Plane::checkerboardColor(glm::vec3 rgbColor, glm::vec3 hitPoint) const{
    // Checkerboard pattern
    float scaleParameter = 0.5f;
    float checkerboard = 0.0;

    if (hitPoint.x < 0) {
        checkerboard += floor((0.5 - hitPoint.x) / scaleParameter);
    } else {
        checkerboard += floor(hitPoint.x / scaleParameter);
    }

    if (hitPoint.y < 0) {
        checkerboard += floor((0.5 - hitPoint.y) / scaleParameter);
    } else {
        checkerboard += floor(hitPoint.y / scaleParameter);
    }

    checkerboard = std::fmod(checkerboard * 0.5, 1.0) * 2.0;

    if (checkerboard > 0.5) {
        return 0.5f * rgbColor;
    }
    return rgbColor;
}

std::string Plane::toString() const {
    std::ostringstream oss;
    oss << "Plane: { "
        << Object3D::toString() << ", "
        << "d: " << d << " }";
    return oss.str();
}

std::string Plane::getType() const{
    return "Plane";
}

glm:: vec3 Plane::getNormal(glm::vec3 intersectionPosition)const{
    return glm::normalize(-getParameters());
}

IntersectionPoint Plane::intersection(const Ray& ray) const {
    glm::vec3 normal = getNormal(glm::vec3(0,0,0));
    
    float denominator = glm::dot(ray.getDirection(), normal);

    // If the denominator is close to zero, the ray is parallel to the plane
    if (std::abs(denominator) < 1e-6)
        return IntersectionPoint::NO_INTERSECTION;

    glm::vec3 Q0 = getPointOnPlane(); 
    float numerator = glm::dot(Q0 - ray.getPosition(), normal);

    float t = numerator / denominator;

    // If t is negative, the intersection is behind the ray's origin
    if (std::abs(t) < 0.0f) 
        return IntersectionPoint::NO_INTERSECTION;

    // Calculate the intersection point
    glm::vec3 intersectionPosition = ray.getPosition() + t * ray.getDirection();

    return IntersectionPoint(intersectionPosition, t, this);
}

glm::vec3 Plane::getPointOnPlane() const {
    glm::vec3 normal = getParameters();    
                        
    // Determine Q0 by checking components of the normal
    if (std::abs(normal.x) > 1e-6) {
        return glm::vec3(-d / normal.x, 0, 0); // Set y=0, z=0
    } else if (std::abs(normal.y) > 1e-6) {
        return glm::vec3(0, -d / normal.y, 0); // Set x=0, z=0
    } else if (std::abs(normal.z) > 1e-6) {
        return glm::vec3(0, 0, -d / normal.z); // Set x=0, y=0
    } else {
        throw std::runtime_error("Invalid plane parameters: normal vector is zero.");
    }
}






