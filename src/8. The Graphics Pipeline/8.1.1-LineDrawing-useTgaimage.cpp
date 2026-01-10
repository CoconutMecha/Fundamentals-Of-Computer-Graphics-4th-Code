/**
 * 8.1.1Line Drawing
 */
#include "tgaimage.h"
#include <iostream>
using namespace std;
constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
TGAImage GenerateLine(int width, int height,vector<int> pos1, vector<int> pos2);

int main(int argc, char** argv) {

    
    constexpr int width  = 1280; 
    constexpr int height = 720; 

    vector<int>point1 = {100,80};
    vector<int>point2 = {1000,80};//根据约束条件x1必须大于x0
    TGAImage framebuffer = GenerateLine(width, height,point1, point2);
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

//绘制直线2025.9.25
TGAImage GenerateLine(int width, int height,vector<int> pos1, vector<int> pos2)
{
    TGAImage framebuffer(width, height, TGAImage::RGB);
    vector<int> allpos;
    int x,y;
    int x0 = pos1[0];
    int y0 = pos1[1];
    int x1 = pos2[0];
    int y1 = pos2[1];
    y = y0;
    float d = (y0 - y1) * (x + 1) + (x1 - x0) * (y + 0.5) + x0 * y1 + x1 * y0; //d是函数值f（x+0.5,y+1）
    for(x=x0; x<=x1; x++)
    {
        framebuffer.set(x, y, white);
        if( d < 0)//直线在中点的上方，绘制右上侧像素
        {
            y = y + 1;
            d = d + (x1 - x0) + (y0 - y1);
        }
        else
        {
            d = d + (y0 - y1);
        }
    }

    return framebuffer;

}



