/**
 * 
 * 2025.10.16
 * 实现冯氏光照
 */
/**
 * 2025.10.14
 * 透视变换
 * 切记GLM是列优先
 * 
*/
#include <PhongShading.h>
#include <TriangleClass2.h>
#include <iostream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Matrixs.h>
glm::mat4 CreateMvpMat(float nx, float ny);
glm::mat4 CreateOrthographicMat(float l, float r,float n,float f,float b,float t);
glm::mat4 CreateCameraTransformMat();
glm::mat4 CreatePMat(float n,float f);
glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f);
glm::mat4 CreateModelMat();
 int main()
 {
    //设置窗口大小
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height);  

    // vector<float> point1 = {100,200}; 
    // vector<float> point2 = {100,600}; 
    // MyLine line1 = MyLine(point1,point2); 
    // line1.DrawLine();
    // 定义顶点类型

//三角面坐标
const std::array<glm::vec4, 36> cubeTriangles = {
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    glm::vec4(-1.0f,  0.05f, -1.0f, 1.0f), // D
    glm::vec4( 1.0f,  0.05f, -1.0f, 1.0f), // C
    
    glm::vec4( 1.0f,  0.05f, -1.0f, 1.0f), // C
    glm::vec4( 1.0f, -0.05f, -1.0f, 1.0f), // B
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    
    // 后面（z=1.0）- 逆时针顺序，法线指向正Z轴
    glm::vec4(-1.0f, -0.05f,  1.0f, 1.0f), // A
    glm::vec4( 1.0f, -0.05f,  1.0f, 1.0f), // B
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    glm::vec4(-1.0f,  0.05f,  1.0f, 1.0f), // D
    glm::vec4(-1.0f, -0.05f,  1.0f, 1.0f), // A
    
    // 左面（x=-1.0）- 逆时针顺序，法线指向负X轴
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    glm::vec4(-1.0f, -0.05f,  1.0f, 1.0f), // B
    glm::vec4(-1.0f,  0.05f,  1.0f, 1.0f), // C
    
    glm::vec4(-1.0f,  0.05f,  1.0f, 1.0f), // C
    glm::vec4(-1.0f,  0.05f, -1.0f, 1.0f), // D
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    
    // 右面（x=1.0）- 逆时针顺序，法线指向正X轴
    glm::vec4( 1.0f, -0.05f, -1.0f, 1.0f), // A
    glm::vec4( 1.0f,  0.05f, -1.0f, 1.0f), // B
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    glm::vec4( 1.0f, -0.05f,  1.0f, 1.0f), // D
    glm::vec4( 1.0f, -0.05f, -1.0f, 1.0f), // A
    
    // 底面（y=-0.05）- 逆时针顺序，法线指向负Y轴
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    glm::vec4( 1.0f, -0.05f, -1.0f, 1.0f), // B
    glm::vec4( 1.0f, -0.05f,  1.0f, 1.0f), // C
    
    glm::vec4( 1.0f, -0.05f,  1.0f, 1.0f), // C
    glm::vec4(-1.0f, -0.05f,  1.0f, 1.0f), // D
    glm::vec4(-1.0f, -0.05f, -1.0f, 1.0f), // A
    
    // 顶面（y=0.05）- 逆时针顺序，法线指向正Y轴
    glm::vec4(-1.0f,  0.05f, -1.0f, 1.0f), // A
    glm::vec4(-1.0f,  0.05f,  1.0f, 1.0f), // B
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    
    glm::vec4( 1.0f,  0.05f,  1.0f, 1.0f), // C
    glm::vec4( 1.0f,  0.05f, -1.0f, 1.0f), // D
    glm::vec4(-1.0f,  0.05f, -1.0f, 1.0f)  // A
};

glm::mat4 M_cam = CreateCameraTransformMat();

// 2. 透视矩阵 P
glm::mat4 P = CreatePMat(-0.8, 1);  

glm::mat4 M_per = CreatePerMat(-1, 1, -1, 1, -0.8, 1);


// 3. 正交投影矩阵
glm::mat4 M_orth = CreateOrthographicMat(-1, 1, -1, 1, -0.8, 1);

// 4. 屏幕映射矩阵  
glm::mat4 M_vp = CreateMvpMat(width, height);

//5.模型矩阵
glm::mat4 M_model = CreateModelMat();
// 6. M = M_vp × M_orth × P × M_cam
glm::mat4 M = M_vp * M_orth * P * M_cam ; 

//光源位置
glm::vec3 lightpos(0.5f, 1.0f, 2.0f);//根据easyx坐标轴特点，y轴坐标反置，或者修改Mvp矩阵为ny/2,
//视角位置
glm::vec3 eyepos = glm::vec3(0.0f, 0.5f, -1.5f);  
// glm::vec3 eyepos = glm::vec3(0.0f, -1.0f, -2.5f); 

glm::vec4 lightpos_s = M * glm::vec4(lightpos,1.0f);
//cout<<lightpos_s.x/lightpos_s.w<<","<<lightpos_s.y/lightpos_s.w;
putpixel(lightpos_s.x/lightpos_s.w,lightpos_s.y/lightpos_s.w,YELLOW);


 //绘制立方体的所有边
for(int i=0; i<cubeTriangles.size(); i+=3)
{
   
     glm::vec4 a = M * cubeTriangles[i];
     glm::vec4 b = M * cubeTriangles[i+1];
     glm::vec4 c = M * cubeTriangles[i+2];
    //  vector<float> point1 = {(a.x/a.w),(a.y/a.w)}; 
    //  vector<float> point2 = {(b.x/b.w),(b.y/b.w)}; 
    //  vector<float> point3 = {(c.x/c.w),(c.y/c.w)}; 
    glm::vec4 point1((a.x/a.w),(a.y/a.w),(a.z/a.w),a.w);
    glm::vec4 point2((b.x/b.w),(b.y/b.w),(b.z/b.w),b.w);
    glm::vec4 point3((c.x/c.w),(c.y/c.w),(c.z/c.w),c.w);
    cout<<point1.x<<","<<point1.y<<endl;
    
    //PhongShading(glm::vec3 A,glm::vec3 B,glm::vec3 C,COLORREF Color,glm::vec3 Lightpos,glm::vec3 eyepos)
    PhongShading triangle = PhongShading(point1,point2,point3,YELLOW,lightpos,eyepos,cubeTriangles[i],cubeTriangles[i+1],cubeTriangles[i+2]);
    PhongShading(-1, 1, -1, 1, 0.1f, 1);
    //MyTriangle triangle = MyTriangle(point1,point2,point3,WHITE);
    triangle.DrawTriangle();
}

_getch();
closegraph();
return 0;
}

