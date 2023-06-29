#include<iostream>
#include<math.h>
#include<windows.h>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat Img = imread("C:/Users/Administrator/Desktop/test.jpg");
	if (Img.data == nullptr)
	{
		cout << "can't open img" << endl;
		return 0;
	}

	/*
	*���ܻ��õ�������Ļ���رȵ�ͼ��Ӷ�����ͼ���޷�����Ļ����ʾ��ȫ
	* ������ʹ�õ������ڵķ�������ͼ���������
	* ��ȻҲ������cv�е�resize����
	*/
	// ��ȡ��Ļ���ر�������ͼƬ
	int ScreenHigh = GetSystemMetrics(SM_CYFULLSCREEN);
	int ScreenWeight = GetSystemMetrics(SM_CXFULLSCREEN);
	cout << ScreenHigh << endl;
	cout << ScreenWeight << endl;
	// ��ȡͼƬ���ر�
	int ImgHigh = Img.rows;
	int ImgWeight = Img.cols;

	//�����Ը߶��ж�����������
	if (ImgHigh > ScreenHigh)
	{
		float orirate = ImgHigh / ImgWeight;
		float rate = ImgHigh / ScreenHigh;
		namedWindow("Img Show", 0);
		resizeWindow("Img Show", ScreenHigh / orirate, ScreenHigh);
	}
	else {
		imshow("Img Show", Img);
	}
	//cv::moveWindow()
	waitKey(0);

}