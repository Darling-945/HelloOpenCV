#include<iostream>
#include"utils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<windows.h>
#include<opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

/*
* 计算并绘制直方图
* 业务实现，其中所需的工具函数均在utils.cpp中实现
*/

int main() 
{
    UtilsHistogram hist;
    //读取数据
    string filename = "D:/code/HelloOpenCV/homework/20230629/cpp/data.csv";
    ifstream csv_data(filename);
    if (!csv_data.is_open()) 
    {
        cout << "can't open file: " << endl;
        exit(1);
    }

    vector<vector<double>> columns;
    string line;
    getline(csv_data, line);
    while (getline(csv_data, line)) 
    {
        istringstream iss(line);
        string temp;
        vector<double> row;

        while (getline(iss, temp, ',')) 
        {
            row.push_back(stod(temp));
        }

        columns.push_back(row);
    }

    csv_data.close();

    //输出滤波后数组
    //中值滤波
    int winsize = 3;
    int step = 3;
    cout << "中值滤波" << endl;
    for (int i = 0; i < 2; i++)
    {
        vector<double> data;
        for (const auto& row : columns)
        {
            data.push_back(row[i]);
        }
        vector<double> medfildata = hist.MedFilter(data, winsize, step);
        cout << endl;
        for (const auto& value : medfildata)
        {
            cout << value << " ";
        }
    }

    //均值滤波
    cout << endl;
    cout << "均值滤波" << endl;
    for (int i = 0; i < 2; i++)
    {
        vector<double> data;
        for (const auto& row : columns)
        {
            data.push_back(row[i]);
        }
        vector<double> medfildata = hist.MeanFilter(data, winsize, step);
        cout << endl;
        for (const auto& value : medfildata)
        {
            cout << value << " ";
        }
        cout << endl;
    }
    
    //高斯滤波
    double sigma = 1.0;  //标准差sigma
    cout << "高斯滤波" << endl;
    for (int i = 0; i < 2; i++)
    {
        vector<double> data;
        for (const auto& row : columns)
        {
            data.push_back(row[i]);
        }
        vector<double> medfildata = hist.GaussFilter(data, sigma);
        cout << endl;
        for (const auto& value : medfildata)
        {
            cout << value << " ";
        }
        cout << endl;
    }

    //绘图
    for (int i = 0; i < 2; i++) 
    {
        vector<double> data;
        for (const auto& row : columns) 
        {
            data.push_back(row[i]);
        }

        string title = "Histogram " + to_string(i + 1);
        hist.DrawHis(data, title, winsize, step);
    }

    return 0;
}