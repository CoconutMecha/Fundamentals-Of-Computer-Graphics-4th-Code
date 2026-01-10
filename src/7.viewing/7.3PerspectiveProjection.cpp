/**
 * 2025.10.14
 * 透视变换
 * 切记GLM是列优先
 * 
*/
#include <LineClass.h>
#include <iostream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 CreateMvpMat(float nx, float ny);
glm::mat4 CreateOrthographicMat(float l, float r,float n,float f,float b,float t);
glm::mat4 CreateCameraTransformMat();
glm::mat4 CreatePMat(float n,float f);
glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f);
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

//立方体顶点
const std::array<glm::vec4, 8> cubeVertices = {
    // 前面四个顶点（z=10）
    glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), 
    glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f), 
    glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f), 
    glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), 
    
    // 后面四个顶点（z=20）
    glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),
    glm::vec4(0.5f, -0.5f, 0.5f, 1.0f), 
    glm::vec4(0.5f, 0.5f, -0.5f, 1.0f), 
    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)  
};




glm::mat4 M_cam = CreateCameraTransformMat();

// 2. 透视矩阵 P
glm::mat4 P = CreatePMat(-1.0f, 1.0f);  

glm::mat4 M_per = CreatePerMat(-1, 1, -1, 1, -1, 1);


// 3. 正交投影矩阵
glm::mat4 M_orth = CreateOrthographicMat(-1, 1, -1, 1, -1, 1);

// 4. 屏幕映射矩阵  
glm::mat4 M_vp = CreateMvpMat(width, height);

// 5. M = M_vp × M_orth × P × M_cam
glm::mat4 M = M_vp * M_orth * P * M_cam; 


       

// 原文所说的ai,bi,这里指立方体的12条边
const std::vector<std::pair<int, int>> cubeEdges = {
    // 底面 (y = -0.5)
    {0, 1}, {1, 5}, {5, 4}, {4, 0},
    
    // 顶面 (y = 0.5)
    {2, 3}, {3, 7}, {7, 6}, {6, 2},
    
    // 垂直边 (连接底面和顶面)
    {0, 2}, {1, 3}, {4, 6}, {5, 7}
};
 cout << cubeVertices[cubeEdges[0].first].w<<","<< cubeVertices[cubeEdges[0].first].x<<","<<  cubeVertices[cubeEdges[0].first].y<<endl;
  cout <<  cubeVertices[cubeEdges[0].first].w<<","<<cubeVertices[cubeEdges[0].second].x<<","<<  cubeVertices[cubeEdges[1].second].y<<endl;
//绘制立方体的所有边
for(int i=0; i<cubeEdges.size(); i++)
{
   
     glm::vec4 p = M * cubeVertices[cubeEdges[i].first];
     glm::vec4 q = M * cubeVertices[cubeEdges[i].second];
     vector<float> point1 = {(p.x/p.w),(p.y/p.w)}; 
     vector<float> point2 = {(q.x/q.w),(q.y/q.w)}; 
    // MyLine line = MyLine(point1,point2);   
    // line.DrawLine();//我的中点算法存在问题，这里使用easyx的替代
    line((p.x/p.w),(p.y/p.w),(q.x/q.w),(q.y/q.w));
}

_getch();
return 0;
}


/************************必要矩阵  **************************** */
//屏幕坐标矩阵 glm是列优先
glm::mat4 CreateMvpMat(float nx, float ny)
{
    glm::mat4 customMatrix(
        nx/2,   0.0f,   0.0f,   0.0f,      // 第一列
        0.0f,   ny/2,   0.0f,   0.0f,      // 第二列
        0.0f,   0.0f,   1.0f,   0.0f,      // 第三列
        (nx-1)/2, (ny-1)/2, 0.0f, 1.0f     // 第四列
    );
    return customMatrix;
}
//正交投影矩阵
glm::mat4 CreateOrthographicMat(float l, float r,float b,float t,float n,float f)
{
    glm::mat4 customMatrix(
        2/(r-l), 0.0f,    0.0f,    0.0f,                    // 第一列
        0.0f,    2/(t-b), 0.0f,    0.0f,                    // 第二列
        0.0f,    0.0f,    2/(n-f), 0.0f,                    // 第三列
        -(r+l)/(r-l), -(t+b)/(t-b), -(n+f)/(n-f), 1.0f      // 第四列
    );
    return customMatrix;
}

//相机变化矩阵
glm::mat4 CreateCameraTransformMat()
{

    glm::vec3 eye = glm::vec3(0.0f, -1.0f, -2.0f);  //这里由于easyx的坐标轴下面是正所以-1是向上，也可以修改mvp矩阵为-ny达到商正下负  
    glm::vec3 center = glm::vec3(0.5f, 0.5f, 15.0f);  
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //return glm::lookAt(eye, center, up);
    
    // 视线方向g
    glm::vec3 g = glm::normalize(center - eye);  // 从相机指向目标点
    
    // 计算u,v,w基向量
    glm::vec3 w = -g;  // 相机坐标系w轴
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(w, u);
    
    // 旋转矩阵（列优先）
    glm::mat4 rotation(
        u.x, v.x, w.x, 0.0f,  // 第一列
        u.y, v.y, w.y, 0.0f,  // 第二列
        u.z, v.z, w.z, 0.0f,  // 第三列
        0.0f, 0.0f, 0.0f, 1.0f  // 第四列
    );
    
    // 平移矩阵（列优先）
    glm::mat4 translation(
        1.0f, 0.0f, 0.0f, 0.0f,     // 第一列
        0.0f, 1.0f, 0.0f, 0.0f,     // 第二列
        0.0f, 0.0f, 1.0f, 0.0f,     // 第三列
        -eye.x, -eye.y, -eye.z, 1.0f      // 第四列
    );
    
    return rotation * translation;
}

//透视矩阵
glm::mat4 CreatePMat(float n,float f)
{
    glm::mat4 customMatrix(
        n,     0.0f,   0.0f,   0.0f,    // 第一列
        0.0f,  n,      0.0f,   0.0f,    // 第二列
        0.0f,  0.0f,   n+f,    1.0f,    // 第三列：
        0.0f,  0.0f,   -f*n,   0.0f     // 第四列：
    );
    return customMatrix;
}

glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 perMat = glm::mat4(0.0f);
    
    // 第一行
    perMat[0][0] = (2.0f * n) / (r - l);
    perMat[0][2] = (l + r) / (l - r);
    
    // 第二行  
    perMat[1][1] = (2.0f * n) / (t - b);
    perMat[1][2] = (b + t) / (b - t);
    
    // 第三行
    perMat[2][2] = (f + n) / (n - f);
    perMat[2][3] = (2.0f * f * n) / (f - n);
    
    // 第四行
    perMat[3][2] = 1.0f;
    
    return perMat;
}
