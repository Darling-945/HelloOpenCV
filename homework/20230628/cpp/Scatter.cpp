#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//���嵥����Ľṹ��
struct csvPoint
{
    int x;
    int y;
    int type;
};

//��������vector�����ļ��е����ݴ���
vector<csvPoint> readcsv(string& filename)
{
    //����points��vector������Ϊ��ļ��ϣ�Ҳ����󷵻صĽ��
    vector<csvPoint> points;
    ifstream csv_data(filename);

    if (!csv_data.is_open())
    {
        cout << "can't open file" << endl;
        exit(1);
    }

    //�����ļ�
    string line;
    getline(csv_data, line);
    while (getline(csv_data, line))
    {
        istringstream is(line);
        string temp;
        csvPoint tpoint;
        int col = 0;

        while (getline(is, temp, ','))
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
        //���뵽vector��
        points.push_back(tpoint);
    }
    csv_data.close();
    return points;
}

//��ͼ����
void plot(const vector<csvPoint>& points)
{
    int width = 600;
    int height = 600;

    //���廭��
    Mat canvas(height, width, CV_8UC3, Scalar(255, 255, 255));

    for (const auto& point : points) {
        //ǿ������ת������ת�޷����
        Point2i pt(static_cast<int>(point.x), static_cast<int>(point.y));
        Scalar color;

        //��Բ�ͬ��𻭳���ͬ��ɫ
        switch (point.type) {
        case 3:
            color = Scalar(255, 0, 0);
            break;
        case 2:
            color = Scalar(0, 255, 0);
            break;
        case 1:
            color = Scalar(0, 0, 255);
            break;
        case 0:
            color = Scalar(0, 224, 255);
            break;
        case 5:
            color = Scalar(224, 224, 255);
            break;
        case 9:
            color = Scalar(224, 0, 255);
            break;
        case 6:
            color = Scalar(255, 225, 0);
            break;
        case 14:
            color = Scalar(180, 180, 225);
            break;
        }
        circle(canvas, pt, 1, color, -1);
    }
    cv::imshow("Scatter Plot", canvas);
    cv::waitKey(0);
}

int main()
{
    string filename = "D:/code/dataanls/data.csv";
    vector<csvPoint>points =  readcsv(filename);
    /*for (int i = 0; i < points.size(); ++i)
    {
        std::cout << points[i].type << ' ';
    }*/
    plot(points);

}