# include "Object3D.h"

// Constructor
Object3D::Object3D(const glm:: vec3 parameters,const glm:: vec3 color, const double shininess, const std::string state): 
        parameters(parameters), color(color), shininess(shininess), state(state) {}

glm:: vec3 Object3D::getParameters() const {
    return parameters;
}

glm:: vec3 Object3D::getColor() const{
    return color;
}

double Object3D::getShininess() const{
    return shininess;
}

std::string Object3D::getState() const{
    return state;
}

std::string Object3D::toString() const {
    std::ostringstream oss;
    oss << "Parameters: (" << parameters[0] << ", " << parameters[1] << ", " << parameters[2] << "), "
        << "Color: (" << color[0] << ", " << color[1] << ", " << color[2] << "), "
        << "Shineness: " << shininess << ", "
        << "State: " << state;
    return oss.str();
}

