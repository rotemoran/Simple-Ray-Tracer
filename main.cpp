#include "Camera.h"

int main(){
    std::string scene = "./goal/scene";
    for (int i = 1; i <= 6; i++){
        std::string filePath = scene + std::to_string(i) + ".txt";
        // Create a Camera object with the file path
        Camera camera(filePath);
        cv::Mat screen = camera.getScreen();
        
        // Save the screen to an image file
        std::string outputFileName = "./result/output_scene" + std::to_string(i) + ".png";
        if (!cv::imwrite(outputFileName, screen)) {
            std::cerr << "Error: Could not write image file " << outputFileName << std::endl;
        } else {
            std::cout << "Saved image: " << outputFileName << std::endl;
        }
    }
    return 0;
}

