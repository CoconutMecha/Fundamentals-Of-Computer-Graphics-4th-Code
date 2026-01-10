/**
 * 2025.9.28
 * 8.1.2三角形光栅化算法 brute-force版
 * ABC逆时针排列 坐下标依次为0，1，2
 * 以下的三角形面积其实是正常面积的二倍
 */
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

struct Bounds {
    float xmin, ymin, xmax, ymax;
};
//函数声明
Bounds Coordinates(vector<float> triangleA, vector<float> triangleB, vector<float> triangleC);
float ComputeAlpha(float x,float y);
float ComputeBeta(float x,float y);
float ComputeGamma(float x,float y);
float max3(float a, float b, float c);

//三角形的三个顶点，easyx以左上角为原点，采用的是屏幕坐标系（2d）
/**
 * 备注：在3D图形学中，左手坐标系（Z轴正方向指向屏幕外）和右手坐标系（Z轴正方向指向屏幕内）是常见的。但EasyX是一个2D图形库，不涉及3D，所以通常我们只讨论2D坐标系。
 */
static const vector<float> triangleA = {400,213};  // 上顶点
static const vector<float> triangleB = {300,387};  // 左下
static const vector<float> triangleC = {500,387};  // 右下
//A点坐标
static float x0 = triangleA[0];
static float y0 = triangleA[1];
//B点坐标
static float x1 = triangleB[0];
static float y1 = triangleB[1];
//C点坐标
static float x2 = triangleC[0];
static float y2 = triangleC[1];


//颜色
COLORREF c0 = RGB(255, 0, 0);   // 红色，顶点A
COLORREF c1 = RGB(0, 255, 0);   // 绿色，顶点B
COLORREF c2 = RGB(0, 0, 255);   // 蓝色，顶点C

//COLORREF red = RGB(255, 0, 0);     // 纯红色A


int main(int argc, char const *argv[])
{

    //坐标范围
    Bounds edge = Coordinates(triangleA,triangleB,triangleC);
    static float xmin = edge.xmin;
    static float ymin = edge.ymin;
    static float xmax = edge.xmax;
    static float ymax = edge.ymax;

    std::cout << xmin<<"+"<<xmax<<"+"<<ymin<<"+"<<ymax <<std::endl;
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height);  // 创建 1280x720像素的窗口


    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();
    //根据重心坐标系约束条件判断是否绘制像素
    //数学上的区间是[0,1],这里不绘制边界，同时：
    //α + β + γ = 1 如果 α > 0, β > 0, γ > 0，则自动满足 α < 1, β < 1, γ < 1
    //brute方式，遍历所有像素
    for(float y=ymin; y<=ymax; y++)
    {
        for(float x=xmin; x<=xmax; x++)
        {
            float alpha = ComputeAlpha(x,y);
            float beta = ComputeBeta(x,y);
            float gamma = ComputeGamma(x,y);
            if(alpha > 0 && beta > 0 && gamma > 0)
            {
                // 分别计算RGB分量
                int red = static_cast<int>(alpha * GetRValue(c0) + 
                                        beta * GetRValue(c1) + 
                                        gamma * GetRValue(c2));
                
                int green = static_cast<int>(alpha * GetGValue(c0) + 
                                            beta * GetGValue(c1) + 
                                            gamma * GetGValue(c2));
                
                int blue = static_cast<int>(alpha * GetBValue(c0) + 
                                        beta * GetBValue(c1) + 
                                        gamma * GetBValue(c2));
                
                // 确保颜色值在0-255范围内
                red = max(0, min(red, 255));
                green = max(0, min(green, 255));
                blue = max(0, min(blue, 255));
                
                COLORREF c = RGB(red, green, blue);
                putpixel(x, y, c);
            }

        }
         
    }
    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出结果
    std::cout << "运行时间: " << duration.count() << " 毫秒" << std::endl;
    _getch();
    
    return 0;
}
//计算三个数的最大/小值，mingw -msvcrt不支持c++11写法，无法使用std::max3({triangleA[0], triangleB[0], triangleC[0]})
float max3(float a, float b, float c) {
    float m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}
float min3(float a, float b, float c) {
    float m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}
//计算坐标边界
Bounds Coordinates(vector<float> triangleA, vector<float> triangleB, vector<float> triangleC)
{
    //先比较X
    float xmaxedge = max3(triangleA[0], triangleB[0], triangleC[0]);
    float ymaxedge = max3(triangleA[1], triangleB[1], triangleC[1]);
    float xminedge = min3(triangleA[0], triangleB[0], triangleC[0]);
    float yminedge = min3(triangleA[1], triangleB[1], triangleC[1]);

    return {xminedge,yminedge,xmaxedge,ymaxedge};
}

//计算重心坐标alpha
float ComputeAlpha(float x,float y)
{   
    //三角形PBC面积
    float spbc;
    spbc = (y1 - y2) * x + (x2 - x1) * y + x1 * y2 - x2 * y1;
    //三角形ABC面积
    float sabc;
    sabc = (y1 - y2) * x0 + (x2 - x1) * y0 + x1 * y2 - x2 * y1;
    //利用面积关系计算alpha
    float alpha = spbc / sabc;
    return alpha;
}
//计算重心坐标beta
float ComputeBeta(float x,float y)
{
    //三角形PAC面积
    float spac;
    spac = (y2 - y0) * x + (x0 - x2) * y + x2 * y0 - x0 * y2;
    //三角形ABC面积
    float sabc;
    sabc = (y2 - y0) * x1 + (x0 - x2) * y1 + x2 * y0 - x0 * y2;
    //三角形ABC面积
    //利用面积关系计算alpha
    float beta = spac / sabc;
    return beta;
}
//计算重心坐标Gamma
float ComputeGamma(float x,float y)
{
    //三角形PAB面积
    float spab;
    spab = (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
    //三角形ABC面积
    float sabc;
    sabc = (y0 - y1) * x2 + (x1 - x0) * y2 +  x0 * y1 - x1 * y0;
    //三角形ABC面积
    //利用面积关系计算alpha
    float gamma = spab / sabc;
    return gamma;
}