/**
 * 2025.10.17
 *修改为PPM形式
 2025.11.5 需要把帧缓冲区拆分出来
 2025.11.21 实现p167的算法，使用离屏点处理三角形公共边（不处理公共边渲染结果可能会出现黑线？）
 */
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ZBufferClass.h>
#include <ppm_saver.h>
#include <FrameBufferClass.h>
#include <Matrixs.h>

//#include "tgaimage.h"
using namespace std;

//颜色
// COLORREF c0 = RGB(255, 0, 0);   // 红色，顶点A
// COLORREF c1 = RGB(0, 255, 0);   // 绿色，顶点B
// COLORREF c2 = RGB(0, 0, 255);   // 蓝色，顶点C
class PhongShading
{
    public:
    struct Bounds {
        float xmin, ymin, xmax, ymax;
    };

    // 引用必须在前
    FrameBuffer& frame;
    ZBufferClass& zBuffer;
    
    // 基本数据
    glm::vec4 triangleA, triangleB, triangleC;
    glm::vec4 A3D, B3D, C3D;
    glm::vec3 color, Lightpos, eyepos;
    
    // 依赖项在后
    Bounds edge;
    glm::mat4 MV;
    
    // 其他成员
    int x0,y0,z0,w0,x1,y1,z1,w1,x2,y2,z2,w2;
    float xmin,ymin,xmax,ymax;
    float l,r,b,t,n,f;
    

    PhongShading(glm::vec4 A, glm::vec4 B, glm::vec4 C, 
                 glm::vec3 Color, glm::vec3 Lightpos, glm::vec3 eyepos,
                 glm::vec4 A3D, glm::vec4 B3D, glm::vec4 C3D,
                 FrameBuffer& externalFrame,
                 ZBufferClass& externalZBuffer,
                 float l, float r, float b, float t, float n, float f)
        : frame(externalFrame)
        , zBuffer(externalZBuffer)
        , triangleA(A), triangleB(B), triangleC(C)
        , A3D(A3D), B3D(B3D), C3D(C3D)
        , color(Color), Lightpos(Lightpos), eyepos(eyepos)
        , edge(Coordinates(A, B, C))  // 现在可以在初始化列表初始化！
        , MV(CreateMvpMat(800, 600))
        , l(l), r(r), b(b), t(t), n(n), f(f)
    {
        // 使用参数A,B,C而不是成员变量，避免依赖问题
        this->x0 = A.x; this->y0 = A.y; this->z0 = A.z; this->w0 = A.w;
        this->x1 = B.x; this->y1 = B.y; this->z1 = B.z; this->w1 = B.w;
        this->x2 = C.x; this->y2 = C.y; this->z2 = C.z; this->w2 = C.w;
        
        this->xmin = floor(edge.xmin);
        this->ymin = floor(edge.ymin);
        this->xmax = ceil(edge.xmax);
        this->ymax = ceil(edge.ymax);
    }

        

   

