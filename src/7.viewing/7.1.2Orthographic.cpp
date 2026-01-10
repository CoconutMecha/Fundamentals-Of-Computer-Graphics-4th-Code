/**
 * 2025.10.11
 * 正交视图
 * 正视图的正交投影就是一个矩形//有问题已合并修改到7.3
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
    glm::vec4(200.0f, 400.0f, 300.0f,0), // 左下
    glm::vec4(600.0f, 400.0f, 300.0f,0), // 右下
    glm::vec4(600.0f, 200.0f, 300.0f,0), // 右上
    glm::vec4(200.0f, 200.0f, 300.0f,0), // 左上
    
    // 后面（z = -1）
    glm::vec4(200.0f, 400.0f, -300.0f,0), // 左下
    glm::vec4(600.0f, 400.0f, -300.0f,0), // 右下
    glm::vec4(600.0f, 200.0f, -300.0f,0), // 右上
    glm::vec4(200.0f, 200.0f, -300.0f,0),  // 左上
    glm::vec4(0.0f, 0.0f,0.0f,1.0f)  // 左上
};



glm::mat4 orthmat = CreateOrthographicMat(-400,400,1,1000,-300,300);//l,r,n,f,b,t
glm::mat4 mvpmat = CreateMvpMat(800,600);
glm::mat4 M = mvpmat * orthmat;


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
            0.0f, ny/2, 0.0f, (ny-1)/2,  // 第二列
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
