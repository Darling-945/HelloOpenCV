#pragma once
#include<iostream>
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

//我不会用这个
//#ifdef MATHLIBRARY_EXPORTS
//#define MATHLIBRARY_API __declspec(dllexport)
//#else
//#define MATHLIBRARY_API __declspec(dllimport)
//#endif
#define My_API extern "C" __declspec(dllexport)

using namespace std;
using namespace cv;

constexpr auto M_PI = 3.1415926;

//工具函数声明头文件，其中包含了业务所需的各种工具函数类声明
//在utils.h中声明函数，在utils.cpp中定义函数

//数据读取类，分为两种
My_API struct csvPoint
{
	double x;
	double y;
	int type;
};

My_API class DataLoader
{
public:
	vector<csvPoint> readCsv(string& filename);
};

//滤波器类，包含了各种滤波器
My_API class Filter
{
public:
	vector<double> MedFilter(const vector<double>& data, int winsize, int step); //中值
	vector<double> MeanFilter(const vector<double>& data, int winsize, int step); //均值
	vector<double> GaussFilter(const vector<double>& data, double sigma); //高斯

};

//绘制类，包含绘制直方图和散点图
My_API class Draw
{
public:
	//绘制散点图
	void drawScatter(const vector<csvPoint>& points, const string& scatter_title);
	//绘制直方图
	void drawHist(const vector<int>& histogram, const string& his_title);

};

// 计算类，包含各种计算工具
// 当前有直方图计算和旋转计算
My_API class Calculate
{
public:
	vector<int> calHist(const vector<double>& data, double binsize);
	vector<csvPoint> rotatePoints(const vector<csvPoint>& points, double angle);
	vector<csvPoint> flipDiagonal(const vector<csvPoint>& points);

};