        void DrawTriangle()
        {
                
            for(int y=ymin; y<=ymax; y++)
            {
                for(int x=xmin; x<=xmax; x++)
                {
                    
                    //cout<<"3d:"<<x<<","<<y<<endl;
                    float alpha = ComputeAlpha(x,y);
                    float beta = ComputeBeta(x,y);
                    float gamma = ComputeGamma(x,y);
                    
                    
                    // if(alpha > 0 && beta > 0 && gamma > 0)
                    // {
                    //     float depth = alpha * A3D.z + beta * B3D.z + gamma * C3D.z;
                    //     glm::vec3 P = Compute3D(glm::vec2(x,y));
                    //     // cout << buffer.TestBuffer(x,y,P.z)<<endl;
                    //     // cout<< "endendendendend";
    
                    //     if(zBuffer.TestAndUpdate(x, y, depth))
                    //     {
                    //         //使用BlinnPhong
                    //         glm::vec3 c = ComputeBlinnPhongLight2(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),this->color,P,Lightpos,eyepos);
                            
                    //         //glm::vec3 LightColor,glm::vec3 ObjectColor,glm::vec3 ColorCP,glm::vec3 pixel ,glm::vec3 lightpos,glm::vec3 eyepos)                                      
                    //         //更新2.向帧缓冲区内写入数值
                    //         //UpdatePixelColor(x, y, c);
                    //         frame.WriteinFramebuffer(x,y,c);
                    //         //cout << "color:" << c.x <<","<<c.y<<","<<c.z << endl;
    
                    //     }
                    //     // glm::vec3 c = ComputeBlinnPhongLight(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),this->color,P,Lightpos,eyepos);
                            
                    //     // // //out << "color:" << c.x <<","<<c.y<<","<<c.z << endl;
                    //     // frame.WriteinFramebuffer(x,y,c);

                        
                    // }

                    //公共边处理优化p167
                     if(alpha >= 0 && beta >= 0 && gamma >= 0)
                    {
                        float fa12 = ComputeA12(alpha);
                        float fb20 = ComputeB20(beta);
                        float fc01 = ComputeC01(gamma);
                        if((alpha>0||fa12>0)&&(beta>0||fb20>0)&&(gamma>0||fc01>0))
                        {
                            float depth = alpha * A3D.z + beta * B3D.z + gamma * C3D.z;
                            glm::vec3 P = Compute3D(glm::vec2(x,y));
                            // cout << buffer.TestBuffer(x,y,P.z)<<endl;
                            // cout<< "endendendendend";
        
                            if(zBuffer.TestAndUpdate(x, y, depth))
                            {
                                //使用BlinnPhong
                                glm::vec3 c = ComputeBlinnPhongLight2(glm::vec3(1.0f, 0.6f, 0.2f),glm::vec3(1.0f,1.0f,1.0f),this->color,P,Lightpos,eyepos);
                                
                                //glm::vec3 LightColor,glm::vec3 ObjectColor,glm::vec3 ColorCP,glm::vec3 pixel ,glm::vec3 lightpos,glm::vec3 eyepos)                                      
                                //更新2.向帧缓冲区内写入数值
                                //UpdatePixelColor(x, y, c);
                                frame.WriteinFramebuffer(x,y,c);
                                //cout << "color:" << c.x <<","<<c.y<<","<<c.z << endl;
        
                            }
                            // glm::vec3 c = ComputeBlinnPhongLight(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),this->color,P,Lightpos,eyepos);
                                
                            // // //out << "color:" << c.x <<","<<c.y<<","<<c.z << endl;
                            // frame.WriteinFramebuffer(x,y,c);

                        }
                        

                        
                    }

                }
                
            }  
        }

    private:  

         
        //depth_ndc = alpha * z0 + beta * z1 + gamma * z2
      


        //通过三角形顶点计算三维坐标
        glm::vec3 Compute3D(glm::vec2 pixel)
        {
            //计算三角形内点P的重心坐标
            float x = pixel.x;
            float y = pixel.y;
            float alpha = ComputeAlpha(x,y);
            float beta = ComputeBeta(x,y);
            float gamma = ComputeGamma(x,y);
            //计算三维坐标
            glm::vec3 world;
            world.x = alpha * A3D.x + beta * B3D.x + gamma * C3D.x;
            world.y = alpha * A3D.y + beta * B3D.y + gamma * C3D.y;
            world.z = alpha * A3D.z + beta * B3D.z + gamma * C3D.z;

            //cout<<"3d:"<<world.x<<","<<world.y<<","<<world.z<<endl;
    
            return world;

        }


        glm::vec3 ComputeNDC(glm::vec2 pixel)
        {
            // 计算点P的重心坐标
            float alpha = ComputeAlpha(pixel.x, pixel.y);
            float beta = ComputeBeta(pixel.x, pixel.y);
            float gamma = ComputeGamma(pixel.x, pixel.y);
            
            float x_ndc = (2.0f * pixel.x) / 800.0f - 1.0f;
            float y_ndc = (2.0f * pixel.y) / 600.0f - 1.0f;
            
           
            
            // 深度NDC必须通过插值（使用传入的NDC z坐标）
            float depth_ndc = alpha * z0 + beta * z1 + gamma * z2;
            
            return glm::vec3(x_ndc, y_ndc, depth_ndc);
        }
       
