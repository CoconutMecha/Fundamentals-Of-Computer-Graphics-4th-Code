/**
 * 
 * 2025.10.31
 * 实现模型加载
 * 2025-11-21 除贴图外以基本实现
 * 
*/
#include <PhongShading2.h>
#include <TriangleClass2.h>
#include <iostream>
#include <array>
#include <vector>
#include <load_obj.h>
#include <ppm_saver.h>
glm::mat4 CreateMvpMat(float nx, float ny);
glm::mat4 CreateOrthographicMat(float l, float r,float n,float f,float b,float t);
glm::mat4 CreateCameraTransformMat();
glm::mat4 CreatePMat(float n,float f);
glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f);
glm::mat4 CreateModelMat();
Mesh load_obj(const std::string& filename);


void DebugCoordinateSystem()
{
    // 测试一个点在变换后的Z值变化
    glm::vec4 testPoint(0.0f, 0.0f, 1.0f, 1.0f); // Z=1的点
    
    glm::mat4 model = CreateModelMat();
    glm::mat4 view = CreateCameraTransformMat();
    glm::mat4 projection = CreatePerMat(-1, 1, -1, 1, 0.1f, 100.0f);
    
    glm::vec4 clipSpace = projection * view * model * testPoint;
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
    
    std::cout << "测试点变换结果：" << std::endl;
    std::cout << "裁剪空间: (" << clipSpace.x << ", " << clipSpace.y << ", " << clipSpace.z << ", " << clipSpace.w << ")" << std::endl;
    std::cout << "NDC空间: (" << ndc.x << ", " << ndc.y << ", " << ndc.z << ")" << std::endl;
    
    if (ndc.z > 0) {
        std::cout << "Z轴正方向指向屏幕内（右手系）" << std::endl;
    } else {
        std::cout << "Z轴正方向指向屏幕外（左手系）" << std::endl;
    }
}

 int main()
 {

    //DebugCoordinateSystem();
    
    //设置窗口大小
    constexpr int width  = 800; 
    constexpr int height = 600; 
    //创建帧缓冲区
    FrameBuffer externalFrame = FrameBuffer(width,height);
    ZBufferClass externalZBuffer = ZBufferClass(width,height);
    

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




    //加载模型
    Mesh obj_result = load_obj("obj\\untitled.obj");
    int face_num = obj_result.faceCount; //面数
    int vertices_num  = obj_result.vertexCount;  //顶点数
    std::vector<float> vertices = obj_result.vertices; //顶点数组
    std::vector<unsigned int> vertices_indices = obj_result.indices;//顶点索引
    // for (int i = 0; i < 20; i++)
    // {
    //     std::cout << vertices[i] << std::endl;
    // }
    std::cout << vertices.size()<< std::endl;
    std::cout << vertices_indices.size() << std::endl;
    
    glm::mat4 M_cam = CreateCameraTransformMat();

    // 2. 透视矩阵 P
    glm::mat4 P = CreatePMat(0.1, 100);  

    glm::mat4 M_per = CreatePerMat(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);


    // 3. 正交投影矩阵
    glm::mat4 M_orth = CreateOrthographicMat(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);

    // 4. 屏幕映射矩阵  
    glm::mat4 M_vp = CreateMvpMat(width, height);

    //5.模型矩阵
    glm::mat4 M_model = CreateModelMat();
    // 6. M = M_vp × M_orth × P × M_cam
    glm::mat4 M = M_vp * M_per * M_cam * M_model ; 
    

    //光源位置`
    glm::vec3 lightpos(1.0f, 0.8f, 0.0f);
    //视角位置
    glm::vec3 eyepos = glm::vec3(0.0f, 0.2f, -1.0f);  
    // glm::vec3 eyepos = glm::vec3(0.0f, -1.0f, -2.5f); 

    glm::vec4 lightpos_s = M * glm::vec4(lightpos,1.0f);
    //cout<<lightpos_s.x/lightpos_s.w<<","<<lightpos_s.y/lightpos_s.w;

    
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
        int v1_strart = i1*8;
        int v2_strart = i2*8;
        int v3_strart = i3*8;
        
        
      
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


        // glm::vec4 aa = M * cubeTriangles[i];
        // glm::vec4 bb = M * cubeTriangles[i+1];
        // glm::vec4 cc = M * cubeTriangles[i+2];
        // //  vector<float> point1 = {(a.x/a.w),(a.y/a.w)}; 
        // //  vector<float> point2 = {(b.x/b.w),(b.y/b.w)}; 
        // //  vector<float> point3 = {(c.x/c.w),(c.y/c.w)}; 
        // glm::vec4 point11((aa.x/aa.w),(aa.y/a.w),(aa.z/aa.w),aa.w);
        // glm::vec4 point22((bb.x/bb.w),(bb.y/bb.w),(bb.z/bb.w),bb.w);
        // glm::vec4 point33((cc.x/cc.w),(cc.y/cc.w),(cc.z/cc.w),cc.w);
        
        
        //PhongShading(glm::vec3 A,glm::vec3 B,glm::vec3 C,COLORREF Color,glm::vec3 Lightpos,glm::vec3 eyepos)
        PhongShading triangle = PhongShading(point1,point2,point3,glm::vec3(1.0f,1.0f,1.0f),lightpos,eyepos,verticesA,verticesB,verticesC,externalFrame,externalZBuffer,-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        //cout << triangle.tempframebuffersize;
        //MyTriangle triangle = MyTriangle(point1,point2,point3,WHITE);
     
        triangle.DrawTriangle();
        //cout << "out:" <<triangle.framebuffer[800];
        //PPM::Utils::saveRGB8("gradient_red.ppm", triangle.framebuffer, width, height, true);
        
        //cout << triangle.tempframebuffersize;
        //写入最终帧缓冲区
        //cout<< "endendendendend";
    }

    // 一次性保存
    PPM::Utils::saveRGB8("output.ppm", externalFrame.framebuffer, width, height, true);

   
    return 0;
}

