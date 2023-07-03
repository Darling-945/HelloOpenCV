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
* ���㲢����ֱ��ͼ
* ҵ��ʵ�֣���������Ĺ��ߺ�������utils.cpp��ʵ��
*/

int main() 
{
    UtilsHistogram hist;
    //��ȡ����
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

    //����˲�������
    //��ֵ�˲�
    int winsize = 3;
    int step = 3;
    cout << "��ֵ�˲�" << endl;
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

    //��ֵ�˲�
    cout << endl;
    cout << "��ֵ�˲�" << endl;
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
    
    //��˹�˲�
    double sigma = 1.0;  //��׼��sigma
    cout << "��˹�˲�" << endl;
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

    //��ͼ
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