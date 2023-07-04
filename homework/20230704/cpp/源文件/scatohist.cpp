#include"utils.h"
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<windows.h>


using namespace std;

//ҵ����Դ���
//���utils�е����д����Ƿ������ȷʵ��
//�������ݼ��������DataLoader����
//�����˲�����Filter����
//���л滭������Draw����
//���м��㺯����Calculate����
int main()
{
    //������;
    DataLoader loader;
    //Filter filter;
    Draw draw;
    Calculate cal;

    //����·��
    string filename = "D:/code/HelloOpenCV/homework/20230629/cpp/data.csv";

    //������ת��
    double rotation_angle;
    //int dis = 1;

    vector<csvPoint> points = loader.readCsv(filename);
    //���Ƴ�ʼɢ��ͼ
    //vector<csvPoint> filppoints = sca2hist.flipDiagonal(points);
    draw.drawScatter(points, "origin_scatter");

    //���Ƴ�ʼֱ��ͼ
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

    // ��ת
    cout << "input rotation angle: ";
    cin >> rotation_angle;

    vector<csvPoint> rotatedpoints = cal.rotatePoints(points, rotation_angle);

    // ������ת���ͼ��
    draw.drawScatter(rotatedpoints, "rotated scatter");

    // ��������
    vector<double> xvalues, yvalues;
    for (const auto& point : rotatedpoints)
    {
        xvalues.push_back(point.x);
        yvalues.push_back(point.y);
    }

    // ����X��ֱ��ͼ
    vector<int> xhistogram = cal.calHist(xvalues, 0.7);
    draw.drawHist(xhistogram, "rotated_x_His");

    // ����Y��ֱ��ͼ
    vector<int> yhistogram = cal.calHist(yvalues, 0.7);
    draw.drawHist(yhistogram, "rotated_y_His");

    return 0;
}