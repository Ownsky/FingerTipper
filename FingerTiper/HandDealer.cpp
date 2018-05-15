//
//  HandDealer.cpp
//  CVTest
//
//  Created by mac on 7/26/16.
//  Copyright © 2016 Ownsky. All rights reserved.
//

#include "HandDealer.hpp"
#define ITV 10
 Mat HandDealer::findSkin(Mat& origin){
    Mat YCbCr;
    Mat Skin(origin.rows, origin.cols, CV_8U);
    vector<Mat> Channels;
    cvtColor(origin, YCbCr, CV_BGR2YCrCb);
    split(YCbCr, Channels);
    MatIterator_<uchar> Cb_It = Channels[2].begin<uchar>(),
                        Cr_It = Channels[1].begin<uchar>(),
                        Skin_It = Skin.begin<uchar>(),
                        Skin_End = Skin.end<uchar>();
    for ( ; Skin_It != Skin_End; ++Cb_It, ++Cr_It, ++Skin_It) {
        if (*Cb_It >= m_CbAlpha && *Cb_It <= m_CbBeta && *Cr_It >= m_CrAlpha && *Cr_It <= m_CrBeta) {
            *Skin_It = 255;
        } else {
            *Skin_It = 0;
        }
    }
    dilate(Skin, Skin, Mat(5, 5, CV_8UC1), Point(-1, -1));
    erode(Skin, Skin, Mat(5, 5, CV_8UC1), Point(-1, -1));
//    namedWindow("skinTest1");
//    namedWindow("skinTest2");
//    imshow("skinTest1", Skin);
//    imshow("skinTest2", origin);
    return Skin;
}

