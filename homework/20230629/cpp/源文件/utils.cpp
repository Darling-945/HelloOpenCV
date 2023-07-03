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
* ֱ��ͼ�������ʵ��
* ������
* ����ֱ��ͼ����ֵ����ֵ����˹�˲���
* 
* �����ֶΣ�
* vector��
* ��������������
* �������ɶ������͵�����
* �÷���vector<��������>
*
* ����������
* push_back()
* ����Ԫ�ز���vectorĩβ
*
* �˲�����
* ��ֵ�˲��;�ֵ�˲�
* ��ֵ�˲���������������������
* ��ֵ�˲����ǡ���ͨ���˲�����һ��
*
* �÷���
* int main(){
*   UtilsHistogram X;
*   vector<double> data;
*   int winsize, step;
*   X.MedFilter(const vector<double>& data, int winsize, int step);
* }
*/

//��ֵ�˲�
vector<double> UtilsHistogram::MedFilter(const vector<double>& data, int winsize, int step)
{
    vector<double> filtered_data;
    int data_size = data.size();

    //��ָ�����������˲�
    for (int i = 0; i < data_size; i += step)
    {
        int start_index = max(0, i - winsize / 2);
        int end_index = min(data_size - 1, i + winsize / 2);

        //�˲����Ļ�������
        vector<double> window(data.begin() + start_index, data.begin() + end_index + 1);
        sort(window.begin(), window.end());

        //����ֵ
        double median = (winsize % 2 == 0) ? (window[winsize / 2] + window[winsize / 2 - 1]) / 2.0 : window[winsize / 2];
        filtered_data.push_back(median);
    }

    return filtered_data;
}

//��ֵ�˲�
vector<double> UtilsHistogram::MeanFilter(const vector<double>& data, int winsize, int step)
{
    vector<double> filtered_data;
    int data_size = data.size();

    //��ָ�����������˲�
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

        //���ֵ
        double mean = sum / count;
        filtered_data.push_back(mean);
    }

    return filtered_data;
}

//��˹�˲�
vector<double> UtilsHistogram::GaussFilter(const vector<double>& data, double sigma)
{
    std::vector<double> filtered_data;
    cv::Mat input = cv::Mat(data);
    cv::Mat output;

    // Ӧ��cv�еĸ�˹�˲�
    cv::GaussianBlur(input, output, cv::Size(0, 0), sigma);

    // �����ת��Ϊһά����
    filtered_data = std::vector<double>(output.begin<double>(), output.end<double>());

    return filtered_data;
}

