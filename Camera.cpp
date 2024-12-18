# include "Camera.h" 

const int SCREEN_DIM = 800; // resolution
const double SCREEN_WIDTH = 2;
const double RATIO = SCREEN_WIDTH / SCREEN_DIM; // pixel width
// Define vectors
glm::vec3 P_C = glm::vec3(0.0, 0.0, 0.0); // Image center
glm::vec3 V_TOWARDS = glm::normalize(glm::vec3(0.0, 0.0, -1.0)); // Towards vector
glm::vec3 V_UP = glm::vec3(0.0, 1.0, 0.0); // Up vector
glm::vec3 V_RIGHT = glm::normalize(glm::cross(V_TOWARDS, V_UP)); // Right

Camera::Camera(const std::string& filename){
    screen = cv::Mat::zeros(SCREEN_DIM, SCREEN_DIM, CV_8UC3);
    parseConFile(filename);
    createScene();
}

void Camera::parseConFile(const std::string& filename){  
    std::vector<std::vector<std::string>> elements;
    std::vector<std::vector<std::string>> colors;
    std::vector<std::vector<std::string>> lights;
    std::vector<std::vector<std::string>> intensities;
    std::vector<std::vector<std::string>> lightsPositions;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "e") {
            // Camera (Eye Position)
            std::string multiSamplingFlag;
            double x, y, z;
            iss >> x >> y >> z >> multiSamplingFlag;
            cameraPosition = glm::vec3(x, y, z);
            multiSampling = (multiSamplingFlag != "0.0"); // 0.0 = no, 1.0 = yes
        } 
        else if (type == "a") {
            // Global Ambient Light
            double r, g, b, unused;
            iss >> r >> g >> b >> unused;
            ambientLight.push_back(new AmbientLight(glm::vec3(r, g, b)));
        }
        else if (type == "o" || type == "r" || type == "t") {
            // Object (Sphere or Plane)
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            elements.push_back({type, element[0], element[1], element[2], element[3]});
        }
        else if (type == "c") {
            // Color and Shininess
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            colors.push_back({element[0], element[1], element[2], element[3]});
        }
        else if (type == "d") {
            // Directional Light
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            lights.push_back({element[0], element[1], element[2], element[3]});
        } 
        else if (type == "p") {
            // Spotlight Position
            std::vector<std::string> element(4);
            iss >> element[0] >> element[1] >> element[2] >> element[3];
            lightsPositions.push_back({element[0], element[1], element[2], element[3]});
        } 
        else if (type == "i") {
            // Light Intensity
            std::vector<std::string> element(3);
            iss >> element[0] >> element[1] >> element[2];
            intensities.push_back({element[0], element[1], element[2]});
        }
    }
    
    // Add objects to the scene
    for (unsigned long i = 0; i < elements.size(); i++) {
        std::vector<std::string> element = elements[i];
        std::vector<std::string> color = colors[i];

        // Convert parameters and color to glm::vec3
        glm::vec3 params(std::stod(element[1]), std::stod(element[2]), std::stod(element[3]));
        glm::vec3 objColor(std::stod(color[0]), std::stod(color[1]), std::stod(color[2]));

        if (std::stod(element[4]) > 0) {
            // Create Sphere object and add to vector
            objects.push_back(new Sphere(
                params,                 // glm::vec3 parameters
                objColor,               // glm::vec3 color
                std::stod(color[3]),    // Shininess
                element[0],             // State
                std::stod(element[4])   // Radius
            ));
        } else {
            // Create Plane object and add to vector
            objects.push_back(new Plane(
                params,                 // glm::vec3 parameters
                objColor,               // glm::vec3 color
                std::stod(color[3]),    // Shininess
                element[0],             // State
                std::stod(element[4])   // d 
            ));
        }
    }
    // Add lights to the scene
    int spotlightNextIndx = 0;
    for (unsigned long i = 0; i < lights.size(); i++) {
        std::vector<std::string> light = lights[i];
        std::vector<std::string> intensity = intensities[i];

        // Convert light direction and intensity to glm::vec3
        glm::vec3 lightDirec(std::stod(light[0]), std::stod(light[1]), std::stod(light[2]));
        glm::vec3 lightInten(std::stod(intensity[0]), std::stod(intensity[1]), std::stod(intensity[2]));

        // Spotlight = 1.0, Directional = 0.0
        if (std::stod(light[3]) == 0.0) {
            // Add directional light
            directionalLights.push_back(new DirectionalLight(
                lightInten, // Intensity
                lightDirec  // Direction
            ));
        } 
        else {
            // Add spotlight
            std::vector<std::string> pos = lightsPositions[spotlightNextIndx];
            glm::vec3 lightPos(std::stod(pos[0]), std::stod(pos[1]), std::stod(pos[2]));
            spotlightNextIndx++;

            spotlights.push_back(new Spotlight(
                lightInten,          // Intensity
                lightDirec,          // Direction
                lightPos,            // Position
                std::stod(pos[3])    // Cutoff angle cosine value
            ));
        }
    }
    file.close();
}

