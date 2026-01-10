#include "ppm_saver.h"
#include <vector>

int main() {
    const int width = 800;
    const int height = 600;
    
    // 方法1：使用工具函数一次性保存
    std::vector<uint8_t> frameBuffer(width * height * 3);
    
    // 生成测试数据（红色渐变）
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            frameBuffer[index] = static_cast<uint8_t>(255 * x / width);     // R
            frameBuffer[index + 1] = 0;                                      // G
            frameBuffer[index + 2] = 0;                                      // B
        }
    }
    
    // 一次性保存
    PPM::Utils::saveRGB8("gradient_red.ppm", frameBuffer, width, height, true);
    
    return 0;
}


// 2.浮点颜色数据
//#include "ppm_saver.h"
// #include <vector>
// #include <cmath>

// void saveFloatExample() {
//     const int width = 512;
//     const int height = 512;
    
//     // 浮点帧缓冲区
//     std::vector<float> floatBuffer(width * height * 3);
    
//     // 生成正弦波图案
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             int index = (y * width + x) * 3;
//             float u = static_cast<float>(x) / width;
//             float v = static_cast<float>(y) / height;
            
//             floatBuffer[index] = 0.5f + 0.5f * std::sin(u * 10.0f);     // R
//             floatBuffer[index + 1] = 0.5f + 0.5f * std::cos(v * 10.0f);  // G  
//             floatBuffer[index + 2] = 0.5f + 0.5f * std::sin((u + v) * 5.0f); // B
//         }
//     }
    
//     PPM::Utils::saveRGBFloat("sine_wave.ppm", floatBuffer, width, height);
// }


// 3.多帧保存
// #include "ppm_saver.h"
// #include <vector>
// #include <string>

// class AnimationSaver {
// private:
//     int frame_count_;
    
// public:
//     AnimationSaver() : frame_count_(0) {}
    
//     // 保存一帧
//     void saveFrame(const std::vector<uint8_t>& frame, int width, int height) {
//         std::string filename = "frame_" + std::to_string(frame_count_++) + ".ppm";
//         PPM::Utils::saveRGB8(filename, frame, width, height);
//     }
    
//     // 保存浮点帧
//     void saveFrame(const std::vector<float>& frame, int width, int height) {
//         std::string filename = "frame_" + std::to_string(frame_count_++) + ".ppm";
//         PPM::Utils::saveRGBFloat(filename, frame, width, height);
//     }
// };

// // 使用示例
// void animationExample() {
//     AnimationSaver saver;
//     const int width = 256, height = 256;
    
//     for (int frame = 0; frame < 10; ++frame) {
//         std::vector<uint8_t> buffer(width * height * 3);
        
//         // 生成动画帧
//         for (int i = 0; i < buffer.size(); i += 3) {
//             buffer[i] = static_cast<uint8_t>((frame * 25) % 256);      // R - 动画
//             buffer[i + 1] = 100;                                       // G - 固定
//             buffer[i + 2] = 200;                                       // B - 固定
//         }
        
//         saver.saveFrame(buffer, width, height);
//     }
// }