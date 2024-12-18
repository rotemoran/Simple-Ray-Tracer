#ifndef AmbientLight_H
#define AmbientLight_H

#include <iostream>
#include <string>
#include <array>  
#include <sstream>
#include <glm/glm.hpp> // Include GLM

class AmbientLight{
    private:
        glm::vec3 intensity;

    public:
        AmbientLight(const glm::vec3 intensity);
        glm::vec3 getIntensity() const;
        virtual std::string getType() const;
        virtual std::string toString() const;
        virtual ~AmbientLight() = default;
};

#endif // AmbientLight_H