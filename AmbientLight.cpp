#include "AmbientLight.h"

// Constructor
AmbientLight::AmbientLight(const glm:: vec3 intensity): intensity(intensity) {};

glm::vec3 AmbientLight::getIntensity() const{
    return intensity;
}

std::string AmbientLight::getType() const{
    return "AmbientLight";
}

std::string AmbientLight::toString() const {
    std::ostringstream oss;
    oss << "AmbientLight: { "
        << "Intensity: (" << intensity[0] << ", " << intensity[1] << ", " << intensity[2] << ") }";
    return oss.str();
}




