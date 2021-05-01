"""Compute depth maps for images in the input folder.
"""
import sys
import re
import os
import glob
import torch
import cv2
import argparse
import util.io
import scipy.misc
import numpy as np

import socket


import io
import jetson.inference
import jetson.utils
import base64

from torchvision.transforms import Compose

from dpt.models import DPTDepthModel
from dpt.midas_net import MidasNet_large
from dpt.transforms import Resize, NormalizeImage, PrepareForNet

from util.misc import visualize_attention

def run(input_path, output_path, model_path, model_type="dpt_hybrid", optimize=True):
    """Run MonoDepthNN to compute depth maps.

    Args:
        input_path (str): path to input folder
        output_path (str): path to output folder
        model_path (str): path to saved model
    """
    print("initialize")

    # select device
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("device: %s" % device)

    # load network
    if model_type == "dpt_large":  # DPT-Large
        net_w = net_h = 384
        model = DPTDepthModel(
            path=model_path,
            backbone="vitl16_384",
            non_negative=True,
            enable_attention_hooks=args.vis,
        )
        normalization = NormalizeImage(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
    elif model_type == "dpt_hybrid":  # DPT-Hybrid
        net_w = net_h = 384
        model = DPTDepthModel(
            path=model_path,
            backbone="vitb_rn50_384",
            non_negative=True,
            enable_attention_hooks=args.vis,
        )
        normalization = NormalizeImage(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
    elif model_type == "dpt_hybrid_kitti":
        net_w = 1216
        net_h = 352

        model = DPTDepthModel(
            path=model_path,
            scale=0.00006016,
            shift=0.00579,
            invert=True,
            backbone="vitb_rn50_384",
            non_negative=True,
            enable_attention_hooks=args.vis,
        )

        normalization = NormalizeImage(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
    elif model_type == "dpt_hybrid_nyu":
        net_w = 640
        net_h = 480

        model = DPTDepthModel(
            path=model_path,
            scale=0.000305,
            shift=0.1378,
            invert=True,
            backbone="vitb_rn50_384",
            non_negative=True,
            enable_attention_hooks=args.vis,
        )

        normalization = NormalizeImage(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
    elif model_type == "midas_v21":  # Convolutional model
        net_w = net_h = 384

        model = MidasNet_large(model_path, non_negative=True)
        normalization = NormalizeImage(
            mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]
        )
    else:
        assert (
            False
        ), f"model_type '{model_type}' not implemented, use: --model_type [dpt_large|dpt_hybrid|dpt_hybrid_kitti|dpt_hybrid_nyu|midas_v21]"

    transform = Compose(
        [
            Resize(
                net_w,
                net_h,
                resize_target=None,
                keep_aspect_ratio=True,
                ensure_multiple_of=32,
                resize_method="minimal",
                image_interpolation_method=cv2.INTER_CUBIC,
            ),
            normalization,
            PrepareForNet(),
        ]
    )

    model.eval()

    if optimize == True and device == torch.device("cuda"):
        model = model.to(memory_format=torch.channels_last)
        model = model.half()

    model.to(device)

    # get input
    img_names = glob.glob(os.path.join(input_path, "*"))
    num_images = len(img_names)

    # create output folder
    os.makedirs(output_path, exist_ok=True)
    cap = cv2.VideoCapture('rtsp://admin:admin@192.168.250.99:554/ch01/0') ####################################3
    #cap = cv2.VideoCapture(1)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 0)

    net = jetson.inference.detectNet("ssd-mobilenet-v2", ["","","ssd-mobilenet-v2","box,labels,conf","0.9"], 0.9)
    

    print("start processing")
    while(1):
        #if os.path.isdir(img_name):
        #    continue

        #print("  processing {} ({}/{})".format(img_name, ind + 1, num_images))
        # input

        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()
        ret, img = cap.read()

        cuda_frame = jetson.utils.cudaFromNumpy(img)
        detections = net.Detect(cuda_frame, overlay="box,labels,conf")
        


        if args.kitti_crop is True:
            height, width, _ = img.shape
            top = height - 352
            left = (width - 1216) // 2
            img = img[top : top + 352, left : left + 1216, :]

        img_input = transform({"image": img})["image"]

        # compute
        with torch.no_grad():
            sample = torch.from_numpy(img_input).to(device).unsqueeze(0)

            if optimize == True and device == torch.device("cuda"):
                sample = sample.to(memory_format=torch.channels_last)
                sample = sample.half()

            prediction = model.forward(sample)
            prediction = (
                torch.nn.functional.interpolate(
                    prediction.unsqueeze(1),
                    size=img.shape[:2],
                    mode="bicubic",
                    align_corners=False,
                )
                .squeeze()
                .cpu()
                .numpy()
            )

            if model_type == "dpt_hybrid_kitti":
                prediction *= 256

            if model_type == "dpt_hybrid_nyu":
                prediction *= 1000.0

            if args.vis:
                visualize_attention(sample, model, prediction, args.model_type)

        #filename = os.path.join(
        #    output_path, os.path.splitext(os.path.basename(img_name))[0]
        #)
        #util.io.write_depth(filename, prediction, bits=2)
        mindist = 100.0
        mindetect = None
        for detection in detections:

            if(detection.ClassID == 1):
                            print("\n\n\n")
                            print("Class: ",detection.ClassID)
                            depth_min = prediction.min()
                            depth_max = prediction.max()
                            
                            max_val = (2 ** (8 * 2)) - 1
                            #out = None
                            if depth_max - depth_min > np.finfo("float").eps:

                                out = max_val * (prediction - depth_min) / (depth_max - depth_min)
                            else:
                                out = np.zeros(prediction.shape, dtype=prediction.dtype)
                            
                            
                            dist = (max_val-np.mean([out.astype("uint16")[int(detection.Center[1]),int(detection.Center[0])],     out.astype("uint16")[int(detection.Center[1]-5),int(detection.Center[0]-5)],   out.astype("uint16")[int(detection.Center[1]+5),int(detection.Center[0]+5)],   out.astype("uint16")[int(detection.Center[1]+5),int(detection.Center[0]-5)], out.astype("uint16")[int(detection.Center[1]-5),int(detection.Center[0]+5)]]))/10000  
                            print("Distanse = ",dist)
                            if(mindist>dist):
                                mindist = dist
                                mindetect = detection
                            #print("\n\n\n\n\n\n\n\n\n",detection,"\n\n\n\n\n\n\n\n\n")
                            cv2.rectangle(img, (int(detection.Left),int(detection.Top)),(int(detection.Right),int(detection.Bottom)),(0,255,0),2)
                            
                            if(detection.Top<15)&(detection.Bottom>460):
                                cv2.putText(img,str(dist), (int(detection.Left), int(detection.Center[1])),cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,0,255), 1, cv2.LINE_AA)
                                
                            else:
                                if((detection.Top<15)&(detection.Top<460)):    

                                    cv2.putText(img,str(dist), (int(detection.Left)-5, int(detection.Bottom)+15),cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,0,255), 1, cv2.LINE_AA)
                                    
                                else:
                                    cv2.putText(img,str(dist), (int(detection.Left)-5, int(detection.Top)-5),cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,0,255), 1, cv2.LINE_AA)
                                    
                            cv2.imshow("depth", out.astype("uint16"))
        cv2.imshow("img", img)
        
        speed = 0
        rot = 0
        if(mindetect):
            if(mindetect.Center[0]<510):
                rot = -0.3
                print("to right")
            else: 
                if (mindetect.Center[0]>770):
                    rot = 0.3
                    print("to left")
                else:
                    rot = 0
                    print("to forward")
                    speed = float(mindist/10)
                    if (mindist>1):
                        mindist = 1
                    if(speed>1):
                        speed = 1
        else:
            speed = 0
            rot = 0
        
        exay = (str(speed) + " " + str(rot))
        print("\n\n\n\n\n\n\n\n\n", exay,"\n\n\n\n\n\n\n\n")
        sock.send(exay.encode())
        #sock.send("rostopic pub /twist geometry_msgs/Twist '[1.0,0.0,0.0]' '[0.0,0.0,0.0]'".encode())		
        data = sock.recv(1024)
   
        if (cv2.waitKey(1) & (0xFF==ord('q'))):
            break
    sock.close()

