#include"utils.h"

//工具函数类定义代码，其中包含了各个工具类的定义
//添加函数时请在utils.h中声明函数，再到utils.cpp中完成函数定义

//数据读取类实现
vector<csvPoint> DataLoader::readCsv(string& filename)
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

//滤波器类实现
//滤波器：
//中值滤波和均值滤波
//中值滤波器用来对噪声进行抑制
//均值滤波器是“低通”滤波器的一种
vector<double> Filter::MedFilter(const vector<double>& data, int winsize, int step)
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
vector<double> Filter::MeanFilter(const vector<double>& data, int winsize, int step)
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
vector<double> Filter::GaussFilter(const vector<double>& data, double sigma)
{
    vector<double> filtered_data;
    Mat input = Mat(data);
    Mat output;

    // 应用cv中的高斯滤波
    GaussianBlur(input, output, Size(0, 0), sigma);

    // 将结果转换为一维数组
    filtered_data = std::vector<double>(output.begin<double>(), output.end<double>());

    return filtered_data;
}


//绘制类实现
//包含绘制散点图和绘制直方图
void Draw::drawScatter(const vector<csvPoint>& points, const string& scatter_title)
{
    //散点图图片大小
    int width = 600;
    int height = 600;

    /*int min_value = *min_element(points.begin(), points.end());
    int max_value = *max_element(points.begin(), points.end());*/

    Mat scatter_plot(height, width, CV_8UC3, Scalar(255, 255, 255));

    for (const auto& point : points)
    {
        //通过找到数据中的组大最小值来计算绘图边界（未实现），把图像比作具有四个象限但是第一象限在右下角的坐标系
        //cv::Point pt(static_cast<int>(point.x), static_cast<int>(point.y));
        Point pt(static_cast<double>(point.x + width / 2), static_cast<double>(point.y + width / 2));
        circle(scatter_plot, pt, 1, Scalar(255, 180, 0), -1);
    }

    imshow(scatter_title, scatter_plot);
    waitKey(0);
}

void Draw::drawHist(const vector<int>& histogram, const string& his_title)
{
    int numbins = static_cast<int>(histogram.size());
    int hist_width = 600;
    int hist_height = 400;

    Mat his_image(hist_height, hist_width, CV_8UC3, Scalar(255, 255, 255));

    //直方图绘制计数
    int max_count = *max_element(histogram.begin(), histogram.end());

    int bin_width = static_cast<int>(hist_width / numbins);

    for (int i = 0; i < numbins; i++)
    {
        int bin_height = static_cast<int>(histogram[i] * hist_height / max_count);
        rectangle(his_image, Point(i * bin_width, hist_height - bin_height),
            Point((i + 1) * bin_width - 1, hist_height - 1),
            Scalar(255, 100, 0), FILLED);
    }

    imshow(his_title, his_image);
    waitKey(0);
}


//计算类实现
vector<int> Calculate::calHist(const vector<double>& data, double binsize)
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

vector<csvPoint> Calculate::rotatePoints(const vector<csvPoint>& points, double angle)
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

vector<csvPoint> Calculate::flipDiagonal(const vector<csvPoint>& points)
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
