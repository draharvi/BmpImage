#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <Windows.h>

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

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    unsigned char* data = nullptr; // массив для данных цветов картинки
    int width = 0, height = 0, bytesPerPixel = 0, padding = 0;

public:

    bool loadBMP(string filename) {
        ifstream inBMP(filename, ios::binary);
        if (!inBMP)
        {
            cerr << "Ошибка: не удалось открыть файл." << endl;
            return false;
        }

        inBMP.read((char*)&fileHeader, sizeof(fileHeader));
        inBMP.read((char*)&infoHeader, sizeof(infoHeader));

        if (fileHeader.bfType != 0x4D42) {
            cerr << "Ошибка: это не BMP-файл." << endl;
            return false;
        }

        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
            cerr << "Ошибка: поддерживаются только 24- и 32-битные BMP." << endl;
            return false;
        }

        width = infoHeader.biWidth; //ширина
        height = infoHeader.biHeight; // высота
        bytesPerPixel = infoHeader.biBitCount / 8; // размер одного пикселя
        padding = (4 - (width * bytesPerPixel) % 4) % 4;

        int dataSize = (width * bytesPerPixel + padding) * height;
        data = new unsigned char[dataSize];

        inBMP.seekg(fileHeader.bfOffBits, std::ios::beg);
        inBMP.read((char*)data, dataSize);
        inBMP.close();
        return true;
    }

    void printToConsole()
    {
        for (int y = height - 1; y > 0; --y)
        {
            for (int x = width - 1; x > 0; --x)
            {
                unsigned char* pixel = getPixel(x, y);
                unsigned char r = pixel[2], g = pixel[1], b = pixel[0];
                if (r == 0 && g == 0 && b == 0)
                    cout << "X";
                else if (r == 255 && g == 255 && b == 255)
                    cout << " ";
                else
                    cout << ".";
            }
         cout << "\n";
        }
    }
    void drawLine(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1), dy = -abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;

        while (true) {
            if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                unsigned char* pixel = getPixel(x1, y1);
                pixel[0] = 0; // B
                pixel[1] = 0; // G
                pixel[2] = 0; // R
            }

            if (x1 == x2 && y1 == y2) break;

            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }

    void drawCross() {
        drawLine(0, 0, width - 1, height - 1);           // диагональ 
        drawLine(width - 1, 0, 0, height - 1);           // диагональ /
    }

    void save(const string& filename) {
        ofstream out(filename, ios::binary);
        if (!out) {
            cerr << "Error! No save file BMP" << endl;
            return;
        }

        out.write((char*)&fileHeader, sizeof(fileHeader));
        out.write((char*)&infoHeader, sizeof(infoHeader));

        for (int y = height - 1; y >= 0; --y) {
            out.write((char*)&data[y * (width * bytesPerPixel + padding)], width * bytesPerPixel);
            out.write("\0\0\0", padding);
        }

        out.close();
    }

    ~BMPImage() {
        delete[] data;
    }

private:
    unsigned char* getPixel(int x, int y)
    {
        int rowSize = width * bytesPerPixel + padding;
        return  &data[(y * rowSize) + (x * bytesPerPixel)];
    }
};


int main()
{
    BMPImage image; // экземпляр класса
    
    string filename, newFileName;
    cout << "Enter input BMP file name: ";
    cin >> filename;

    if (!image.loadBMP(filename))
    {
        cout << "No\n";
        return 0;
    }
    else
    {
        cout << "Yes\n";
    }
    cout << "\nInput pictures:\n";
    
    image.printToConsole();

    image.drawCross();

    std::cout << "\nEnter name new bmp file: ";
    cin >> newFileName;

    image.printToConsole();

    image.save(newFileName);

    return 0;

}