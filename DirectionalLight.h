#ifndef DirectionalLight_H
#define DirectionalLight_H

#include <iostream>
#include <string>
#include <sstream>
#include <glm/glm.hpp> 
#include "AmbientLight.h"


class DirectionalLight : public AmbientLight{
private:
    const glm::vec3 direction;

public:
   DirectionalLight(const glm::vec3 intensity, const glm::vec3 direction);

   glm::vec3 getDirection() const;
   std::string getType() const override;
   std::string toString() const override;
};

#endif // DirectionalLight_H