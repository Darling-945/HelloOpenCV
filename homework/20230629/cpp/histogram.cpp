#include<iostream>
#include<math.h>
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
* ȱ�㣺
* ��ͼ�������������Խ��䵥������Ϊһ��.hpp��.cpp�ļ�
*/

//��ֵ�˲�
std::vector<double> medFilter(const std::vector<double>& data, int windowSize, int stepSize) {
    std::vector<double> filteredData;
    int dataSize = data.size();

    for (int i = 0; i < dataSize; i += stepSize) {
        int startIndex = std::max(0, i - windowSize / 2);
        int endIndex = std::min(dataSize - 1, i + windowSize / 2);

        std::vector<double> window(data.begin() + startIndex, data.begin() + endIndex + 1);
        std::sort(window.begin(), window.end());

        double median = (windowSize % 2 == 0) ? (window[windowSize / 2] + window[windowSize / 2 - 1]) / 2.0 : window[windowSize / 2];
        filteredData.push_back(median);
    }

    return filteredData;
}

//��ֵ�˲�
std::vector<double> meanFilter(const std::vector<double>& data, int windowSize, int stepSize) {
    std::vector<double> filteredData;
    int dataSize = data.size();

    for (int i = 0; i < dataSize; i += stepSize) {
        int startIndex = std::max(0, i - windowSize / 2);
        int endIndex = std::min(dataSize - 1, i + windowSize / 2);

        double sum = 0.0;
        int count = 0;
        for (int j = startIndex; j <= endIndex; j++) {
            sum += data[j];
            count++;
        }

        double mean = sum / count;
        filteredData.push_back(mean);
    }

    return filteredData;
}

//����ֱ��ͼ����
void drawhis(const vector<double>& data, const string& title, int winsize, int step) 
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
    vector<double> meddata = medFilter(data, winsize, step);
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
    vector<double> meandata = meanFilter(data, winsize, step);
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
    cv::resizeWindow(title, 600, 200);
    cv::imshow(title, combinedImage);
    cv::waitKey(0);
}

int main() 
{
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
        vector<double> medfildata = medFilter(data, winsize, step);
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
        vector<double> medfildata = meanFilter(data, winsize, step);
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
        drawhis(data, title, winsize, step);
    }

    return 0;
}