//20230630
//����ֱ��ͼ
//ͳһ�����ڻ���ֱ��ͼ�ĺ���
//��ҵ�������ô������ݺ��������ݽ��д����ٵ��øú�������ֱ��ͼ����
vector<cv::Mat> UtilsHistogram::CalHist(vector<double>& data)
{
    int bins = 256; //��������
    int width = 600;
    int height = 400;
    int binwidth = cvRound((double)width / bins); //���

    cv::Mat histImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    //ԭʼͼ��
    //�ҵ������������Сֵ
    double min_value, max_value;
    cv::minMaxLoc(data, &min_value, &max_value);

    vector<int> hist(bins, 0);
    for (const auto& value : data)
    {
        int bin = cvRound((value - min_value) / (max_value - min_value) * (bins - 1));
        hist[bin]++;
    }

    //�ҵ���������������ֵ
    int max_his_value = *max_element(hist.begin(), hist.end());

    //��ͼ
    for (int i = 0; i < bins; i++)
    {
        int bin_height = cvRound(hist[i] * (double)height / max_his_value);
        cv::rectangle(histImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - bin_height), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    return histImage;
}

//20230629
//���Ʋ���ֱ��ͼ���ߺ���
//����һ�������ԭʼ����ֵ�˲�����ֵ����ͼ�Ա�
//��������
void UtilsHistogram::DrawHis(const vector<double>& data, const string& title, int winsize, int step)
{
    //ֱ��ͼ��������
    int bins = 256; //��������
    int width = 600;
    int height = 400;
    int binwidth = cvRound((double)width / bins); //���

    cv::Mat histImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat medhistImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat meanhistImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    //ԭʼͼ��
    //�ҵ������������Сֵ
    double Minvalue, Maxvalue;
    cv::minMaxLoc(data, &Minvalue, &Maxvalue);

    vector<int> hist(bins, 0);
    for (const auto& value : data)
    {
        int bin = cvRound((value - Minvalue) / (Maxvalue - Minvalue) * (bins - 1));
        hist[bin]++;
    }

    //�ҵ���������������ֵ
    int maxhisvalue = *max_element(hist.begin(), hist.end());

    //��ͼ
    for (int i = 0; i < bins; i++)
    {
        int binheight = cvRound(hist[i] * (double)height / maxhisvalue);
        cv::rectangle(histImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - binheight), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //��ֵ�˲�ͼ��
    vector<double> meddata = MedFilter(data, winsize, step);
    double medMinvalue, medMaxvalue;
    cv::minMaxLoc(meddata, &medMinvalue, &medMaxvalue);

    vector<int> medhist(bins, 0);
    for (const auto& value : meddata)
    {
        int medbin = cvRound((value - medMinvalue) / (medMaxvalue - medMinvalue) * (bins - 1));
        medhist[medbin]++;
    }

    //�ҵ���������������ֵ
    int medMaxhis = *max_element(medhist.begin(), medhist.end());

    //��ͼ
    for (int i = 0; i < bins; i++)
    {
        int medbinH = cvRound(medhist[i] * (double)height / medMaxhis);
        cv::rectangle(medhistImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - medbinH), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //��ֵ�˲�ͼ��
    vector<double> meandata = MeanFilter(data, winsize, step);
    double meanMinvalue, meanMaxvalue;
    cv::minMaxLoc(meandata, &meanMinvalue, &meanMaxvalue);

    vector<int> meanhist(bins, 0);
    for (const auto& value : meandata)
    {
        int meanbin = cvRound((value - meanMinvalue) / (meanMaxvalue - meanMinvalue) * (bins - 1));
        meanhist[meanbin]++;
    }

    //�ҵ���������������ֵ
    int meanMaxhis = *max_element(meanhist.begin(), meanhist.end());

    //��ͼ
    for (int i = 0; i < bins; i++)
    {
        int meanbinH = cvRound(meanhist[i] * (double)height / meanMaxhis);
        cv::rectangle(meanhistImage, cv::Point(i * binwidth, height - 1), cv::Point((i + 1) * binwidth - 1, height - meanbinH), cv::Scalar(255, 190, 0), cv::FILLED);
    }

    //���ɵ�һ�Ż�����
    cv::resize(histImage, histImage, cv::Size(), 0.6, 0.6);
    cv::resize(medhistImage, medhistImage, cv::Size(), 0.6, 0.6);
    cv::resize(meanhistImage, meanhistImage, cv::Size(), 0.6, 0.6);

    // ����һ���հ�ͼ�����ڴ洢ˮƽƴ�Ӻ�Ľ��
    cv::Mat combinedImage(histImage.rows, histImage.cols + medhistImage.cols + meanhistImage.cols, histImage.type());

    // ������ͼƬˮƽƴ��
    cv::Mat roi1(combinedImage, cv::Rect(0, 0, histImage.cols, histImage.rows));
    histImage.copyTo(roi1);
    cv::Mat roi2(combinedImage, cv::Rect(histImage.cols, 0, medhistImage.cols, medhistImage.rows));
    medhistImage.copyTo(roi2);
    cv::Mat roi3(combinedImage, cv::Rect(histImage.cols + medhistImage.cols, 0, meanhistImage.cols, meanhistImage.rows));
    meanhistImage.copyTo(roi3);

    // ����һ����ʾ����
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 700, 300);
    cv::imshow(title, combinedImage);
    cv::waitKey(0);
}
