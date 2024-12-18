#ifndef Spotlight_H
#define Spotlight_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "DirectionalLight.h"

class Spotlight : public DirectionalLight{
private:
    const glm::vec3 position;
    const double cutAngleCos;

public:
   Spotlight(const glm::vec3 intensity, const glm::vec3 direction, const glm::vec3 position, const double cutAngleCos);
   
   glm::vec3 getPosition() const;
   double getCutAngleCos() const;
   std::string getType() const override;
   std::string toString() const override;
   
};

#endif // Spotlight_H