void HandDealer::dealAll(){
    GaussianBlur(m_originalGraphic, m_originalGraphic, Size(5,5), 0);
    m_grayGraphic = findSkin(m_originalGraphic);
    findContours(m_grayGraphic, m_contour, m_hiach, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Mat test(m_originalGraphic.rows, m_originalGraphic.cols, CV_8U);
//    vector<vector<Point>>::iterator it;
//    for (it = m_contour.begin(); it < m_contour.end(); it++) {
//        
//        for (int j = 0; j < it->size(); j++) {
//            test.at<uchar>((*it)[j]) = 255;
//        }
//    }
//    namedWindow("skinTest3");
//    imshow("skinTest3", test);
}

HandDealer::HandDealer(int CbAlpha, int CbBeta, int CrAlpha, int CrBeta){
    m_CbAlpha = CbAlpha;
    m_CbBeta = CbBeta;
    m_CrAlpha = CrAlpha;
    m_CrBeta = CrBeta;
}

HandDealer::HandDealer(Mat& m, int CbAlpha, int CbBeta, int CrAlpha, int CrBeta){
    m_CbAlpha = CbAlpha;
    m_CbBeta = CbBeta;
    m_CrAlpha = CrAlpha;
    m_CrBeta = CrBeta;
    m_originalGraphic = m.clone();
    dealAll();
}

HandDealer::HandDealer(HandDealer& h){
    m_originalGraphic = h.m_originalGraphic.clone();
    m_grayGraphic = h.m_grayGraphic.clone();
    //m_edgeGraphic = h.m_edgeGraphic.clone();
}

void HandDealer::setGraphic(cv::Mat &g){
    m_originalGraphic = g.clone();
    dealAll();
}

Mat HandDealer::originalGraphic(){
    return m_originalGraphic;
}

Mat HandDealer::grayGraphic(){
    return m_grayGraphic;
}

vector<Point> HandDealer::findFingerTips(){
//    Mat test(m_originalGraphic.rows, m_originalGraphic.cols, CV_8U);
    if (m_contour.size() == 0) {
        return *new vector<Point>();
    }
    vector<vector<Point>*> inflections;
    vector<Point> fingerTips;
    vector<Point> currentContour;
    int area = 0, k = 0;
    for (int i = 0; i < m_contour.size(); i++) {
        int s = contourArea(m_contour[i]);
        if (s > area) {
            k = i;
            area = s;
        }
    }
    currentContour = m_contour[k];
    
    Moments handMoment = moments(currentContour);
    Point handCenter(handMoment.m10/handMoment.m00, handMoment.m01/handMoment.m00);
//    circle(test, handCenter, 20, 255);
    
//    Mat testSheet(220, int(currentContour.size()), CV_8U);
//    MatIterator_<uchar> t_It = testSheet.begin<uchar>();
//    for (; t_It<testSheet.end<uchar>(); ++t_It) {
//        *t_It = 0;
//    }
    for (int j = ITV; j < currentContour.size()-ITV; j++) {
        Point p0 = currentContour[j],
        p1 = currentContour[j-ITV],
        p2 = currentContour[j+ITV];
        double scalar = (p0.x-p1.x)*(p0.x-p2.x) + (p0.y-p1.y)*(p0.y-p2.y);
        double dist1 = distance(p0, p1);
        double dist2 = distance(p0, p2);
        double cosA = scalar/(dist1*dist2);
//        int color = 127;
        
        if (cosA >= -0.866 && cosA <= 0.5) {
//            color = 255;
//            test.at<uchar>(p0) = 255;
//            circle(test, p0, 5, 200);
            if (inflections.size() != 0 &&
                ((*(inflections.end()-1))->size() == 0 ||
                 ((*(inflections.end()-1))->size() != 0 &&
                  distance(*((*(inflections.end()-1))->end()-1), p0) < 40))){
                     (*(inflections.end()-1))->push_back(p0);
                 }else{
                     vector<Point>* v = new vector<Point>();
                     v->push_back(p0);
                     inflections.push_back(v);
                     //v->push_back(p0);
                 }
        }
        //testSheet.at<uchar>(100-cosA*100, j) = color;
//        Point tmp(j, 110-cosA*100);
//        Point tmp_b(j, 210);
//        line(testSheet, tmp, tmp_b, color);
    }
//    namedWindow("testSheet");
//    imshow("testSheet", testSheet);
    

    for (int i = 0; i < inflections.size(); i++) {
        if ((*inflections[i]).size() < 5) {
            continue;
        }
        Moments mo = moments(*inflections[i]);
        Point center(mo.m10/mo.m00, mo.m01/mo.m00);
        if (center.x < 20 || center.x > m_originalGraphic.cols - 20 || center.y < 20 || center.y > m_originalGraphic.rows - 20) {
            continue;
        }
        fingerTips.push_back(center);
        delete inflections[i];
    }
    vector<Point>::iterator it1,it2;
    for (it1 = fingerTips.begin(); it1 < (fingerTips.end()-1); it1++) {
        for (it2 = it1+1; it2 < fingerTips.end(); it2++) {
            if (distance(*it1, *it2) < 40) {
                (*it1).x = ((*it1).x + (*it2).x)/2;
                (*it1).y = ((*it1).y + (*it2).y)/2;
                it2 = fingerTips.erase(it2)-1;
            }
        } 
    }
    double aveDist = 0;
    for (it1 = fingerTips.begin(); it1 < fingerTips.end(); it1++) {
        aveDist += distance(handCenter, *it1);
    }
    aveDist /= fingerTips.size();
//    circle(test, handCenter, aveDist, 255);
    for (it1 = fingerTips.begin(); it1 < fingerTips.end(); it1++) {
        if (distance(handCenter, *it1) < aveDist*6/5.0) {
//            circle(test, *it1, 20, 180);
            it1 = fingerTips.erase(it1)-1;
        }
    }
//    for (int i = 0; i < fingerTips.size(); i++) {
//        circle(test, fingerTips[i], 20, 255);
//    }
//    namedWindow("SkinTestFinal");
//    imshow("SkinTestFinal", test);
    return fingerTips;
}

//Mat HandDealer::edgeGraphic(){
//    return m_edgeGraphic;
//}

