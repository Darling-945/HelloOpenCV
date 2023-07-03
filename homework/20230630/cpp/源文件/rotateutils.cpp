#include<iostream>
#include"rotateutils.h"
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
constexpr auto M_PI = 3.1415926;

// 计算直方图类的实现
//通过对散点图进行旋转来得到直方图
//本质是数据的旋转
//但也期望尝试通过识别旋转后的散点图来得到直方图的思路（未实现）
//难点：旋转之后的散点图边界框
//     直方图的计算

//设置容器vector，将文件中的数据传入
vector<csvPoint> rotateCal::readCsv(string& filename)
{
    //定义points的vector，意义为点的集合，也是最后返回的结果
    vector<csvPoint> points;
    ifstream csv_data(filename);

    if (!csv_data.is_open())
    {
        cout << "can't open file" << endl;
        exit(1);
    }

    //遍历文件
    string line;
    getline(csv_data, line);
    while (getline(csv_data, line))
    {
        istringstream iss(line);
        string temp;
        csvPoint tpoint;
        int col = 0;

        while (getline(iss, temp, ','))
        {
            if (col == 0)
            {
                tpoint.x = stoi(temp);
            }
            else if (col == 1)
            {
                tpoint.y = stoi(temp);
            }
            else
            {
                tpoint.type = stoi(temp);
            }
            col++;
        }
        //加入到vector中
        points.push_back(tpoint);
    }
    csv_data.close();
    return points;
}

// 绘制散点图并显示
void rotateCal::drawScatter(const vector<csvPoint>& points, const string& scatter_title)
{
    //散点图图片大小
    int width = 600;
    int height = 600;

    /*int min_value = *min_element(points.begin(), points.end());
    int max_value = *max_element(points.begin(), points.end());*/

    cv::Mat scatter_plot(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    for (const auto& point : points)
    {
        //通过找到数据中的组大最小值来计算绘图边界（未实现），把图像比作具有四个象限但是第一象限在右下角的坐标系
        //cv::Point pt(static_cast<int>(point.x), static_cast<int>(point.y));
        cv::Point pt(static_cast<double>(point.x + width / 2), static_cast<double>(point.y + width / 2));
        cv::circle(scatter_plot, pt, 1, cv::Scalar(255, 180, 0), -1);
    }

    cv::imshow(scatter_title, scatter_plot);
    cv::waitKey(0);
}

//对数据进行旋转
vector<csvPoint> rotateCal::rotatePoints(const vector<csvPoint>& points, double angle)
{
    vector<csvPoint> rotated_points;

    double radians = angle * M_PI / 180.0;
    double cos_angle = cos(radians);
    double sin_angle = sin(radians);

    //计算，根据数学公式计算旋转后的坐标
    for (const auto& point : points)
    {
        csvPoint rotated_point;
        rotated_point.x = point.x * cos_angle - point.y * sin_angle;
        rotated_point.y = point.x * sin_angle + point.y * cos_angle;
        rotated_point.type = point.type;
        rotated_points.push_back(rotated_point);
    }

    return rotated_points;
}

/*
* 直方图绘制难点，要想数据展示效果平滑
* 组距，也就是步长可以人为改变，但是组数也就是bins必须是自适应
* 这样才能保证调整步长时，直方图不会出现锯齿
*/

//计算直方图
vector<int> rotateCal::calHist(const vector<double>& data ,double binsize)
{
    //double binsize = 1;
    //计算最大和最小值
    double min_value = *min_element(data.begin(), data.end());
    double max_value = *max_element(data.begin(), data.end());

    int numbins = static_cast<int>(ceil((max_value - min_value) / binsize));

    vector<int> histogram(numbins, 0);

    for (const auto& value : data) 
    {
        int binIndex = static_cast<int>((value - min_value) / binsize);
        histogram[binIndex]++;
    }

    return histogram;
}

//直方图绘制
void rotateCal::drawHist(const vector<int>&histogram, const string & his_title)
{
    int numbins = static_cast<int>(histogram.size());
    int hist_width = 600;
    int hist_height = 400;

    cv::Mat his_image(hist_height, hist_width, CV_8UC3, cv::Scalar(255, 255, 255));

    //直方图绘制计数
    int max_count = *max_element(histogram.begin(), histogram.end());

    int bin_width = static_cast<int>(hist_width / numbins);

    for (int i = 0; i < numbins; i++)
    {
        int bin_height = static_cast<int>(histogram[i] * hist_height / max_count);
        cv::rectangle(his_image, cv::Point(i * bin_width, hist_height - bin_height),
            cv::Point((i + 1) * bin_width-1, hist_height - 1),
            cv::Scalar(255, 100, 0), cv::FILLED);
    }

    cv::imshow(his_title, his_image);
    cv::waitKey(0);
}

vector<csvPoint> rotateCal::flipDiagonal(const vector<csvPoint>& points) 
{
    vector<csvPoint> flippedPoints;

    for (const auto& point : points) 
    {
        csvPoint flippedPoint;
        flippedPoint.x = point.x;
        flippedPoint.y = -(point.y);
        flippedPoint.type = point.type;

        flippedPoints.push_back(flippedPoint);
    }

    return flippedPoints;
}
