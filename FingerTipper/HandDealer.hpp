//
//  HandDealer.hpp
//  CVTest
//
//  Created by mac on 7/26/16.
//  Copyright © 2016 Ownsky. All rights reserved.
//

#ifndef HandDealer_hpp
#define HandDealer_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

inline double distance(Point& p1, Point& p2){
    return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y, 2));
}



class HandDealer{
    Mat m_originalGraphic;
    Mat m_grayGraphic;
    //Mat m_edgeGraphic;
    vector<vector<Point>> m_contour;
    vector<Vec4i> m_hiach;
    int m_CbAlpha, m_CbBeta;
    int m_CrAlpha, m_CrBeta;
    Mat findSkin(Mat& origin);
    void dealAll();
    
public:
    HandDealer(int CbAlpha = 100, int CbBeta = 127, int CrAlpha = 138, int CrBeta = 170);
    HandDealer(Mat& m, int CbAlpha = 100, int CbBeta = 127, int CrAlpha = 138, int CrBeta = 170);
    //HandDealer(Mat& m, int CbAlpha = 100, int CbBeta = 130, int CrAlpha = 125, int CrBeta = 220);
    HandDealer(HandDealer& h);
    void setGraphic(Mat& g);
    Mat originalGraphic();
    Mat grayGraphic();
//    Mat edgeGraphic();
    vector<vector<Point>> Contour(){
        return m_contour;
    }
    vector<Point> findFingerTips();
    
};

#endif /* HandDealer_hpp */
