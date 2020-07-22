#include <fstream>
#include <iostream>

int main() {
    uint width = 500;
    uint height = 500;
    const char* fileName = "gradientColorImage.ppm";
    std::ofstream outFile(fileName);  //opens the file to write

    //output P3 250 250 255 as the header
    outFile << "P3\n"
            << width << " " << height << " 255 ";
    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) {
            //output gradient color
            uint red = 255 * x / width;
            uint green = 127;
            uint blue = 255 * y / width;
            outFile<<red<<" "<<green<<" "<<blue<<std::endl;
        }
    }
    outFile.close();

    return 0;
}