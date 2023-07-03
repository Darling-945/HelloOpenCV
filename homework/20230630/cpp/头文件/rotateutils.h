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

struct csvPoint
{
    double x;
    double y;
    int type;
};

class rotateCal
{
public:

    vector<csvPoint> readCsv(string& filename);
    void drawScatter(const vector<csvPoint>& points, const string& scatter_title);
    vector<csvPoint> rotatePoints(const vector<csvPoint>& points, double angle);
    vector<int> calHist(const vector<double>& data, double binsize);
    void drawHist(const vector<int>& histogram, const string& his_title);
    vector<csvPoint> flipDiagonal(const std::vector<csvPoint>& points);

};