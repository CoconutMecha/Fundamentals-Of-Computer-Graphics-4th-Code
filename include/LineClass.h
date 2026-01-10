
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

class MyLine
{
    public:
        glm::vec3 point1;
        glm::vec3 point2;
        int x0,y0,x1,y1;
        COLORREF Color;
        MyLine(glm::vec3 point1, glm::vec3 point2, COLORREF Color)
        {
            this->point1 = point1;
            this->point2 = point2;
            x0 = this->point1.x;
            y0 = this->point1.y;
            x1 = this->point2.x;
            y1 = this->point2.y;
            this->Color = Color;
           
            //cout << point1.x << "," <<point1.y<<std::endl;
            
        }
        void DrawLine()
        {
            float slope = abs((y1-y0) / (x1-x0));
            //垂直线单独处理，防止分母为0
            if(x0 == x1)
            {
                GenerateLinevertical();
            }
            else if(slope < 1)
            {
                GenerateLine();
            }
            else
            {
                GenerateLineSteep();
            }
            
        }
    private:
        //使用斜率<1的中点算法绘制直线2025.9.25
        void GenerateLine()
        {
            //约束条件
            if(x0 > x1)
            {
                swap(x0, x1);
                swap(y0, y1);
            }
            vector<int> allpos;
           
            int y = y0;
           
            y = y0;
            float d = (y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0; //d是函数值f（x+0.5,y+1）
            for(int x=x0; x<=x1; x++)
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

        // 处理斜率绝对值 > 1 的情况
    void GenerateLineSteep()
    {
        //约束条件y0<y1，确保直线从下到上
        if(y0 > y1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }
        
        int x = x0;
        float slope = static_cast<float>(x1 - x0) / (y1 - y0);
        
        // 调整决策参数计算
        float d = (x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5f) + y0 * x1 - y1 * x0;
        
        for(int y = y0; y <= y1; y++)
        {
            putpixel(x, y, WHITE);
            
            if(d < 0)
            {
                x = x + 1;
                d = d + (y1 - y0) + (x0 - x1);
            }
            else
            {
                d = d + (x0 - x1);
            }
        }
    }
    
        void GenerateLinevertical()
        {
            // 处理竖线情况（x0 == x1）
    
            // 确保y0 <= y1
            if (y0 > y1) {
                swap(y0, y1);
            }
            
            // 绘制竖线
            for (int y = y0; y <= y1; y++) {
                putpixel(x0, y, Color);
            }
            

        }
       

};

