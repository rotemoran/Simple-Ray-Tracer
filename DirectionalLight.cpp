# include "DirectionalLight.h"

// Constructor
DirectionalLight::DirectionalLight(const glm:: vec3 intensity, const glm::vec3 direction): 
        AmbientLight(intensity),direction(direction) {};

glm::vec3 DirectionalLight::getDirection() const {
    return direction;
}

std::string DirectionalLight::getType() const{
    return "DirectionalLight";
}

std::string DirectionalLight::toString() const {
    const glm::vec3 inten = getIntensity();

    std::ostringstream oss;
    oss << "DirectionalLight: {"
        << "Direction: (" << direction[0] << ", " << direction[1] << ", " << direction[2] << "),"
        << " Intensity: (" << inten[0] << ", " << inten[1] << ", " << inten[2] << ")}";
    return oss.str();
}