void Camera::createScene() {
    const float SAMPLES = multiSampling ? 4 : 1; 
    const float OFFSET = RATIO / 2.0f;                  // Sub-pixel offset for multi-sampling

    for (int y = 0; y < screen.rows; y++) {           
        for (int x = 0; x < screen.cols; x++) { 
            glm::vec3 accumulatedColor(0.0f); 

            if (multiSampling) {
                // Generate 4 sub-pixel rays (2x2 grid for multi-sampling)
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        // Calculate sub-pixel position
                        float offsetX = (j * OFFSET) - OFFSET / 2.0f;
                        float offsetY = (i * OFFSET) - OFFSET / 2.0f;

                        glm::vec3 subPixelCenter = P_C +
                            float((x - SCREEN_DIM / 2) * RATIO + offsetX) * V_RIGHT -
                            float((y - SCREEN_DIM / 2) * RATIO + offsetY) * V_UP;

                        glm::vec3 rayDirection = glm::normalize(subPixelCenter - cameraPosition);
                        Ray subPixelRay(rayDirection, cameraPosition);

                        // Accumulate color
                        accumulatedColor += getColor(subPixelRay, 5, nullptr);
                    }
                }
            } else {
                // Single ray per pixel (no multi-sampling)
                glm::vec3 pixelCenter = P_C +
                                        float((x - SCREEN_DIM / 2) * RATIO) * V_RIGHT -
                                        float((y - SCREEN_DIM / 2) * RATIO) * V_UP;

                glm::vec3 rayDirection = glm::normalize(pixelCenter - cameraPosition);
                Ray pixelRay(rayDirection, cameraPosition);

                accumulatedColor = getColor(pixelRay, 5, nullptr);
            }

            // Average the accumulated colors (for multi-sampling)
            glm::vec3 finalColor = accumulatedColor / SAMPLES;

            // Set pixel color
            screen.at<cv::Vec3b>(y, x) = cv::Vec3b(
                static_cast<uchar>(finalColor.b * 255),
                static_cast<uchar>(finalColor.g * 255),
                static_cast<uchar>(finalColor.r * 255)
            );
        }
    }
}

// Calculate the color at a certain pixel
glm:: vec3 Camera::getColor(Ray ray, int recursionDepth, const Object3D* currentObject) const{
    glm::vec3 result = glm::vec3(0,0,0);
    if (recursionDepth == 0)
        return result;
    
    IntersectionPoint intersectedRes = findIntersectionObj(ray, currentObject);

    if (!intersectedRes.isValid())
        return result;

    const Object3D* object = intersectedRes.getObject();
    glm::vec3 intersectionPos = intersectedRes.getPosition();
    
    glm::vec3 N = object->getNormal(intersectionPos);
    glm::vec3 Kr = glm::vec3(0,0,0);

    if(object -> getState() == "r"){ // Reflective object
        glm::vec3 reflectDirection = glm::normalize(glm::reflect(ray.getDirection(), N));
        Ray reflectedRay(reflectDirection, intersectionPos);
        Kr = getColor(reflectedRay, recursionDepth -1, object);
        return glm::clamp(Kr, 0.0f, 1.0f);
    }
    if(object -> getState() == "t"){ // Transperent object. Must: object is a Shpere.
        const Sphere* sphere = dynamic_cast<const Sphere*>(object);
        float reflectiveIdx = 1.5f;
        glm::vec3 refractDirection = glm::normalize(glm::refract(ray.getDirection(), N, 1.0f/reflectiveIdx)); 
        Ray refractedRay(refractDirection, intersectionPos);

        IntersectionPoint farIntersection =  sphere -> farIntersection(refractedRay);
        glm::vec3 exitNormal = glm::normalize(object->getParameters() - farIntersection.getPosition());
        refractDirection = glm::normalize(glm::refract(refractDirection, exitNormal, reflectiveIdx/1.0f));
        Ray outRefractedRay(refractDirection, farIntersection.getPosition());
        Kr = getColor(outRefractedRay, recursionDepth -1, object);
        return glm::clamp(Kr, 0.0f, 1.0f);
    }

    // Normal object
    glm::vec3 Ie = glm::vec3(0.0f, 0.0f, 0.0f);                                                
    glm::vec3 Ia = ambientLight[0]->getIntensity();                                
    glm::vec3 Ii, Li, Ri;
    glm::vec3 Ka = object->getColor();
    glm::vec3 Kd = Ka;
    glm::vec3 Ks = glm::vec3(0.7f,0.7f,0.7f);
    glm::vec3 Si = glm::vec3(1.0f, 1.0f, 1.0f);                                              
    double n = object->getShininess();
    glm::vec3 V = glm::normalize(ray.getPosition() - intersectionPos);              

    if(object->getType() == "Plane"){
        const Plane* plane = dynamic_cast<const Plane*>(object);
        Ka = plane->checkerboardColor(Ka, intersectionPos);
        Kd = Ka;
    }
    result += Ie + Ka * Ia;
    for (size_t i=0; i<directionalLights.size(); i++){
        Ii = directionalLights[i]->getIntensity();                              
        Li = glm::normalize(-directionalLights[i]->getDirection());             
        Ri = glm::normalize(glm::reflect(-Li, N));                              

        Si = castShadowRay(intersectionPos, directionalLights[i], Li, object);

        float diffuse = glm::dot(N, Li);
        float specular = glm::pow(glm::max(glm::dot(V, Ri), 0.0f), n);

        result += (glm::max(Kd * diffuse,0.0f) + glm::max(Ks * specular, 0.0f)) * Ii * Si;
    }
    for (size_t i=0; i<spotlights.size(); i++){
        Ii = spotlights[i]->getIntensity();                                     
        Li = glm::normalize(spotlights[i]->getPosition()-intersectionPos);      
        Ri = glm::normalize(glm::reflect(-Li, N));                              
        
        Si = castShadowRay(intersectionPos, spotlights[i], Li, object);

        float diffuse = glm::dot(N, Li); 
        float specular = glm::pow(glm::max(glm::dot(V, Ri), 0.0f), n);
        
        glm::vec3 spotlightDir = glm::normalize(spotlights[i]->getDirection());
        float cosTheta = glm::dot(spotlightDir, -Li);
        if (cosTheta < spotlights[i]->getCutAngleCos()) 
            Si = glm::vec3(0.0f, 0.0f, 0.0f); // Outside cone

        result += (glm::max(Kd * diffuse,0.0f) + glm::max(Ks * specular, 0.0f)) * Ii * Si; 
    }
    return glm::clamp(result, 0.0f, 1.0f);
}

