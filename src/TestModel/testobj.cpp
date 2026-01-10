/**
 * 
 * 2025.10.31
 * 实现模型加载
 * 
*/
#include <PhongShading.h>
#include <TriangleClass2.h>
#include <iostream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <load_obj.h>
glm::mat4 CreateMvpMat(float nx, float ny);
glm::mat4 CreateOrthographicMat(float l, float r,float n,float f,float b,float t);
glm::mat4 CreateCameraTransformMat();
glm::mat4 CreatePMat(float n,float f);
glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f);
glm::mat4 CreateModelMat();
Mesh load_obj(const std::string& filename);
 int main()
 {
    
    
    //设置窗口大小
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height);

    //加载模型
    Mesh obj_result = load_obj("obj\\bunny_10k (1).obj");
    int face_num = obj_result.faceCount; //面数
    int vertices_num  = obj_result.vertexCount;  //顶点数
    std::vector<float> vertices = obj_result.vertices; //顶点数组
    std::vector<unsigned int> vertices_indices = obj_result.indices;//顶点索引
    // for (int i = 0; i < vertices.size(); i++)
    // {
    //     std::cout << vertices[i] << std::endl;
    // }

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

    //光源位置`
    glm::vec3 lightpos(0.0f, 2.0f, -0.5f);//根据easyx坐标轴特点，y轴坐标反置，或者修改Mvp矩阵为ny/2,
    //视角位置
    glm::vec3 eyepos = glm::vec3(0.0f, 4.0f, -0.2f);  
    // glm::vec3 eyepos = glm::vec3(0.0f, -1.0f, -2.5f); 

    glm::vec4 lightpos_s = M * glm::vec4(lightpos,1.0f);
    //cout<<lightpos_s.x/lightpos_s.w<<","<<lightpos_s.y/lightpos_s.w;
    putpixel(lightpos_s.x/lightpos_s.w,lightpos_s.y/lightpos_s.w,YELLOW);


    //绘制
    for(int i=0; i<vertices_indices.size(); i+=3)
    {
    
        //vertices转换为齐次坐标Homogeneous Coordinates再计算
        //每个索引三个坐标，每个面三个索引
        //当前面的三个索引依次为
        int i1 = vertices_indices[i];
        int i2 = vertices_indices[i+1];
        int i3 = vertices_indices[i+2];
        //每个索引对应顶点的起始坐标,在读取时已经处理过，不需要减1
        int v1_strart = (vertices_indices[i])*8;
        int v2_strart = (vertices_indices[i+1])*8;
        int v3_strart = (vertices_indices[i+2])*8;
        
      
        glm::vec4 verticesA = glm::vec4(vertices[v1_strart],vertices[v1_strart+1],vertices[v1_strart+2],1.0f);
        //cout<<verticesA.x<<","<<verticesA.y<<","<<verticesA.z<<endl;
        glm::vec4 verticesB = glm::vec4(vertices[v2_strart],vertices[v2_strart+1],vertices[v2_strart+2],1.0f);
        //cout<<verticesB.x<<","<<verticesB.y<<","<<verticesB.z<<endl;
        glm::vec4 verticesC = glm::vec4(vertices[v3_strart],vertices[v3_strart+1],vertices[v3_strart+2],1.0f);
        //cout<<verticesC.x<<","<<verticesC.y<<","<<verticesC.z<<endl;

        glm::vec4 a = M * verticesA;
        glm::vec4 b = M * verticesB;
        glm::vec4 c = M * verticesC;
        //cout<<verticesA.x<<","<<verticesA.y<<","<<verticesA.z<<endl;
      
        glm::vec4 point1((a.x/a.w),(a.y/a.w),(a.z/a.w),a.w);
        glm::vec4 point2((b.x/b.w),(b.y/b.w),(b.z/b.w),b.w);
        glm::vec4 point3((c.x/c.w),(c.y/c.w),(c.z/c.w),c.w);
        
        
        //PhongShading(glm::vec3 A,glm::vec3 B,glm::vec3 C,COLORREF Color,glm::vec3 Lightpos,glm::vec3 eyepos)
        PhongShading triangle = PhongShading(point1,point2,point3,WHITE,lightpos,eyepos,verticesA,verticesB,verticesC);
        PhongShading(-1, 1, -1, 1, 0.1f, 1);
        //MyTriangle triangle = MyTriangle(point1,point2,point3,WHITE);
        triangle.DrawTriangle();
    }

    _getch();
    closegraph();
    return 0;
}


/************************必要矩阵  **************************** */
//屏幕坐标矩阵 glm是列优先
glm::mat4 CreateMvpMat(float nx, float ny)
{
    glm::mat4 customMatrix(
        nx/2,   0.0f,   0.0f,   0.0f,      // 第一列
        0.0f,   -ny/2,   0.0f,   0.0f,      // 第二列//easyx坐标y向下，这里可以取负数
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

    //glm::vec3 eye = glm::vec3(0.0f, -1.0f, -2.5f);  //这里由于easyx的坐标轴下面是正所以-1是向上，也可以修改mvp矩阵为-ny达到商正下负  
    glm::vec3 eye = glm::vec3(0.0f, 2.0f, -0.5f);  
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);  
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //return glm::lookAt(eye, center, up);
    
   // 视线方向g
    glm::vec3 g = glm::normalize(center - eye);  // 从相机指向目标点
    
    // 计算u,v,w基向量
    glm::vec3 w = g;  // 相机坐标系w轴,easyx基于左手坐标系，因此相机变换也要基于左手坐标系不需要取反
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
   glm::mat4 customMatrix(
        2*n/(r-l),     0.0f,   0.0f,   0.0f,    // 第一列
        0.0f,  2*n/(t-b),      0.0f,   0.0f,    // 第二列
        (l+r)/(l-r),  (b+t)/(b-t),   (f+n)/(n-f),    1.0f,    // 第三列：
        0.0f,  0.0f,   2*f*n/(f-n),   0.0f     // 第四列：
    );
    return customMatrix;
}
glm::mat4 CreateModelMat()
{
    // 旋转矩阵测试，转化成齐次型
        float radians = glm::radians(30.0f); 
        float cosa = std::cos(radians);  // float 类型
        float sina = std::sin(radians);  // float 类型

        // 正确的绕 Y 轴旋转矩阵 (4x4)
        glm::mat4 rotatey(
            cosa,  0.0f, -sina, 0.0f,    
            0.0f,  1.0f,  0.0f, 0.0f,    
            sina,  0.0f,  cosa, 0.0f,    
            0.0f,  0.0f,  0.0f, 1.0f     
        );
        return rotatey;
}
