#include <LineClass.h>
#include <iostream>
 int main()
 {
    constexpr int width  = 800; 
    constexpr int height = 600; 
    initgraph(width, height); 

    static const vector<float> point1 = {300,387};  
    static const vector<float> point2 = {0,0};

    static const vector<float> pointa = {100,0};  
    static const vector<float> pointb = {200,587};

    MyLine line = MyLine(point1,point2);
    MyLine line1 = MyLine(pointa,pointb);

    line.DrawLine();
    line1.DrawLine();
   
   _getch();
    return 0;
 }