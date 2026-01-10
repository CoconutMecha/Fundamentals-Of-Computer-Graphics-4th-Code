#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <LineClass.h>
using namespace std;
// class ZBufferClass
// {
//     float defaultZbuffer;
//     float* zbuffer; 
//     int width,height;
    

//     public:
//         ZBufferClass(int width,int height)
//         {
//             this->defaultZbuffer = INFINITE;
//             this->zbuffer = new float[width*height];
//             this->width = width;
//             this->height = height;
//             std::fill_n(zbuffer, width*height, this->defaultZbuffer); // 初始化最远值
//         }

//         void AddZbuffer(int x,int y,float z)
//         {
            
//             zbuffer[y*width + x] = z;
            
//         }
//         bool TestBuffer(int x,int y,float currentZ)
//         {
//             if(currentZ<zbuffer[y*width + x])
//             {
//                 AddZbuffer(x,y,currentZ);
//                 //cout<<true;
//                 return true;
//             }
            
//             else
//             {
//                 //cout<<false;
//                 return false;
//             }


//         }


// };

class ZBufferClass
{
private:
    float* zbuffer;
    int width, height;
    static constexpr float DEFAULT_FAR_VALUE = std::numeric_limits<float>::max();

public:
    ZBufferClass(int width, int height) : width(width), height(height)
    {
        zbuffer = new float[width * height];
        Clear(); // 使用专门的清除函数
    }

    // 析构函数 - 修复内存泄漏
    ~ZBufferClass()
    {
        delete[] zbuffer;
    }

    // 禁止拷贝（或实现深拷贝）
    ZBufferClass(const ZBufferClass&) = delete;
    ZBufferClass& operator=(const ZBufferClass&) = delete;

    // 清除深度缓冲区（初始化为最远值）
    void Clear()
    {
        std::fill_n(zbuffer, width * height, DEFAULT_FAR_VALUE);
    }

    // 边界检查函数
    bool IsValidCoord(int x, int y) const
    {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // 修正的深度测试逻辑
    bool TestAndUpdate(int x, int y, float currentZ)
    {
        if (!IsValidCoord(x, y)) return false;
        
        int index = y * width + x;
        
        if (currentZ < zbuffer[index]) 
        {
             // 通过测试，更新深度值
            zbuffer[index] = currentZ;
            return true;
            
        }
        return false; // 当前片段被遮挡
        
       
    }

    // 获取深度值（用于调试）
    float GetDepth(int x, int y) const
    {
        if (!IsValidCoord(x, y)) return DEFAULT_FAR_VALUE;
        return zbuffer[y * width + x];
    }
};
