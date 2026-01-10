/**
 * 2025-9-28
 * 8.1.1Line Drawing
 * 修改为easyx,使用easyx代替tgaimage 效果更好
 */
//#include "tgaimage.h"
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
using namespace std;
//声明
void GenerateLine(int width, int height,vector<int> pos1, vector<int> pos2);

int main(int argc, char** argv) {

    // 初始化图形窗口
    constexpr int width  = 1280; 
    constexpr int height = 720; 
    initgraph(width, height);  // 创建 640x480 像素的窗口

    vector<int>point1 = {0,0};
    vector<int>point2 = {1000,80};//根据约束条件x1必须大于x0
    GenerateLine(width, height,point1, point2);
    _getch();
    return 0;
}

//使用斜率不为0的中点算法绘制直线2025.9.25
void GenerateLine(int width, int height,vector<int> pos1, vector<int> pos2)
{
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
        putpixel(x, y, WHITE);
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

}



