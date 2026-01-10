#include <TriangleClass.h>
#include <iostream>
 int main()
 {
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height);  // 创建 1280x720像素的窗口

    static const vector<float> triangleA = {300,213};  // 上顶点
    static const vector<float> triangleB = {300,387};  // 左下
    static const vector<float> triangleC = {500,387};  // 右下

   // 修正后的顶点顺序（逆时针）
   static const vector<float> triangleA1 = {600,213};  // 上顶点
   static const vector<float> triangleB1 = {700,387};  // 右下
   static const vector<float> triangleC1 = {500,387};  // 左下（原B1和C1交换）
    COLORREF c0 = RGB(255, 0, 0);   // 红色，顶点A
    COLORREF c1 = RGB(255, 0, 0);   // 绿色，顶点B
    COLORREF c2 = RGB(255, 0, 0);   // 蓝色，顶点C
    MyTriangle triangle = MyTriangle(triangleA,triangleB,triangleC,c0,c1,c2);
    MyTriangle triangle1 = MyTriangle(triangleA1,triangleB1,triangleC1,c0,c1,c2);
    triangle.DrawTriangle();
    triangle1.DrawTriangle();
   _getch();
    return 0;
 }