import ctypes
import os
import csv
import numpy as np
import pandas as pd
import flask
import matplotlib.pyplot as plt
from flask import Flask, render_template, request, send_file
import re
from utils import *

app = Flask(__name__)
savefolder = './static/'


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/upload', methods=['GET', 'POST'])
def result():
    global savefolder
    global ori_scat
    global rotated_scat
    global ori_hist_x
    global ori_hist_y
    global rotated_hist_x
    global rotated_hist_y
    global filted_rotated_hist_x
    global filted_rotated_hist_y

    f = request.files['file']
    angle = request.form['angle']
    angle = float(angle)
    win_size = request.form['step_size']
    step_size = request.form['step_size']
    step_size = int(step_size)
    smoothi_alg = request.form['smoothi_alg']

    # 读取 CSV 文件
    x_values = []
    y_values = []
    data = pd.read_csv(f)

    # 数据处理
    x_values = np.array(data['x'].values)
    y_values = np.array(data['y'].values)

    # 绘制原始散点图
    plt.figure()
    plt.scatter(x_values, y_values, s=10)
    ori_scat = savefolder + "img/ori_scat.jpg"
    plt.savefig(ori_scat)

    # 点的旋转
    rotated_x_values, rotated_y_values = rotate(x_values, y_values, angle)

    # 绘制旋转散点图
    plt.figure()
    plt.scatter(rotated_x_values, rotated_y_values, s=10)
    rotated_scat = savefolder + "img/rotated_scat.jpg"
    fig_sca2 = plt.savefig(rotated_scat)

    # 绘制原始直方图
    plt.figure()
    dis = 1
    group_num = int((max(data.x)-min(data.x))/dis)
    plt.hist(data.x, bins=group_num)
    ori_hist_x = savefolder + "img/ori_hist_x.jpg"
    plt.savefig(ori_hist_x)

    plt.figure()
    dis = 1
    group_num = int((max(data.y)-min(data.y))/dis)
    plt.hist(data.y, bins=group_num)
    ori_hist_y = savefolder + "img/ori_hist_y.jpg"
    plt.savefig(ori_hist_y)

    # 绘制旋转后直方图
    plt.figure(figsize=(10,10))
    dis = 1
    group_num = int((max(rotated_x_values)-min(rotated_x_values))/dis)
    plt.hist(rotated_x_values, bins=group_num)
    rotated_hist_x = savefolder + "img/rotated_hist_x.jpg"
    plt.savefig(rotated_hist_x)

    plt.figure()
    dis = 1
    group_num = int((max(rotated_y_values)-min(rotated_y_values))/dis)
    plt.hist(rotated_y_values, bins=group_num)
    rotated_hist_y = savefolder + "img/rotated_hist_y.jpg"
    plt.savefig(rotated_hist_y)

    if step_size == 0:
        filted_x = rotated_x_values
        filted_y = rotated_y_values
    else:
        filted_x = filter(rotated_x_values, win_size, step_size, smoothi_alg)
        filted_y = filter(rotated_y_values, win_size, step_size, smoothi_alg)

    plt.figure()
    dis = 1
    group_num = int((max(filted_x)-min(filted_x))/dis)
    plt.hist(filted_x, bins=group_num)
    filted_rotated_hist_x = savefolder + "img/filted_rotated_hist_x.jpg"
    plt.savefig(filted_rotated_hist_x)

    plt.figure()
    dis = 1
    group_num = int((max(filted_y)-min(filted_y))/dis)
    plt.hist(filted_y, bins=group_num)
    filted_rotated_hist_y = savefolder + "img/filted_rotated_hist_y.jpg"
    plt.savefig(filted_rotated_hist_y)

    return render_template('result.html', ori_scatter=ori_scat, rotated_scatter=rotated_scat,
                           ori_hist_X=ori_hist_x, ori_hist_Y=ori_hist_y,
                           rotated_hist_X=rotated_hist_x, rotated_hist_Y=rotated_hist_y,
                           filered_hist_X = filted_rotated_hist_x, filered_hist_Y = filted_rotated_hist_y)


if __name__ == '__main__':
    app.debug = True
    port = int(os.environ.get("PORT", 8002))
    app.run(host='0.0.0.0', port=port, debug=True)
