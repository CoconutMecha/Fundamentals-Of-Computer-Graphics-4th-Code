#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>


// 顶点数据结构
struct Vertex {
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    
    // 用于比较相等性
    bool operator==(const Vertex& other) const {
        return position == other.position && 
               texcoord == other.texcoord && 
               normal == other.normal;
    }
};

// 自定义哈希函数 for Vertex
struct VertexHash {
    std::size_t operator()(const Vertex& vertex) const {
        std::size_t h1 = std::hash<float>{}(vertex.position.x);
        std::size_t h2 = std::hash<float>{}(vertex.position.y);
        std::size_t h3 = std::hash<float>{}(vertex.position.z);
        std::size_t h4 = std::hash<float>{}(vertex.texcoord.x);
        std::size_t h5 = std::hash<float>{}(vertex.texcoord.y);
        std::size_t h6 = std::hash<float>{}(vertex.normal.x);
        std::size_t h7 = std::hash<float>{}(vertex.normal.y);
        std::size_t h8 = std::hash<float>{}(vertex.normal.z);
        
        // 组合哈希值
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ 
               (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^ (h8 << 7);
    }
};

// 网格数据结构
struct Mesh {
    std::vector<float> vertices;         // 交错存储的顶点数据：[位置x, 位置y, 位置z, 纹理坐标u, 纹理坐标v, 法线x, 法线y, 法线z]
    std::vector<unsigned int> indices;   // 索引数组
    int vertexCount;                     // 顶点数量
    int faceCount;                       // 面数量
};


// Mesh load_obj(const std::string& filename);
// int main()
// {
//     Mesh result = load_obj("obj\\test.obj");
//     for (int i = 0; i < result.indices.size(); i++)
//     {
//         std::cout << result.indices[i] << std::endl;
//     }
    
// }


Mesh load_obj(const std::string& filename) {
    Mesh mesh;
    mesh.vertexCount = 0;
    mesh.faceCount = 0;
    
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;
    
    // 使用自定义哈希函数的unordered_map
    std::unordered_map<Vertex, unsigned int, VertexHash> uniqueVertices;
    
    // 打开文件
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return mesh;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {  // 顶点位置
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        else if (prefix == "vt") {  // 纹理坐标
            glm::vec2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            // OBJ纹理坐标v分量通常是从下到上，需要翻转
            texcoord.y = 1.0f - texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        else if (prefix == "vn") {  // 法线向量
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") {  // 面数据
            std::vector<std::string> faceVertices;
            std::string vertexData;
            
            while (iss >> vertexData) {
                faceVertices.push_back(vertexData);
            }
            
            // 将面转换为三角形（支持多边形三角化）
            if (faceVertices.size() >= 3) {
                // 三角化：对于n边形，创建n-2个三角形
                for (size_t i = 1; i < faceVertices.size() - 1; i++) {
                    // 处理面的三个顶点
                    std::vector<std::string> triangleVertices = {
                        faceVertices[0], 
                        faceVertices[i], 
                        faceVertices[i + 1]
                    };
                    
                    for (const auto& vertexStr : triangleVertices) {
                        Vertex vertex;
                        std::istringstream vss(vertexStr);
                        std::string indexStr;
                        std::vector<int> indices;
                        
                        // 解析顶点索引（v/vt/vn格式）
                        while (std::getline(vss, indexStr, '/')) {
                            if (indexStr.empty()) {
                                indices.push_back(-1); // 缺失的索引
                            } else {
                                indices.push_back(std::stoi(indexStr) - 1); // OBJ索引从1开始
                            }
                        }
                        
                        // 设置顶点属性
                        if (indices.size() > 0 && indices[0] >= 0 && indices[0] < temp_positions.size()) {
                            vertex.position = temp_positions[indices[0]];
                        } else {
                            // 如果位置数据缺失，跳过这个顶点
                            continue;
                        }
                        
                        if (indices.size() > 1 && indices[1] >= 0 && indices[1] < temp_texcoords.size()) {
                            vertex.texcoord = temp_texcoords[indices[1]];
                        } else {
                            vertex.texcoord = glm::vec2(0.0f, 0.0f); // 默认纹理坐标
                        }
                        
                        if (indices.size() > 2 && indices[2] >= 0 && indices[2] < temp_normals.size()) {
                            vertex.normal = temp_normals[indices[2]];
                        } else {
                            // 如果没有法线，可以计算面法线或使用默认值
                            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // 默认法线
                        }
                        
                        // 检查顶点是否已存在
                        if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
                            // 新顶点，添加到顶点数组
                            uniqueVertices[vertex] = static_cast<unsigned int>(mesh.vertices.size() / 8);
                            
                            // 交错存储顶点数据：position(3) + texcoord(2) + normal(3) = 8个float
                            mesh.vertices.push_back(vertex.position.x);
                            mesh.vertices.push_back(vertex.position.y);
                            mesh.vertices.push_back(vertex.position.z);
                            mesh.vertices.push_back(vertex.texcoord.x);
                            mesh.vertices.push_back(vertex.texcoord.y);
                            mesh.vertices.push_back(vertex.normal.x);
                            mesh.vertices.push_back(vertex.normal.y);
                            mesh.vertices.push_back(vertex.normal.z);
                        }
                        
                        // 添加索引
                        mesh.indices.push_back(uniqueVertices[vertex]);
                    }
                    
                    mesh.faceCount++;
                }
            }
        }
    }
    
    file.close();
    
    mesh.vertexCount = static_cast<int>(mesh.vertices.size() / 8);
    
    // 输出统计信息
    std::cout << "OBJ文件加载完成: " << filename << std::endl;
    std::cout << "唯一顶点数量: " << mesh.vertexCount << std::endl;
    std::cout << "三角形面数量: " << mesh.faceCount << std::endl;
    std::cout << "索引数量: " << mesh.indices.size() << std::endl;
    std::cout << "顶点数据大小: " << mesh.vertices.size() << " floats" << std::endl;
    
    return mesh;
}