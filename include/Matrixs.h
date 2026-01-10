#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
/************************必要矩阵  **************************** */
//屏幕坐标矩阵 glm是列优先
glm::mat4 CreateMvpMat(float nx, float ny)
{
    glm::mat4 customMatrix(
        nx/2,   0.0f,   0.0f,   0.0f,      // 第一列
        0.0f,   -ny/2,   0.0f,   0.0f,      // 第二列//屏幕坐标y向下，这里可以取负数
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
        0.0f,    0.0f,    2/(f-n), 0.0f,                    // 第三列，书中是n-f这里修改为f-n
        -(r+l)/(r-l), -(t+b)/(t-b), -(n+f)/(f-n), 1.0f      // 第四列,同上
    );
    //return customMatrix;
    return glm::ortho(l, r, b, t, n, f); 
}

//相机变化矩阵
//右手系f<n
glm::mat4 CreateCameraTransformMat()
{

    //glm::vec3 eye = glm::vec3(0.0f, -1.0f, -2.5f);  
    glm::vec3 eye = glm::vec3(0.0f, 0.7f, -1.0f);  
    glm::vec3 center = glm::vec3(0.0f, 0.2f, 1.0f);  
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    
    
    // 视线方向g
    glm::vec3 g = glm::normalize(center - eye);  // 从相机指向目标点
    
    // 计算u,v,w基向量
    glm::vec3 w = g;  // 相机坐标系w轴
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
    
    // return translation * rotation;
    return glm::lookAt(eye, center, up);
}

//半个透视矩阵
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

//完整透视矩阵
glm::mat4 CreatePerMat(float l, float r, float b, float t, float n, float f)
{
   glm::mat4 customMatrix(
        2*n/(r-l),     0.0f,   0.0f,   0.0f,    // 第一列
        0.0f,  2*n/(t-b),      0.0f,   0.0f,    // 第二列
        (l+r)/(l-r),  (b+t)/(b-t),   -(f+n)/(f-n),    1.0f,    // 第三列：书中是n-f 这里修改为-(f-n)
        0.0f,  0.0f,   -2*f*n/(f-n),   0.0f     // 第四列： 这里修改为-2
    );
    //return customMatrix;
    return glm::frustum(l, r, b, t, n, f);

    //  // 计算宽高比
    // float aspect = (r - l) / (t - b);
    
    // // 计算垂直视野角度（弧度）
    // float fovY = 2.0f * atan2(t, n);  // 或者使用 atan2(t-b, 2*n)
    
    // return glm::perspective(fovY, aspect, n, f);
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

