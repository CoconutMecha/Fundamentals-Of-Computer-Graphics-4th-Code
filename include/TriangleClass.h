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

//颜色
// COLORREF c0 = RGB(255, 0, 0);   // 红色，顶点A
// COLORREF c1 = RGB(0, 255, 0);   // 绿色，顶点B
// COLORREF c2 = RGB(0, 0, 255);   // 蓝色，顶点C
class MyTriangle
{
    public:
        struct Bounds {
            float xmin, ymin, xmax, ymax;
        };
        vector<float> triangleA;
        vector<float> triangleB;
        vector<float> triangleC;

        float x0,y0,x1,y1,x2,y2;
        float xmin,ymin,xmax,ymax;
        COLORREF c0,c1,c2;

        //三角形三个顶点以及对应的颜色插值
        MyTriangle(vector<float> A,vector<float> B,vector<float> C,COLORREF C0,COLORREF C1,COLORREF C2)
        {
            triangleA = A;
            triangleB = B;
            triangleC = C;
            //A点坐标
            x0 = A[0];
            y0 = A[1];
            //B点坐标
            x1 = B[0];
            y1 = B[1];
            //C点坐标
            x2 = C[0];
            y2 = C[1];
            //颜色插值
            c0 = C0;
            c1 = C1;
            c2 = C2;
            //坐标范围
            Bounds edge = Coordinates(triangleA,triangleB,triangleC);
            xmin = edge.xmin;
            ymin = edge.ymin;
            xmax = edge.xmax;
            ymax = edge.ymax;

        }

        void DrawTriangle()
        {
            
           

            //std::cout << xmin<<"+"<<xmax<<"+"<<ymin<<"+"<<ymax <<std::endl;
            

            
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
        
      
            
        }

    private:   
        float max3(float a, float b, float c) 
        {
            float m = a;
            if (b > m) m = b;
            if (c > m) m = c;
            return m;
        }
        float min3(float a, float b, float c) 
        {
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



};
