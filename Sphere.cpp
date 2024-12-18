#include "Sphere.h"
#include "IntersectionPoint.h"

Sphere::Sphere(const glm:: vec3 parameters, const glm:: vec3 color, const double shininess, const std::string state, const double r): 
Object3D(parameters, color, shininess, state), r(r)  {}

double Sphere::getRadius() const{
    return r;
}

std::string Sphere::toString() const {
    std::ostringstream oss;
    oss << "Sphere: { "
        << Object3D::toString() << ", "
        << "Radius: " << r << " }";
    return oss.str();
}

std::string Sphere::getType() const{
    return "Sphere";
}

glm:: vec3 Sphere::getNormal(glm::vec3 intersectionPosition)const{
    return glm::normalize(intersectionPosition - getParameters());
}

IntersectionPoint Sphere::intersection(const Ray& ray) const{
    // Coefficients for the quadratic equation
    glm::vec3 direction = glm::normalize(ray.getDirection());
    float a = glm::dot(direction, direction);
    float b = 2 * glm::dot(direction, ray.getPosition() - getParameters());
    float c = glm::dot(ray.getPosition() - getParameters(), ray.getPosition() - getParameters()) - r * r;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {// No intersection
        return IntersectionPoint::NO_INTERSECTION;
    }

    // Calculate the two solutions for t
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2 * a); // Closest intersection point
    float t2 = (-b + sqrtDiscriminant) / (2 * a); // Farther intersection point

    // Find the closest valid t (positive t means the intersection is in front of the ray origin)
    float t = std::min(t1, t2);
    if(std::abs(t) < 0.0f)
        t = std::max(t1, t2);
    if (std::abs(t) < 0.0f) {// Both intersection points are behind the ray origin
        return IntersectionPoint::NO_INTERSECTION;
    }

    // Calculate the intersection point and distance
    glm::vec3 intersectionPosition = ray.getPosition() + t * direction;
    
    return IntersectionPoint(intersectionPosition, t, this);
}

IntersectionPoint Sphere::farIntersection(const Ray& ray) const{
    // Coefficients for the quadratic equation
    glm::vec3 direction = glm::normalize(ray.getDirection());
    float a = glm::dot(direction, direction);
    float b = 2 * glm::dot(direction, ray.getPosition() - getParameters());
    float c = glm::dot(ray.getPosition() - getParameters(), ray.getPosition() - getParameters()) - r * r;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {// No intersection
        return IntersectionPoint::NO_INTERSECTION;
    }

    // Calculate the two solutions for t
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2 * a); // Closest intersection point
    float t2 = (-b + sqrtDiscriminant) / (2 * a); // Farther intersection point

    // Find the closest valid t (positive t means the intersection is in front of the ray origin)
    float t = std::max(t1, t2);
    if (std::abs(t) < 0.0f) {// Both intersection points are behind the ray origin
        return IntersectionPoint::NO_INTERSECTION;
    }

    // Calculate the intersection point and distance
    glm::vec3 intersectionPosition = ray.getPosition() + t * direction;

    return IntersectionPoint(intersectionPosition, t, this);
}





