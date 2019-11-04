#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();
    void exit();
    
    void keyPressed(int key);
    
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    float tileCount = 10;
    float tileWidth;
    float tileHeight;
    float shapeSize = 50;
    float newShapeSize = shapeSize;
    float shapeAngle = 0;
    float maxDist;
    float currentShape;
//    float shape;
//
    int sizeMode = 0;
    
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    
    
    int nearThreshold;
    int farThreshold;
    
    bool bThreshWithOpenCV;
    bool bDrawPointCloud;
    
    int angle;
    
    // used for viewing the point cloud
    ofEasyCam easyCam;
//
//    int tileWidth;
//    int tileHeight;
//    float shapeSize = 50;
//    float newShapeSize = shapeSize;
//    float shapeAngle = 0;
//    float maxDist;
//    float currentShape;
//
//    int sizeMode = 0;
//
    int iterator;
    
    float closestX = 0;
    float closestY = 0;
    float averageX = 0;
    float averageY = 0;
    float smoothedX = 0;
    float smoothedY = 0;
    

    //    int shapes= 7;
    //   int shape= 0;
    
//    shape[0] = svg1;
//    shape[1] = svg2;
//    shape[2] = svg3;
//    shape[3] = svg4;
//    shape[4] = svg5;
//    shape[5] = svg6;
//    shape[6] = svg7;
//
    
    ofxSVG shapes[7];
    
    
    ofxSVG svg1;
    ofxSVG svg2;
    ofxSVG svg3;
    ofxSVG svg4;
    ofxSVG svg5;
    ofxSVG svg6;
    ofxSVG svg7;
    
    };
    
    //
    //    int tileCount;
    //
    //    int tileWidth;
    //    int tileHeight;
    //    float shapeSize = 50;
    //    float newShapeSize = shapeSize;
    //    float shapeAngle = 0;
    //    float maxDist;
    //    float currentShape;
    //
    //    int sizeMode = 0;
    //
    //    int iterator;
    //   vector<ofxSVG> shapes;
    //
    //// ofxSVG shapes[7];
    
    // from p5 sketch
    //    var tileCount = 10;
    //
    //    var tileWidth;
    //    var tileHeight;
    //    var shapeSize = 50;
    //    var newShapeSize = shapeSize;
    //    var shapeAngle = 0;
    //    var maxDist;
    //    var currentShape;
    //    var shapes;
    //
    //    var sizeMode = 0;
    //
    //
    



