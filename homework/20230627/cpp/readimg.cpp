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
	*可能会拿到超过屏幕像素比的图像从而导致图像无法在屏幕上显示完全
	* 在这里使用调整窗口的方法来对图像进行缩放
	* 当然也可以用cv中的resize方法
	*/
	// 获取屏幕像素比以缩放图片
	int ScreenHigh = GetSystemMetrics(SM_CYFULLSCREEN);
	int ScreenWeight = GetSystemMetrics(SM_CXFULLSCREEN);
	cout << ScreenHigh << endl;
	cout << ScreenWeight << endl;
	// 获取图片像素比
	int ImgHigh = Img.rows;
	int ImgWeight = Img.cols;

	//优先以高度判断来进行缩放
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