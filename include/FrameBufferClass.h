//2025-11-5 帧缓冲区
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ppm_saver.h>
#include <cstdint>
//#include "tgaimage.h"
using namespace std;
class FrameBuffer
{
    public:
        int width;
        int height;
        std::vector<uint8_t> framebuffer;

        FrameBuffer(int width,int height)
        {
           this->width = width;
           this->height = height;
           framebuffer.resize(width * height * 3);
           InitializationFrameBuffer();
        }


    
        //更新3.写入帧缓冲区函数
        void WriteinFramebuffer(int x,int y,glm::vec3 rgbcolor)
        {
            float R = static_cast<uint8_t>(rgbcolor.x * 255) ;
            float G = static_cast<uint8_t>(rgbcolor.y * 255) ;
            float B = static_cast<uint8_t>(rgbcolor.z * 255) ;
            //cout << x << "," << y << "," << R << endl;
            this->framebuffer[(width * y + x) * 3] = R;
            this->framebuffer[((width * y + x) * 3) + 1] = G;
            this->framebuffer[((width * y + x) * 3) + 2] = B;
        }

        //更新4.帧缓冲区初始化
        void InitializationFrameBuffer()
        {
            for (int i = 0; i < this->width*this->height*3; i++)
            {
                this->framebuffer[i] = 0;
            }
            

        }



};