        //计算风氏光照 ：光源位置，像素位置，相机位置
        //phong是局部光照模型，它只考虑物体表面与光源之间的直接关系，而不考虑场景中的其他物体或光线传播路径。
        glm::vec3 ComputePhongLight(glm::vec3 Color ,glm::vec3 pixel ,glm::vec3 lightpos,glm::vec3 eyepos)
        {
            
            glm::vec3 lightDir = glm::normalize(lightpos - pixel);  // 光源方向 l向量
           //glm::vec3 lightDir = lightpos - pixel;
            glm::vec3 viewDir = glm::normalize(eyepos - pixel);     // 视线方向（e向量）
            //法向量 （0，1，0）
            //glm::vec3 n(0.0f,1.0f,0.0f);
            // 计算三角形法向量
            glm::vec3 edge1 = glm::vec3(triangleB - triangleA);
            glm::vec3 edge2 = glm::vec3(triangleC - triangleA);
            /**
             * 默认使用右手坐标系，并且默认情况下，逆时针顺序的三角形表示正面（法线向外）
             * 但是使用easyx，其坐标y轴倒置，所以变成左手坐标系，因此法线方向需要加负号
             */
            glm::vec3 n = -glm::normalize(glm::cross(edge1, edge2));
            //glm::vec3 n = glm::cross(edge1, edge2);
            //cout <<n.x<<","<<n.y<<","<<n.z<<","<<endl;
            //line(n.x,n.y,pixel.x,pixel.y);
           // MyLine Line = MyLine(n,pixel,YELLOW);
            

           
            float r = Color.x;
            float g = Color.y;
            float b = Color.z; 
            glm::vec3 objectColor(r, g, b);


             
        // 光照参数
        float ambientStrength = 0.3f;   // 环境光强度
        float specularStrength = 15.0f;  // 高光强度  
        float shininess = 12.0f;        // 高光指数
        
        // 1. 环境光 (永远存在)
        glm::vec3 ambient = ambientStrength * objectColor;
        
        // 2. 漫反射 (当光线照射到表面时存在)
        float diff = glm::max(glm::dot(n, lightDir), 0.0f);
        glm::vec3 diffuse = diff * objectColor;
        
        // 3. 高光 (只在特定角度可见)
        glm::vec3 reflectDir = glm::reflect(-lightDir, n);
        float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), shininess);
        glm::vec3 specular = specularStrength * spec * glm::vec3(1.0f); // 白色高光
        
        // 合并所有分量
        glm::vec3 result = ambient + diffuse + specular;

        // 确保颜色在有效范围内
        result = glm::clamp(result, 0.0f, 1.0f);
        
        return glm::vec3(
            result.x,
            result.y, 
            result.z
        );

    }

        //计算Blinn-Phong光照模型，参考10.7
        
        glm::vec3 ComputeBlinnPhongLight(glm::vec3 LightColor,glm::vec3 ObjectColor,glm::vec3 ColorCP,glm::vec3 pixel ,glm::vec3 lightpos,glm::vec3 eyepos)
        {
            
            glm::vec3 lightDir = glm::normalize(lightpos - pixel);  // 光源方向 l向量
            glm::vec3 viewDir = glm::normalize(eyepos - pixel);     // 视线方向（e向量）
            glm::vec3 halfwayVector = glm::normalize(lightDir + viewDir); //半角向量

            //法向量
            glm::vec3 edge1 = glm::vec3(triangleB - triangleA);
            glm::vec3 edge2 = glm::vec3(triangleC - triangleA);
            glm::vec3 n = -glm::normalize(glm::cross(edge1, edge2));
            

            // 将 COLORREF 转换为 glm::vec3（颜色范围归一化到 [0, 1]）
            float r = ObjectColor.x;
            float g = ObjectColor.y; 
            float b = ObjectColor.z; 
            //cout << r << "," << g << "," << b << endl;
            glm::vec3 objectColor(r, g, b);


             
        // 光照参数
        float ambientStrength = 0.2f;   // 环境光强度:ca
        float specularStrength = 0.5f;  // 高光强度 
        float shininess = 32.0f;        // 高光指数:p
        glm::vec3 cr = ObjectColor;            //表面颜色：cr
        glm::vec3 cl = LightColor;            //光源颜色：cl
        glm::vec3 cp = ColorCP;            //高光颜色：cp
        
        // 1. 环境光
        glm::vec3 ambient = ambientStrength * objectColor;
        
        // 2. 漫反射
        float diff = glm::max(glm::dot(n, lightDir), 0.0f);
        glm::vec3 diffuse = diff * objectColor;
        
        // 3. 高光
        glm::vec3 reflectDir = glm::reflect(-lightDir, n);
        //float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), shininess);
        glm::vec3 specular = objectColor*(ambientStrength+cl*(glm::max(0.0f,glm::dot(n,lightDir))))+cl * cp*pow(glm::dot(halfwayVector,n),shininess);
        
        // 合并所有分量
        glm::vec3 result = ambient + diffuse + specular;

        // 确保颜色在有效范围内
        result = glm::clamp(result, 0.0f, 1.0f);
        //cout << result.x << "," << result.y << "," << result.z << endl;
        
        return glm::vec3(
            result.x,
            result.y,
            result.z
        );       

    }

    //优化版
    glm::vec3 ComputeBlinnPhongLight2(glm::vec3 LightColor, glm::vec3 ObjectColor, 
                                glm::vec3 ColorCP, glm::vec3 pixel, 
                                    glm::vec3 lightpos, glm::vec3 eyepos)
    {
        // 1. 计算方向向量
        glm::vec3 lightDir = glm::normalize(lightpos - pixel);    // 光源方向
        glm::vec3 viewDir = glm::normalize(eyepos - pixel);      // 视线方向
        glm::vec3 halfwayVector = glm::normalize(lightDir + viewDir); // 半角向量 
        
        // 2. 法向量计算（使用插值法向量，而不是每像素计算）
        // 假设你有插值后的法向量，这里暂时使用面法线
        glm::vec3 edge1 = glm::vec3(triangleB - triangleA);
        glm::vec3 edge2 = glm::vec3(triangleC - triangleA);
        glm::vec3 n = -glm::normalize(glm::cross(edge1, edge2));
        
        // 3. 光照参数
        float ambientStrength = 0.2f;   // 环境光强度
        float specularStrength = 0.6f;  // 高光强度
        float shininess = 316.0f;        // 高光指数
        
        glm::vec3 cl = LightColor;      // 光源颜色
        glm::vec3 cr = ObjectColor;     // 表面颜色
        glm::vec3 cp = ColorCP;         // 高光颜色（通常用白色）
        
        // 4. 环境光分量
        glm::vec3 ambient = ambientStrength * cr;
        
        // 5. 漫反射分量
        float diff = glm::max(glm::dot(n, lightDir), 0.0f);
        glm::vec3 diffuse = diff * cr * cl;
        
        // 6. 高光分量（Blinn-Phong修正版）
        float spec = glm::pow(glm::max(glm::dot(n, halfwayVector), 0.0f), shininess);
        glm::vec3 specular = specularStrength * spec * cp * cl;
        
        // 7. 合并所有分量
        glm::vec3 result = ambient + diffuse + specular;
        
        // 8. 颜色裁剪
        result = glm::clamp(result, 0.0f, 1.0f);
        
        return result;
    }



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
        Bounds Coordinates(glm::vec4 triangleA, glm::vec4 triangleB,glm::vec4 triangleC)
        {
            //先比较X
            float xmaxedge = max3(triangleA.x, triangleB.x, triangleC.x);
            float ymaxedge = max3(triangleA.y, triangleB.y, triangleC.y);
            float xminedge = min3(triangleA.x, triangleB.x, triangleC.x);
            float yminedge = min3(triangleA.y, triangleB.y, triangleC.y);

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
        //参考p167,分别计算faf12，fbf20 ,fcf01
        bool ComputeA12(float alpha)
        {

            //f12(BC)
            // /f(x, y) ≡ (y0 ? y1)x + (x1 ? x0)y + x0y1 ? x1y0 = 0.
    
            float f12 =  (y1 - y2)*-1 + (x2 - x1)*-1 + x1*y2 - x2*y1;
            float fa =   (y1 - y2)*x0 + (x2 - x1)*y0 + x1*y2 - x2*y1;
            return fa * f12 > 0;
        

        }
        bool ComputeB20(float beta)
        {
            float f20 =  (y2 - y0)*-1 + (x0 - x2)*-1 + x2*y0 - x0*y2;
            float fb =   (y2 - y0)*x1 + (x0 - x2)*y1 + x2*y0 - x0*y2;
            return fb * f20 > 0;
            
        }
        bool ComputeC01(float gamma)
        {
            float f01 =  (y0 - y1)*-1 + (x1 - x0)*-1 + x0*y1 - x1*y0;
            float fc =   (y0 - y1)*x2 + (x1 - x0)*y2 + x0*y1 - x1*y0;
            return fc * f01 > 0;
        }



};
