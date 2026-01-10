#ifndef PPM_SAVER_H
#define PPM_SAVER_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <memory>

namespace PPM {

// 支持的像素格式
enum class PixelFormat {
    RGB8,      // 每个通道8位 (0-255)
    RGBFloat,  // 每个通道浮点数 (0.0-1.0)
    RGBA8,     // 带Alpha的RGB (Alpha将被忽略)
    RGBAFloat  // 带Alpha的浮点RGB (Alpha将被忽略)
};

// PPM保存器主类
class PPMSaver {
private:
    int width_, height_;
    PixelFormat format_;
    std::vector<uint8_t> rgb_buffer_;  // 统一存储为RGB8用于输出
    
public:
    // 构造函数
    PPMSaver(int width, int height, PixelFormat format = PixelFormat::RGB8)
        : width_(width), height_(height), format_(format) {
        rgb_buffer_.resize(width * height * 3, 0);
    }
    
    // 从各种格式设置帧缓冲区数据
    bool setFrameBuffer(const uint8_t* data, size_t data_size) {
        if (format_ != PixelFormat::RGB8 && format_ != PixelFormat::RGBA8) {
            return false;
        }
        
        size_t expected_size = width_ * height_ * 
            (format_ == PixelFormat::RGB8 ? 3 : 4);
        if (data_size != expected_size) {
            return false;
        }
        
        if (format_ == PixelFormat::RGB8) {
            std::copy(data, data + data_size, rgb_buffer_.begin());
        } else { // RGBA8 -> 忽略Alpha
            for (size_t i = 0, j = 0; i < data_size; i += 4, j += 3) {
                rgb_buffer_[j] = data[i];     // R
                rgb_buffer_[j + 1] = data[i + 1]; // G
                rgb_buffer_[j + 2] = data[i + 2]; // B
            }
        }
        return true;
    }
    
    bool setFrameBuffer(const float* data, size_t data_size) {
        if (format_ != PixelFormat::RGBFloat && format_ != PixelFormat::RGBAFloat) {
            return false;
        }
        
        size_t expected_elements = width_ * height_ * 
            (format_ == PixelFormat::RGBFloat ? 3 : 4);
        if (data_size != expected_elements) {
            return false;
        }
        
        // 浮点数转uint8_t
        if (format_ == PixelFormat::RGBFloat) {
            for (size_t i = 0; i < data_size; i += 3) {
                rgb_buffer_[i] = floatToByte(data[i]);
                rgb_buffer_[i + 1] = floatToByte(data[i + 1]);
                rgb_buffer_[i + 2] = floatToByte(data[i + 2]);
            }
        } else { // RGBAFloat -> 忽略Alpha
            for (size_t i = 0, j = 0; i < data_size; i += 4, j += 3) {
                rgb_buffer_[j] = floatToByte(data[i]);     // R
                rgb_buffer_[j + 1] = floatToByte(data[i + 1]); // G
                rgb_buffer_[j + 2] = floatToByte(data[i + 2]); // B
            }
        }
        return true;
    }
    
    // 模板函数，支持各种容器
    template<typename Container>
    bool setFrameBuffer(const Container& data) {
        return setFrameBuffer(data.data(), data.size());
    }
    
    // 保存为PPM文件
    bool saveToFile(const std::string& filename, bool binary_format = true) const {
        if (binary_format) {
            return saveP6(filename);
        } else {
            return saveP3(filename);
        }
    }
    
    // 获取图像信息
    int width() const { return width_; }
    int height() const { return height_; }
    PixelFormat format() const { return format_; }
    
    // 批量操作
    void fillColor(uint8_t r, uint8_t g, uint8_t b) {
        for (size_t i = 0; i < rgb_buffer_.size(); i += 3) {
            rgb_buffer_[i] = r;
            rgb_buffer_[i + 1] = g;
            rgb_buffer_[i + 2] = b;
        }
    }
    
    void fillColor(float r, float g, float b) {
        fillColor(floatToByte(r), floatToByte(g), floatToByte(b));
    }

private:
    // P3格式保存（ASCII，易于调试）
    bool saveP3(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        file << "P3\n";
        file << "# Created by PPMSaver\n";
        file << width_ << " " << height_ << "\n255\n";
        
        for (size_t i = 0; i < rgb_buffer_.size(); i += 3) {
            file << static_cast<int>(rgb_buffer_[i]) << " "
                 << static_cast<int>(rgb_buffer_[i + 1]) << " "
                 << static_cast<int>(rgb_buffer_[i + 2]) << "\n";
        }
        
        return true;
    }
    
    // P6格式保存（二进制，文件小）
    bool saveP6(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        
        file << "P6\n";
        file << width_ << " " << height_ << "\n255\n";
        file.write(reinterpret_cast<const char*>(rgb_buffer_.data()), rgb_buffer_.size());
        
        return true;
    }
    
    // 浮点数到字节转换
    static uint8_t floatToByte(float value) {
        return static_cast<uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255);
    }
};

// 工具函数：一次性保存
namespace Utils {
    // 一次性保存RGB8数据
    inline bool saveRGB8(const std::string& filename, 
                        const uint8_t* data, 
                        int width, int height, 
                        bool binary = true) {
        PPMSaver saver(width, height, PixelFormat::RGB8);
        return saver.setFrameBuffer(data, width * height * 3) && 
               saver.saveToFile(filename, binary);
    }
    
    // 一次性保存浮点RGB数据
    inline bool saveRGBFloat(const std::string& filename,
                           const float* data,
                           int width, int height,
                           bool binary = true) {
        PPMSaver saver(width, height, PixelFormat::RGBFloat);
        return saver.setFrameBuffer(data, width * height * 3) && 
               saver.saveToFile(filename, binary);
    }
    
    // 容器版本
    template<typename Container>
    inline bool saveRGB8(const std::string& filename,
                        const Container& data,
                        int width, int height,
                        bool binary = true) {
        return saveRGB8(filename, data.data(), width, height, binary);
    }
    
    template<typename Container>
    inline bool saveRGBFloat(const std::string& filename,
                           const Container& data,
                           int width, int height,
                           bool binary = true) {
        return saveRGBFloat(filename, data.data(), width, height, binary);
    }
}

} // namespace PPM

#endif // PPM_SAVER_H