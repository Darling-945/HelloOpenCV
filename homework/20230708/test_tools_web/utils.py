# 把app.py的工具函数调用重写到这里
import ctypes
import os
import csv
import numpy as np
import pandas as pd

mydll = ctypes.CDLL('D:/code/dllexp.dll')

mydll.MedFilter.restype = ctypes.POINTER(ctypes.c_double)
mydll.MedFilter.argtypes = [ctypes.POINTER(
    ctypes.c_double), ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]

mydll.MeanFilter.restype = ctypes.POINTER(ctypes.c_double)
mydll.MeanFilter.argtypes = [ctypes.POINTER(
    ctypes.c_double), ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]

mydll.RotatePoints.restype = ctypes.POINTER(ctypes.c_double)
mydll.RotatePoints.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double),
                               ctypes.POINTER(ctypes.c_double), ctypes.POINTER(
                                   ctypes.c_double),
                               ctypes.c_int, ctypes.c_double]

mydll.ReleaseMemory.argtypes = [ctypes.POINTER(ctypes.c_double)]


def rotate(x_values, y_values, angle):

    num_points = len(x_values)

    rotated_x_values = (ctypes.c_double * num_points)()
    rotated_y_values = (ctypes.c_double * num_points)()

    # 将输入数据转换为 C 数组类型
    c_x_values = (ctypes.c_double * num_points)(*x_values)
    c_y_values = (ctypes.c_double * num_points)(*y_values)

    mydll.RotatePoints(c_x_values, c_y_values, rotated_x_values,
                       rotated_y_values, num_points, angle)

    rotated_x_values = np.array(rotated_x_values)
    rotated_y_values = np.array(rotated_y_values)

    return rotated_x_values, rotated_y_values

def filter(data, winsize, step, alg):
    if (alg == "mean"):
        data_array = (ctypes.c_double * len(data))(*data)
        filtered_size = ctypes.c_int()
        filtered_data_ptr = mydll.MeanFilter(data_array, len(data), int(winsize), int(step), ctypes.byref(filtered_size))
        filtered_data_mean = [filtered_data_ptr[i] for i in range(filtered_size.value)]
        mydll.ReleaseMemory(filtered_data_ptr) 
        return filtered_data_mean
    
    elif (alg == "median"):
        data_array = (ctypes.c_double * len(data))(*data)
        filtered_size = ctypes.c_int()
        filtered_data_ptr = mydll.MedFilter(data_array, len(data), int(winsize), int(step), ctypes.byref(filtered_size))
        filtered_data_med = [filtered_data_ptr[i] for i in range(filtered_size.value)]
        mydll.ReleaseMemory(filtered_data_ptr)

        return filtered_data_med