// find the intersection point to the closest obj
IntersectionPoint Camera::findIntersectionObj(Ray ray, const Object3D* currentObject) const {
    float minDist = FLT_MAX;
    IntersectionPoint minPoint = IntersectionPoint::NO_INTERSECTION;
    
    // Iterate through all objects in the scene
    for (const Object3D* obj : objects) { 
        if(obj==currentObject)
            continue;

        IntersectionPoint intersectionPoint = obj->intersection(ray);
        // If the intersection exists and is closer than the current min distance
        if (intersectionPoint.isValid() && minDist > intersectionPoint.getDistance()) {
            minDist = intersectionPoint.getDistance();
            minPoint = intersectionPoint;
        }
    }
    return minPoint;
}

glm::vec3 Camera::castShadowRay(const glm::vec3& intersectionPoint,             // Ray origin from intersection point
                                const AmbientLight* lightSource,                // Light source (raw pointer)
                                const glm::vec3& Li,                            // Ray direction (toward light)
                                const Object3D* currentObject                   // Object generating the shadow ray (raw pointer)
                                ) const {             

    for (const Object3D* object : objects) {
        if (object == currentObject)
            continue; // Ignore self-intersection

        // Intersect the ray with the object
        IntersectionPoint new_intersection = object->intersection(Ray(Li, intersectionPoint)); 
        
        if (!new_intersection.isValid()) 
            continue; // Skip if no intersection
        
        if (lightSource->getType() == "Spotlight") {
            const Spotlight* spotlightSource = dynamic_cast<const Spotlight*>(lightSource);
            // Check if the object is behind the spotlight
            float lightDistance = glm::length(new_intersection.getPosition() - intersectionPoint);
            float rayDistance = glm::length(intersectionPoint - spotlightSource->getPosition());
            if (lightDistance > rayDistance)
                continue; // Intersection point is behind the spotlight
        }
        return glm::vec3(0.0f, 0.0f, 0.0f);    // If we get here, the ray is blocked
    }
    return glm::vec3(1.0f, 1.0f, 1.0f);       // No objects block the ray
}

glm::vec3 Camera::getPosition() const{
    return cameraPosition;
}

bool Camera::isMultiSamplingEnabled() const{
    return multiSampling;
}

cv::Mat Camera:: getScreen() const{
    return screen;
}

const std::vector<Object3D*>& Camera::getObjects() const {
    return objects;
}

const std::vector<AmbientLight*>& Camera::getAmbientLight() const {
    return ambientLight;
}

const std::vector<DirectionalLight*>& Camera::getDirectionalLights() const {
    return directionalLights;
}

const std::vector<Spotlight*>& Camera::getSpotlights() const {
    return spotlights;
}

void Camera::toString(){
    std::cout << "Camera Position: (" << cameraPosition[0] << ", " << cameraPosition[1] << ", " << cameraPosition[2] << ") ";
    std::cout << std::endl;
    std::cout << "multiSampling: " << multiSampling;
    std::cout << std::endl;
    std::cout << ambientLight[0]->toString();
    std::cout << std::endl;
    for (const auto& element : objects) {
        std::cout << element->toString();
        std::cout << std::endl;
    }
    for (const auto& element : directionalLights) {
        std::cout << element->toString();
        std::cout << std::endl;
    }
    for (const auto& element : spotlights) {
    std::cout << element->toString();
    std::cout << std::endl;
    }
}

Camera::~Camera() {
    for (Object3D* obj : objects) {
        delete obj;
    }
    for (AmbientLight* light : ambientLight) {
        delete light;
    }
    for (DirectionalLight* light : directionalLights) {
        delete light;
    }
    for (Spotlight* light : spotlights) {
        delete light;
    }
}