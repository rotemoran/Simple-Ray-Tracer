# include "Spotlight.h"

// Constructor
Spotlight::Spotlight(const glm:: vec3 intensity, const glm:: vec3 direction, const glm:: vec3 position, const double cutAngleCos): 
        DirectionalLight(intensity, direction),position(position), cutAngleCos(cutAngleCos) {};

glm:: vec3 Spotlight::getPosition() const {
    return position;
}

double Spotlight::getCutAngleCos() const {
    return cutAngleCos;
}

std::string Spotlight::getType() const {
    return "Spotlight";
}

std::string Spotlight::toString() const {
    const glm:: vec3 inten = getIntensity();
    const glm:: vec3 direction = getDirection();

    std::ostringstream oss;
    oss << "SpotLight: { "
        << "Direction: (" << direction[0] << ", " << direction[1] << ", " << direction[2] << "),"
        << " Position: (" << position[0] << ", " << position[1] << ", " << position[2] << "),"
        << " Intensity: (" << inten[0] << ", " << inten[1] << ", " << inten[2] << "),"
        << " Cutoff angle cosine value: " << cutAngleCos << "}";
    return oss.str();
}

