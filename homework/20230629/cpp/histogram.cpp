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
#include<opencv2/imgcodecs.hpp>

using namespace std;



void drawhis(const vector<double>& data, const string& title) {
    int bins = 256;
    int histWidth = 600;
    int histHeight = 400;
    int binWidth = cvRound((double)histWidth / bins);

    cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    double minValue, maxValue;
    cv::minMaxLoc(data, &minValue, &maxValue);

    vector<int> hist(bins, 0);
    for (const auto& value : data) {
        int bin = cvRound((value - minValue) / (maxValue - minValue) * (bins - 1));
        hist[bin]++;
    }

    int maxHistValue = *max_element(hist.begin(), hist.end());

    for (int i = 0; i < bins; i++) {
        int binHeight = cvRound(hist[i] * (double)histHeight / maxHistValue);
        cv::rectangle(histImage, cv::Point(i * binWidth, histHeight - 1), cv::Point((i + 1) * binWidth - 1, histHeight - binHeight), cv::Scalar(255, 10, 10), cv::FILLED);
    }

    cv::imshow(title, histImage);
    cv::waitKey(0);
}

int main() {
    string filename = "D:/code/HelloOpenCV/homework/20230629/cpp/data.csv";

    ifstream csv_data(filename);
    if (!csv_data.is_open()) {
        cout << "can't open file: " << endl;
        exit(1);
    }

    vector<vector<double>> columns;
    string line;
    getline(csv_data, line);
    while (getline(csv_data, line)) {
        istringstream iss(line);
        string temp;
        vector<double> row;

        while (getline(iss, temp, ',')) {
            row.push_back(stod(temp));
        }

        columns.push_back(row);
    }

    csv_data.close();

    for (int i = 0; i < 2; i++) {
        vector<double> data;
        for (const auto& row : columns) {
            data.push_back(row[i]);
        }

        string title = "Histogram " + to_string(i + 1);
        drawhis(data, title);
    }

    return 0;
}