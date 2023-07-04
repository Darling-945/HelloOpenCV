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

//�Ҳ��������
//#ifdef MATHLIBRARY_EXPORTS
//#define MATHLIBRARY_API __declspec(dllexport)
//#else
//#define MATHLIBRARY_API __declspec(dllimport)
//#endif
#define My_API extern "C" __declspec(dllexport)

using namespace std;
using namespace cv;

constexpr auto M_PI = 3.1415926;

//���ߺ�������ͷ�ļ������а�����ҵ������ĸ��ֹ��ߺ���������
//��utils.h��������������utils.cpp�ж��庯��

//���ݶ�ȡ�࣬��Ϊ����
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

//�˲����࣬�����˸����˲���
My_API class Filter
{
public:
	vector<double> MedFilter(const vector<double>& data, int winsize, int step); //��ֵ
	vector<double> MeanFilter(const vector<double>& data, int winsize, int step); //��ֵ
	vector<double> GaussFilter(const vector<double>& data, double sigma); //��˹

};

//�����࣬��������ֱ��ͼ��ɢ��ͼ
My_API class Draw
{
public:
	//����ɢ��ͼ
	void drawScatter(const vector<csvPoint>& points, const string& scatter_title);
	//����ֱ��ͼ
	void drawHist(const vector<int>& histogram, const string& his_title);

};

// �����࣬�������ּ��㹤��
// ��ǰ��ֱ��ͼ�������ת����
My_API class Calculate
{
public:
	vector<int> calHist(const vector<double>& data, double binsize);
	vector<csvPoint> rotatePoints(const vector<csvPoint>& points, double angle);
	vector<csvPoint> flipDiagonal(const vector<csvPoint>& points);

};

