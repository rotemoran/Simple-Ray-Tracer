#ifndef CAMERA_H
#define CAMERA_H

#include <array>
#include <vector>
#include <cfloat>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <typeinfo>

#include "Object3D.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "IntersectionPoint.h"
#include "Spotlight.h"
#include "Sphere.h"
#include "Plane.h"
#include "Ray.h"


extern const int SCREEN_DIM;        // resolution
extern const double RATIO;          // pixel width
extern glm::vec3 P_C;               // image center 
extern glm::vec3 V_TOWARDS; 
extern glm::vec3 V_UP;
extern glm::vec3 V_RIGHT;


class Camera {
private:
    glm::vec3 cameraPosition;                               // Represents {x, y, z}
    bool multiSampling;                                     // BONUS feature        
    std::vector<Object3D*> objects;                         // Objects in the scene
    std::vector<AmbientLight*> ambientLight;                // Ambient Light in the scene
    std::vector<DirectionalLight*> directionalLights;       // Directional lights
    std::vector<Spotlight*> spotlights;                     // Spotlights 
    cv::Mat screen; 

    void parseConFile(const std::string& filename);         
    void createScene();
    glm:: vec3 getColor(Ray ray, int recursionDepth, const Object3D* currentObject) const;
    IntersectionPoint findIntersectionObj(Ray ray, const Object3D* currentObject) const;
    glm:: vec3 castShadowRay(const glm::vec3& intersectionPoint, 
                   const AmbientLight* lightSource,    
                   const glm::vec3& Li,                
                   const Object3D* currentObject) const;

public:
    // Constructor
    Camera(const std::string& filename);

    // Accessors
    glm::vec3 getPosition() const; 
    bool isMultiSamplingEnabled() const;
    cv::Mat getScreen() const;

    // Getters for objects and lights
    const std::vector<Object3D*>& getObjects() const;
    const std::vector<AmbientLight*>& getAmbientLight() const;
    const std::vector<DirectionalLight*>& getDirectionalLights() const;
    const std::vector<Spotlight*>& getSpotlights() const;
    void toString();

    ~Camera();
};

#endif // CAMERA_H
