#include<iostream>
#include "utils.h"
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
* 直方图功能类的实现
* 包含：
* 计算直方图，中值、均值及高斯滤波器
* 
* 常见字段：
* vector：
* 容器，向量类型
* 可以容纳多种类型的数据
* 用法：vector<数据类型>
*
* 常见函数：
* push_back()
* 将新元素插入vector末尾
*
* 滤波器：
* 中值滤波和均值滤波
* 中值滤波器用来对噪声进行抑制
* 均值滤波器是“低通”滤波器的一种
*
* 用法：
* int main(){
*   UtilsHistogram X;
*   vector<double> data;
*   int winsize, step;
*   X.MedFilter(const vector<double>& data, int winsize, int step);
* }
*/

//中值滤波
vector<double> UtilsHistogram::MedFilter(const vector<double>& data, int winsize, int step)
{
    vector<double> filtered_data;
    int data_size = data.size();

    //对指定步长进行滤波
    for (int i = 0; i < data_size; i += step)
    {
        int start_index = max(0, i - winsize / 2);
        int end_index = min(data_size - 1, i + winsize / 2);

        //滤波器的滑动窗口
        vector<double> window(data.begin() + start_index, data.begin() + end_index + 1);
        sort(window.begin(), window.end());

        //求中值
        double median = (winsize % 2 == 0) ? (window[winsize / 2] + window[winsize / 2 - 1]) / 2.0 : window[winsize / 2];
        filtered_data.push_back(median);
    }

    return filtered_data;
}

//均值滤波
vector<double> UtilsHistogram::MeanFilter(const vector<double>& data, int winsize, int step)
{
    vector<double> filtered_data;
    int data_size = data.size();

    //对指定步长进行滤波
    for (int i = 0; i < data_size; i += step)
    {
        int start_index = max(0, i - winsize / 2);
        int end_index = min(data_size - 1, i + winsize / 2);

        double sum = 0.0;
        int count = 0;
        for (int j = start_index; j <= end_index; j++) {
            sum += data[j];
            count++;
        }

        //求均值
        double mean = sum / count;
        filtered_data.push_back(mean);
    }

    return filtered_data;
}

//高斯滤波
vector<double> UtilsHistogram::GaussFilter(const vector<double>& data, double sigma)
{
    std::vector<double> filtered_data;
    cv::Mat input = cv::Mat(data);
    cv::Mat output;

    // 应用cv中的高斯滤波
    cv::GaussianBlur(input, output, cv::Size(0, 0), sigma);

    // 将结果转换为一维数组
    filtered_data = std::vector<double>(output.begin<double>(), output.end<double>());

    return filtered_data;
}

//20230630
//计算直方图
//统一的用于绘制直方图的函数
//在业务中先用处理数据函数对数据进行处理，再调用该函数进行直方图绘制
vector<cv::Mat> UtilsHistogram::CalHist(vector<double>& data)
{
    int bins = 256; //柱的数量
    int width = 600;
    int height = 400;
    int binwidth = cvRound((double)width / bins); //组距

    cv::Mat histImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    //原始图像
    //找到数组中最大最小值
    double min_value, max_value;
    cv::minMaxLoc(data, &min_value, &max_value);

    vector<int> hist(bins, 0);
    for (const auto& value : data)
    {
        int bin = cvRound((value - min_value) / (max_value - min_value) * (bins - 1));
        hist[bin]++;
    }

    //找到数组内最大计数的值
    int max_his_value = *max_element(hist.begin(), hist.end());

    //画图
    for (int i = 0; i < bins; i++)
    {
        int bin_height = cvRound(hist[i] * (double)height / max_his_value);
        cv::rectangle(histImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - bin_height), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    return histImage;
}

//20230629
//绘制并列直方图工具函数
//可以一次性输出原始、中值滤波、均值三幅图对比
//可以弃用
void UtilsHistogram::DrawHis(const vector<double>& data, const string& title, int winsize, int step)
{
    //直方图基本参数
    int bins = 256; //柱的数量
    int width = 600;
    int height = 400;
    int binwidth = cvRound((double)width / bins); //组距

    cv::Mat histImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat medhistImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat meanhistImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    //原始图像
    //找到数组中最大最小值
    double Minvalue, Maxvalue;
    cv::minMaxLoc(data, &Minvalue, &Maxvalue);

    vector<int> hist(bins, 0);
    for (const auto& value : data)
    {
        int bin = cvRound((value - Minvalue) / (Maxvalue - Minvalue) * (bins - 1));
        hist[bin]++;
    }

    //找到数组内最大计数的值
    int maxhisvalue = *max_element(hist.begin(), hist.end());

    //画图
    for (int i = 0; i < bins; i++)
    {
        int binheight = cvRound(hist[i] * (double)height / maxhisvalue);
        cv::rectangle(histImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - binheight), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //中值滤波图像
    vector<double> meddata = MedFilter(data, winsize, step);
    double medMinvalue, medMaxvalue;
    cv::minMaxLoc(meddata, &medMinvalue, &medMaxvalue);

    vector<int> medhist(bins, 0);
    for (const auto& value : meddata)
    {
        int medbin = cvRound((value - medMinvalue) / (medMaxvalue - medMinvalue) * (bins - 1));
        medhist[medbin]++;
    }

    //找到数组内最大计数的值
    int medMaxhis = *max_element(medhist.begin(), medhist.end());

    //画图
    for (int i = 0; i < bins; i++)
    {
        int medbinH = cvRound(medhist[i] * (double)height / medMaxhis);
        cv::rectangle(medhistImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - medbinH), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //均值滤波图像
    vector<double> meandata = MeanFilter(data, winsize, step);
    double meanMinvalue, meanMaxvalue;
    cv::minMaxLoc(meandata, &meanMinvalue, &meanMaxvalue);

    vector<int> meanhist(bins, 0);
    for (const auto& value : meandata)
    {
        int meanbin = cvRound((value - meanMinvalue) / (meanMaxvalue - meanMinvalue) * (bins - 1));
        meanhist[meanbin]++;
    }

    //找到数组内最大计数的值
    int meanMaxhis = *max_element(meanhist.begin(), meanhist.end());

    //画图
    for (int i = 0; i < bins; i++)
    {
        int meanbinH = cvRound(meanhist[i] * (double)height / meanMaxhis);
        cv::rectangle(meanhistImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - meanbinH), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //集成到一张画布上
    cv::resize(histImage, histImage, cv::Size(), 0.6, 0.6);
    cv::resize(medhistImage, medhistImage, cv::Size(), 0.6, 0.6);
    cv::resize(meanhistImage, meanhistImage, cv::Size(), 0.6, 0.6);

    // 创建一个空白图像，用于存储水平拼接后的结果
    cv::Mat combinedImage(histImage.rows, histImage.cols + medhistImage.cols + meanhistImage.cols, histImage.type());

    // 将三张图片水平拼接
    cv::Mat roi1(combinedImage, cv::Rect(0, 0, histImage.cols, histImage.rows));
    histImage.copyTo(roi1);
    cv::Mat roi2(combinedImage, cv::Rect(histImage.cols, 0, medhistImage.cols, medhistImage.rows));
    medhistImage.copyTo(roi2);
    cv::Mat roi3(combinedImage, cv::Rect(histImage.cols + medhistImage.cols, 0, meanhistImage.cols, meanhistImage.rows));
    meanhistImage.copyTo(roi3);

    // 创建一个显示窗口
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 700, 300);
    cv::imshow(title, combinedImage);
    cv::waitKey(0);
}
