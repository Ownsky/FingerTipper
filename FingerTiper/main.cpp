//
//  main.cpp
//  CVTest
//
//  Created by mac on 16/7/17.
//  Copyright © 2016年 Ownsky. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "HandDealer.hpp"

using namespace std;
using namespace cv;


int main(int argc, const char * argv[]) {
    //Mat imageTest = imread("/Users/mac/Desktop/opencv/CVTest/CVTest/dontstarve.jpeg");
    namedWindow("image1", WINDOW_NORMAL);
    //namedWindow("image2", WINDOW_NORMAL);
    CvCapture* vc = cvCreateCameraCapture(0);
    Mat vP;
    /*
    vP = imread("/Users/mac/Desktop/AD26BC5B-3715-4AC1-86EE-8063597A46FE.png");
    //flip(vP, vP, 1);
    
    HandDealer hd(vP);
    imshow("image1", hd.originalGraphic());
    waitKey();
    imshow("image1", hd.grayGraphic());
    waitKey();
    vector<Point> tips = hd.findFingerTips();
    for (int i = 0; i < tips.size(); i++) {
        circle(vP, tips[i], 20, 255, -1);
    }
    imshow("image1", vP);
    waitKey();
    */
    HandDealer hd;
    while (1) {
        vP = cvQueryFrame(vc);
        flip(vP, vP, 1);
        
        //HandDealer hd(vP);
        hd.setGraphic(vP);
        vector<Point> tips = hd.findFingerTips();
        for (int i = 0; i < tips.size(); i++) {
            circle(vP, tips[i], 20, 255, -1);
        }
        imshow("image1", vP);
        //imshow("image2", hd.grayGraphic());
        waitKey(33);
    }

    return 0;
}
