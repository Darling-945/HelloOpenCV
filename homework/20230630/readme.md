# 20230630更新
20230629文件夹新增*utils.cpp*和*utils.h*文件  
20230630文件夹新增*rotateutils.cpp*、*scatohis.cpp*、*rotateutils.h*文件  
## 对20230629文件夹的更新
将工具函数和业务代码区分开来，新增utils.cpp和utils.h文件，文件内容为20230629的绘制直方图工具函数的整合  
其中histogram.cpp为绘制直方图业务逻辑的代码  
## 20230630作业
### *rotateutils.cpp*
计算旋转直方图的工具函数实现  
主要功能函数如下  
```
//计算直方图函数
vector<int> calHist(const vector<double>& data, double binsize);
{
    return histogram;
}

```
calHist()函数返回经过计算好的直方图数据，再调用drawHist()函数绘制直方图即可
```
// 计算点的旋转函数
vector<csvPoint> rotatePoints(const vector<csvPoint>& points, double angle);
{
    return rotated_points;
}
```
rotatePoints()函数返回经过旋转的点
### *scatohis.cpp*
计算对散点图进行旋转后的直方图的业务逻辑代码  
主要实现了  
*原数据散点图与直方图的显示*  
*旋转之后的散点图与直方图的显示*

### *rotateutils.h*
头文件，声明*rotateutils.cpp*文件中类的成员函数