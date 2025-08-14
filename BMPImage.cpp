#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>

using namespace std;

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t bfType;     
    uint32_t bfSize;     
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;  
};

struct BMPInfoHeader {
    uint32_t biSize;        
    int32_t  biWidth;       
    int32_t  biHeight;      
    uint16_t biPlanes;      
    uint16_t biBitCount;    
    uint32_t biCompression; 
    uint32_t biSizeImage;   
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;     
    uint32_t biClrImportant;
};

#pragma pack(pop)



class BMPImage {
    private:

    BMPFileHeader fileHeader{};
    BMPInfoHeader infoHeader{};
    unsigned char* data = nullptr; // массив для данных цветов картинки
    int width = 0, height = 0, bytesPerPixel = 0, padding = 0;

    public:

    bool loadBMP (string filename){
        ifstream inBMP(filename, ios::binary);
        if(!inBMP)
        {
            std::cerr << "Ошибка: не удалось открыть файл." << std::endl;
            return false;
        }

        inBMP.read((char*)&fileHeader, sizeof(fileHeader));
        inBMP.read((char*)&infoHeader, sizeof(infoHeader));

        return true;
    }
    
};