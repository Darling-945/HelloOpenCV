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

using namespace std;
using namespace cv;

/*
* ����ֱ��ͼ������
* ͷ�ļ��а��������������Ա
* ��ͷ�ļ�utils.h��������
* ���еĹ�����utils.cppʵ��
*/

class UtilsHistogram
{
public:
	vector<double> MedFilter(const vector<double>& data, int winsize, int step);
	vector<double> MeanFilter(const vector<double>& data, int winsize, int step);
	vector<double> GaussFilter(const vector<double>& data, double sigma);
	vector<cv::Mat> CalHist(vector<double>& data);
	void DrawHis(const vector<double>& data, const string& title, int winsize, int step);
	

private:
	int step_size;
	int window_size;
	double sigma;
	string title;
	vector<double> data;
};

