#include"utils.h"

//���ߺ����ඨ����룬���а����˸���������Ķ���
//��Ӻ���ʱ����utils.h�������������ٵ�utils.cpp����ɺ�������

//���ݶ�ȡ��ʵ��
vector<csvPoint> DataLoader::readCsv(string& filename)
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
        //���뵽vector��
        points.push_back(tpoint);
    }
    csv_data.close();
    return points;
}

//�˲�����ʵ��
//�˲�����
//��ֵ�˲��;�ֵ�˲�
//��ֵ�˲���������������������
//��ֵ�˲����ǡ���ͨ���˲�����һ��
vector<double> Filter::MedFilter(const vector<double>& data, int winsize, int step)
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
vector<double> Filter::MeanFilter(const vector<double>& data, int winsize, int step)
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
vector<double> Filter::GaussFilter(const vector<double>& data, double sigma)
{
    vector<double> filtered_data;
    Mat input = Mat(data);
    Mat output;

    // Ӧ��cv�еĸ�˹�˲�
    GaussianBlur(input, output, Size(0, 0), sigma);

    // �����ת��Ϊһά����
    filtered_data = std::vector<double>(output.begin<double>(), output.end<double>());

    return filtered_data;
}


//������ʵ��
//��������ɢ��ͼ�ͻ���ֱ��ͼ
void Draw::drawScatter(const vector<csvPoint>& points, const string& scatter_title)
{
    //ɢ��ͼͼƬ��С
    int width = 600;
    int height = 600;

    /*int min_value = *min_element(points.begin(), points.end());
    int max_value = *max_element(points.begin(), points.end());*/

    Mat scatter_plot(height, width, CV_8UC3, Scalar(255, 255, 255));

    for (const auto& point : points)
    {
        //ͨ���ҵ������е������Сֵ�������ͼ�߽磨δʵ�֣�����ͼ����������ĸ����޵��ǵ�һ���������½ǵ�����ϵ
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

    //ֱ��ͼ���Ƽ���
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


//������ʵ��
vector<int> Calculate::calHist(const vector<double>& data, double binsize)
{
    //double binsize = 1;
    //����������Сֵ
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

    //���㣬������ѧ��ʽ������ת�������
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
