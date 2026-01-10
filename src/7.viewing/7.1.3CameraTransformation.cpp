/**
 * 2025.10.14
 * 相机变换//有问题已合并修改到7.3
 * 
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
 int main()
 {
    //设置窗口大小
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height);  

    // 定义顶点类型

//立方体顶点
const std::array<glm::vec4, 9> cubeVertices = {
    // 前面（z = 1）
    glm::vec4(200.0f, 400.0f, 300.0f,1.0f), // 左下
    glm::vec4(600.0f, 400.0f, 300.0f,1.0f), // 右下
    glm::vec4(600.0f, 200.0f, 300.0f,1.0f), // 右上
    glm::vec4(200.0f, 200.0f, 300.0f,1.0f), // 左上
    
    // 后面（z = -1）
    glm::vec4(200.0f, 400.0f, -300.0f,1.0f), // 左下
    glm::vec4(600.0f, 400.0f, -300.0f,1.0f), // 右下
    glm::vec4(600.0f, 200.0f, -300.0f,1.0f), // 右上
    glm::vec4(200.0f, 200.0f, -300.0f,1.0f),  // 左上
    glm::vec4(0.0f, 0.0f,0.0f,1.0f)  // 左上
};



glm::mat4 orthmat = CreateOrthographicMat(0, 1000, 0 ,2000, 0 ,1000);//l,r,n,f,b,t
glm::mat4 mvpmat = CreateMvpMat(800,600);
glm::mat4 cammat = CreateCameraTransformMat();
glm::mat4 M = mvpmat * orthmat * cammat;
//glm::mat4 M = mvpmat * orthmat;

/****渲染立方体 */
// 原文所说的ai,bi,这里指立方体的12条边
const std::vector<std::pair<int, int>> cubeEdges = {
    // 前面的四条边
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    // 后面的四条边
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    // 连接前面和后面的四条边
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

//绘制立方体的所有边
for(int i=0; i<cubeEdges.size(); i++)
{
     glm::vec4 p = M * cubeVertices[cubeEdges[i].first];
     glm::vec4 q = M * cubeVertices[cubeEdges[i].second];
     vector<float> point1 = {p.x,p.y}; 
     vector<float> point2 = {q.x,q.y}; 
     //MyLine line = MyLine(point1,point2);   
     //line.DrawLine();
}

_getch();
return 0;
}


/************************必要矩阵  **************************** */
//屏幕坐标矩阵
glm::mat4 CreateMvpMat(float nx, float ny)
{
    glm::mat4 customMatrix(
            nx/2, 0.0f, 0.0f, (nx-1)/2,  // 第一列
            0.0f, ny/2, 0.0f, (ny-1)/2,  // 第二列第二列：y轴取负适配easyx
            0.0f, 0.0f, 1.0f, 0.0f,  // 第三列
            0.0f, 0.0f, 0.0f, 1.0f   // 第四列
        );
    return customMatrix;
}
//正交投影矩阵
glm::mat4 CreateOrthographicMat(float l, float r,float n,float f,float b,float t)
{
    glm::mat4 customMatrix(
            2/(r-l), 0.0f, 0.0f, -1*(r+l)/(r-l),  // 第一列
            0.0f, 2/(t-b), 0.0f, -1*(t+b)/(t-b),  // 第二列
            0.0f, 0.0f, 2/(n-f), -1*(n+f)/(n-f),  // 第三列
            0.0f, 0.0f, 0.0f, 1.0f   // 第四列
        );
    return customMatrix;
}

//相机变化矩阵
glm::mat4 CreateCameraTransformMat()
{
    // 修正相机参数 - 从斜上方观察立方体
    glm::vec3 e = glm::vec3(800.0f, 600.0f, 800.0f);  // 相机在右上方
    glm::vec3 center = glm::vec3(400.0f, 300.0f, 0.0f); // 看向立方体中心
    glm::vec3 t = glm::vec3(0.0f, -1.0f, 0.0f);        // 适配EasyX左上角原点
    
    // 修正：计算正确的视线方向
    glm::vec3 g = glm::normalize(center - e);  // 从相机指向目标点
    
    // 计算u,v,w基向量
    glm::vec3 w = -g;  // 相机坐标系的前向（指向相机后方）
    glm::vec3 u = glm::normalize(glm::cross(t, w));
    glm::vec3 v = glm::cross(w, u);
    
    // 构建相机变换矩阵
    glm::mat4 rotation(
        u.x, u.y, u.z, 0.0f,  // 第一列：右向量
        v.x, v.y, v.z, 0.0f,  // 第二列：上向量
        w.x, w.y, w.z, 0.0f,  // 第三列：前向向量
        0.0f, 0.0f, 0.0f, 1.0f
    );
    
    glm::mat4 translation(
        1.0f, 0.0f, 0.0f, -e.x,
        0.0f, 1.0f, 0.0f, -e.y,
        0.0f, 0.0f, 1.0f, -e.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    
    return rotation * translation;
}