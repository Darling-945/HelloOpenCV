#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<windows.h>
#include"rotateutils.h"
#include<opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>

using namespace std;

int main()
{
    //struct csvPoint;
    rotateCal sca2hist;
    string filename = "D:/code/HelloOpenCV/homework/20230629/cpp/data.csv";

    //������ת��
    double rotation_angle;
    //int dis = 1;

    vector<csvPoint> points = sca2hist.readCsv(filename);
    //���Ƴ�ʼɢ��ͼ
    //vector<csvPoint> filppoints = sca2hist.flipDiagonal(points);
    sca2hist.drawScatter(points, "origin_scatter");

    //���Ƴ�ʼֱ��ͼ
    vector<double> x_values, y_values;
    for (const auto& point : points)
    {
        x_values.push_back(point.x);
        y_values.push_back(point.y);
    }
    vector<int> ori_x_hist = sca2hist.calHist(x_values, 1);
    vector<int> ori_y_hist = sca2hist.calHist(y_values, 1);
    sca2hist.drawHist(ori_x_hist, "origin_x_his");
    sca2hist.drawHist(ori_y_hist, "origin_y_his");

    // ��ת
    cout << "input rotation angle: ";
    cin >> rotation_angle;

    vector<csvPoint> rotatedpoints = sca2hist.rotatePoints(points, rotation_angle);

    // ������ת���ͼ��
    sca2hist.drawScatter(rotatedpoints, "rotated scatter");

    // ��������
    vector<double> xvalues, yvalues;
    for (const auto& point : rotatedpoints)
    {
        xvalues.push_back(point.x);
        yvalues.push_back(point.y);
    }

    // ����X��ֱ��ͼ
    vector<int> xhistogram = sca2hist.calHist(xvalues, 0.7);
    sca2hist.drawHist(xhistogram, "rotated_x_His");

    // ����Y��ֱ��ͼ
    vector<int> yhistogram = sca2hist.calHist(yvalues, 0.7);
    sca2hist.drawHist(yhistogram, "rotated_y_His");

    return 0;
}
