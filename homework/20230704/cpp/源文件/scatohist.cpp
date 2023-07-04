#include"utils.h"
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<windows.h>


using namespace std;

//业务测试代码
//检查utils中的所有代码是否均被正确实现
//所有数据加载相关在DataLoader类中
//所有滤波器在Filter类中
//所有绘画函数在Draw类中
//所有计算函数在Calculate类中
int main()
{
    //创建类;
    DataLoader loader;
    //Filter filter;
    Draw draw;
    Calculate cal;

    //数据路径
    string filename = "D:/code/HelloOpenCV/homework/20230629/cpp/data.csv";

    //定义旋转角
    double rotation_angle;
    //int dis = 1;

    vector<csvPoint> points = loader.readCsv(filename);
    //绘制初始散点图
    //vector<csvPoint> filppoints = sca2hist.flipDiagonal(points);
    draw.drawScatter(points, "origin_scatter");

    //绘制初始直方图
    vector<double> x_values, y_values;
    for (const auto& point : points)
    {
        x_values.push_back(point.x);
        y_values.push_back(point.y);
    }
    vector<int> ori_x_hist = cal.calHist(x_values, 1);
    vector<int> ori_y_hist = cal.calHist(y_values, 1);
    draw.drawHist(ori_x_hist, "origin_x_his");
    draw.drawHist(ori_y_hist, "origin_y_his");

    // 旋转
    cout << "input rotation angle: ";
    cin >> rotation_angle;

    vector<csvPoint> rotatedpoints = cal.rotatePoints(points, rotation_angle);

    // 绘制旋转后的图像
    draw.drawScatter(rotatedpoints, "rotated scatter");

    // 处理数据
    vector<double> xvalues, yvalues;
    for (const auto& point : rotatedpoints)
    {
        xvalues.push_back(point.x);
        yvalues.push_back(point.y);
    }

    // 绘制X轴直方图
    vector<int> xhistogram = cal.calHist(xvalues, 0.7);
    draw.drawHist(xhistogram, "rotated_x_His");

    // 绘制Y轴直方图
    vector<int> yhistogram = cal.calHist(yvalues, 0.7);
    draw.drawHist(yhistogram, "rotated_y_His");

    return 0;
}