if __name__ == "__main__":
    sock = socket.socket()
    sock.connect(('192.168.250.90', 5693))
    print ("connected")		
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "-i", "--input_path", default="input", help="folder with input images"
    )

    parser.add_argument(
        "-o",
        "--output_path",
        default="output_monodepth",
        help="folder for output images",
    )

    parser.add_argument(
        "-m", "--model_weights", default=None, help="path to model weights"
    )

    parser.add_argument("--show-attention", dest="vis", action="store_true")

    parser.add_argument(
        "-t",
        "--model_type",
        default="dpt_hybrid",
        help="model type [dpt_large|dpt_hybrid|midas_v21]",
    )

    parser.add_argument("--kitti_crop", dest="kitti_crop", action="store_true")

    parser.add_argument("--optimize", dest="optimize", action="store_true")
    parser.add_argument("--no-optimize", dest="optimize", action="store_false")

    parser.set_defaults(optimize=True)
    parser.set_defaults(kitti_crop=False)

    args = parser.parse_args()

    default_models = {
        "midas_v21": "weights/midas_v21-f6b98070.pt",
        "dpt_large": "weights/dpt_large-midas-2f21e586.pt",
        "dpt_hybrid": "weights/dpt_hybrid-midas-501f0c75.pt",
        "dpt_hybrid_kitti": "weights/dpt_hybrid_kitti-cb926ef4.pt",
        "dpt_hybrid_nyu": "weights/dpt_hybrid_nyu-2ce69ec7.pt",
    }

    if args.model_weights is None:
        args.model_weights = default_models[args.model_type]

    # set torch options
    torch.backends.cudnn.enabled = True
    torch.backends.cudnn.benchmark = True

    # compute depth maps
    run(
        args.input_path,
        args.output_path,
        args.model_weights,
        args.model_type,
        args.optimize,
    )
