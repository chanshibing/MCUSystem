import os
import sys
import random
import math
import numpy as np
from skimage import io, data
import matplotlib
import matplotlib.pyplot as plt
import time

# 根目录
ROOT_DIR = os.path.abspath("../python/")

# Import Mask RCNN
#sys.path.append(ROOT_DIR)  # To find local version of the library
from mrcnn import utils
import mrcnn.model as modellib
from mrcnn import visualize

# Import COCO config
sys.path.append(os.path.join(ROOT_DIR, "coco/"))  # To find local version
import coco

# logs目录
MODEL_DIR = os.path.join(ROOT_DIR, "logs")

# 模型文件目录
COCO_MODEL_PATH = os.path.join(ROOT_DIR, "mask_rcnn_coco_0117.h5")

# 检测图像目录
IMAGE_DIR = os.path.join(ROOT_DIR, "images")


class InferenceConfig(coco.CocoConfig):
    # Set batch size to 1 since we'll be running inference on
    # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1
    NUM_CLASSES = 1 + 2  # COCO has 80 classes
    BACKBONE = "resnet50"


config = InferenceConfig()
config.display()

# Create model object in inference mode.
model = modellib.MaskRCNN(mode="inference", model_dir=MODEL_DIR, config=config)

# Load weights trained on MS-COCO
model.load_weights(COCO_MODEL_PATH, by_name=True)

# COCO Class names
# Index of the class in the list is its ID. For example, to get ID of
# the teddy bear class, use: class_names.index('teddy bear')
class_names = ['BG', 'r', 'chip']

# Load a random image from the images folder
file_names = next(os.walk(IMAGE_DIR))[2]

#print(sys.argv[1])

for i in range(0, 1):
    time_start = time.clock()
	#检测图片
    image = io.imread(os.path.join(IMAGE_DIR, "chip51_00.JPG"))
    # 文件信息读写
    fo = open("log/information.txt", "w+")
    # 裁剪检测
    image_height = image.shape[0]
    image_width = image.shape[1]
    cut_height = 1500
    cut_width = 1500
    x = 0
    y = 0
    out_str = ""
    if (image_height >= cut_height):
        if (image_width >= cut_width):
            while (y < image_height):#列循环
                while (x < image_width):#行循环
                    if (x + cut_width > image_width):#控制行的越界
                        w = image_width - cut_width
                    else:
                        w = x
                    if (y + cut_height > image_height):#控制列越界
                        h = image_height - cut_height
                    else:
                        h = y
                    roi = image[h:h + cut_height, w:w + cut_width, :]#裁剪
                    x = x + cut_width
                    results = model.detect([roi], verbose=1)
                    r = results[0]
                    #生成输出log
                    for j in range(0, r['rois'].shape[0]):#每个检测物体
                        rois = r['rois'][j]
                        #四个坐标
                        out_str = out_str + str(rois[0] + h) + "_"
                        out_str = out_str + str(rois[1] + w) + "_"
                        out_str = out_str + str(rois[2] + h) + "_"
                        out_str = out_str + str(rois[3] + w) + "_"
                        # 类别
                        out_str = out_str + str(r['class_ids'][j]) + "\n"
                    #visualize.display_instances(roi, r['rois'], r['masks'], r['class_ids'],
                     #                           class_names, r['scores'])
                y = y + cut_height
                x = 0
            fo.write(out_str)
	else:
        results = model.detect([roi], verbose=1)
        r = results[0]
        for j in range(0, r['rois'].shape[0]):  # 每个检测物体
            rois = r['rois'][j]
            # 四个坐标
            out_str = out_str + str(rois[0]) + "_"
            out_str = out_str + str(rois[1]) + "_"
            out_str = out_str + str(rois[2]) + "_"
            out_str = out_str + str(rois[3]) + "_"
            # 类别
            out_str = out_str + str(r['class_ids'][j]) + "\n"
        #visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], class_names, r['scores'])
        fo.write(out_str)
    fo.close()