import ctypes
import os
import csv
import numpy as np
import pandas as pd
import flask
import matplotlib.pyplot as plt
from flask import Flask, render_template, request, send_file
import re

app = Flask(__name__)
savefolder = './static'

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


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/upload', methods=['GET', 'POST'])
def result():
    global savefolder
    global ori_scat
    global rotated_scat
    global ori_hist
    global rotated_hist

    f = request.files['file']
    angle = request.form['angle']
    step_size = request.form['step_size']
    smoothing_algorithm = request.form['smoothing_algorithm']

    # 读取 CSV 文件
    x_values = []
    y_values = []
    data = pd.read_csv(f)

    x_values.append(data['x'].values)
    y_values.append(data['y'].values)

    plt.figure(figsize=(10, 10))
    plt.scatter(x_values, y_values, s=10)
    
    plt.savefig()

    return render_template('result.html')


if __name__ == '__main__':
    app.debug = True
    port = int(os.environ.get("PORT", 8001))
    app.run(host='localhost', port=port, debug=True)
