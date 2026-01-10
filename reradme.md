# 1.概述

## 1.1环境

本项目是Fundamentals-Of-Computer-Graphics-4th-CN的算法的实现

使用x86_64-15.2.0-release-win32-seh-msvcrt-rt_v13-rev0和glm

本项目最终采用ppm格式输出

## 1.2文件结构

1.2.1 include

1. glm：向量计算库

2. easyx.h：easyx库，前期调试所用，后期转为输出ppm

3. FrameBufferClass.h：帧缓冲器

4. graphics.h:easyx库，前期调试所用，后期转为输出ppm

5. LineClass.h：直线绘制算法实现

6. load_obj.h：读取obj文件

7. Matrixs.h：全部用到的变换矩阵

8. PhongShading.h：风氏光照实现v1.0,

9. PhongShading2.h：风氏光照实现v2.0

10. ppm_saver.h：输出ppm图片

11. stb_image.h：图片处理库，用于纹理算法，但没实现...

12. TextureClass.h：用于纹理算法，未实现

13. tgaimage.h：输出tga格式

14. TriangleClass.h：三角形图元算法v1.0

15. TriangleClass.h：三角形图元算法v2.0

16. ZBufferClass：深度缓冲算法实现

1.2.2 lib

1.     只有easyx库得编译文件

1.2.3 obj

1.    测试所用模型，bunny是斯坦福兔子

1.2.4 output

1.     输出可执行文件

1.2.5 src

1.    7. viewing：正交变换，相机操作，透视变换的实现，对应原书第四版第7章

2.    8.The Graphics Pipeline：线段绘制，三角图元绘制（暴力形和增量型），三角形边缘处理，对应原书第四版第8章

3. 10.shading：冯氏光照算法，对应原书第10章

4. 其他文件均是测试文件，已弃用

## 



# 


