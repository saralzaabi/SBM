#include "ofApp.h"
#include <iostream>
#include "ofMath.h"
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofxKinect kinect;
    //from kinect example, openframeworks
    
    kinect.setRegistration(true);
    
    kinect.init();
    
    kinect.open();        // opens first available kinect
    
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;
    
    // start from the front
    bDrawPointCloud = false;
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    
    
    //    ofxSVG svg;
    
    tileCount = 10;
    shapeSize = 10;
    newShapeSize = shapeSize;
    shapeAngle = 0;
    sizeMode = 0;
    
    svg1.load("module_1.svg");
    svg2.load("module_2.svg");
    svg3.load("module_3.svg");
    svg4.load("module_4.svg");
    svg5.load("module_5.svg");
    svg6.load("module_6.svg");
    svg7.load("module_7.svg");
    
    //    shapes[0] = shapes.push_back('data/module_1.svg'); ??
    
    shapes[0] = svg1;
    shapes[1] = svg2;
    shapes[2] = svg3;
    shapes[3] = svg4;
    shapes[4] = svg5;
    shapes[5] = svg6;
    shapes[6] = svg7;
    
    //    cout << ".." << ..[..] << endl;
    
    tileWidth = ofGetWidth() / tileCount;
    tileHeight = ofGetHeight() / tileCount;
    
    maxDist = sqrt(pow(ofGetWidth(), 2) + pow(ofGetHeight(), 2));
    
    //    tileWidth = ofGetWindowWidth() / tileCount;
    //    tileHeight = ofGetWindowHeight() / tileCount;
    
    //    maxDist = sqrt(pow(ofGetWindowWidth() , 2) + pow(ofGetWindowHeight(), 2));
    
    iterator = 0; //
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //    ofClear(100,100,100);
    
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        //CLOSEST PIX
        
        int amount = 0;
        
        ofPixels & pix = grayImage.getPixels();
        int numPixels = pix.size();
        for (int y=0;y<kinect.height;y++){
            for (int x=0; x<kinect.width; x++) {
                int index = x+y*kinect.width;
                if(pix[index] < nearThreshold && pix[index] > farThreshold) {
                    averageX += x;
                    averageY += y;
                    averageX = ofMap(averageX, 0, 640, -200, ofGetWindowWidth());
                    averageY = ofMap(averageY, 0, 480, -200, ofGetWindowHeight());
                    amount ++;
                    pix[index] = 255;
                } else {
                    pix[index] = 0;
                }
            }
        }
        if (amount>0){
            averageX /= amount;
            averageY /= amount;
        }
        smoothedX += (averageX - smoothedX) * 0.02;
        smoothedY += (averageY - smoothedY) * 0.02;
    }
    
    
    //AVERAGE DIST
    {
        int amount = 0;
        float averageX = 0;
        float averageY = 0;
        
        ofPixels & pix = grayImage.getPixels();
        int numPixels = pix.size();
        for (int y=0;y<kinect.height;y++){
            for (int x=0; x<kinect.width; x++) {
                int index = x+y*kinect.width;
                if(pix[index] < nearThreshold && pix[index] > farThreshold) {
                    averageX += x;
                    averageY += y;
                    //                    averageX = ofMap(averageX, 0, 640, -200, ofGetWindowWidth());
                    //                    averageY = ofMap(averageY, 0, 480, -200, ofGetWindowHeight());
                    amount ++;
                    pix[index] = 255;
                } else {
                    pix[index] = 0;
                }
            }
        }
        if (amount>0){
            averageX /= amount;
            averageY /= amount;
            
        }
        
        std::cout<<smoothedX<<endl;
    }
}
    
    //--------------------------------------------------------------

void ofApp::draw() {
        
        ofSetColor(255, 255, 255);
        
        
        
        if(bDrawPointCloud) {
            easyCam.begin();
            drawPointCloud();
            easyCam.end();
        } else {
            // draw from the live kinect
            //        kinect.drawDepth(10, 10, 400, 300);
            //        kinect.draw(420, 10, 400, 300);
            //
            //        grayImage.draw(10, 320, 400, 300);
            //        contourFinder.draw(10, 320, 400, 300);
            //        ofPushMatrix();
            //        ofTranslate(10, 320);
            ofSetColor(0, 250, 0);
            ofDrawEllipse(smoothedX, smoothedY, 30, 30);
            
            //  ofDrawEllipse(mouseX, mouseY, 30, 30);
            //        ofPopMatrix();
#ifdef USE_TWO_KINECTS
            kinect2.draw(420, 320, 400, 300);
#endif
        }
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        
        for (int gridY = 0; gridY < tileCount; gridY++) {
            for (int gridX = 0; gridX < tileCount; gridX++) {
                float posX = tileWidth * gridX + tileWidth / 2;
                float posY = tileHeight * gridY + tileWidth / 2;
                
                // calculate angle between mouse position and actual position of the shape
                float angle = atan2(smoothedY - posY, smoothedX - posX) + (shapeAngle * (PI / 180));
                
                //   float angle = atan2(mouseY - posY, mouseX - posX) + (shapeAngle * (PI / 180));
                
                if (sizeMode == 0) newShapeSize = shapeSize;
                if (sizeMode == 1) newShapeSize = shapeSize * 1.5 - ofMap(ofDist(smoothedX, smoothedY, posX, posY), 0, 500, 5, shapeSize);
                if (sizeMode == 2) newShapeSize = ofMap(ofDist(smoothedX, smoothedY, posX, posY), 0, 500, 5, shapeSize);
                
                
                // if (sizeMode == 1) newShapeSize = shapeSize * 1.5 - ofMap(ofDist(mouseX, mouseY, posX, posY), 0, 500, 5, shapeSize);
                //            if (sizeMode == 2) newShapeSize = ofMap(ofDist(mouseX, mouseY, posX, posY), 0, 500, 5, shapeSize);
                
                
                
                ofPushMatrix();
                ofTranslate(posX, posY);
                ofRotateRad(angle);
                
                ofSetLineWidth(0);
                ofScale(newShapeSize/shapes[iterator].getWidth(), newShapeSize/shapes[iterator].getHeight()); //steven
                shapes[iterator].draw();
                ofSetColor(0);//steven
                ofDrawLine(0, 0, newShapeSize, 0);//steven
                ofPopMatrix();
            }
        }
        
    }
    //old
    //    for (int gridY = 0; gridY < tileCount; gridY++) {
    //        for (int gridX = 0; gridX < tileCount; gridX++) {
    //
    //            float posX = tileWidth * gridX + tileWidth / 2;
    //            float posY = tileHeight * gridY + tileWidth / 2;
    //
    //            // calculate angle between mouse position and actual position of the shape
    //            float angle = atan2(mouseY - posY, mouseX - posX) + (shapeAngle * (PI / 180));
    //
    //            if (sizeMode == 0) newShapeSize = shapeSize;
    //            if (sizeMode == 1) newShapeSize = shapeSize * 1.5 - ofMap(ofDist(mouseX, mouseY, posX, posY), 0, 500, 5, shapeSize);
    //            if (sizeMode == 2) newShapeSize = ofMap(ofDist(mouseX, mouseY, posX, posY), 0, 500, 5, shapeSize);
    //
    //            //            ofPush();
    //
    //            ofPushMatrix();
    //            ofTranslate(posX, posY);
    //            ofRotateRad(angle);
    //            //            rotate(angle)?;
    //            ofSetLineWidth(0);//change?
    //
    //           shapes[].draw(currentShape, 0, 0, newShapeSize, newShapeSize); // define
    //            ofPopMatrix()
    //


//kinect example

void ofApp::drawPointCloud() {
    int w = 640;
    int h = 480;
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            if(kinect.getDistanceAt(x, y) > 0) {
                mesh.addColor(kinect.getColorAt(x,y));
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    ofEnableDepthTest();
    mesh.drawVertices();
    ofDisableDepthTest();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
    
#ifdef USE_TWO_KINECTS
    kinect2.close();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case ' ':
            bThreshWithOpenCV = !bThreshWithOpenCV;
            break;
            
        case'p':
            bDrawPointCloud = !bDrawPointCloud;
            break;
            
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
            
        case '<':
        case ',':
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            break;
            
        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            break;
            
        case '-':
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            break;
            
        case 'w':
            kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
            break;
            
        case 'o':
            kinect.setCameraTiltAngle(angle); // go back to prev tilt
            kinect.open();
            break;
            
        case 'c':
            kinect.setCameraTiltAngle(0); // zero the tilt
            kinect.close();
            break;
            
        case '1':
            kinect.setLed(ofxKinect::LED_GREEN);
            break;
            
        case '2':
            kinect.setLed(ofxKinect::LED_YELLOW);
            break;
            
        case '3':
            kinect.setLed(ofxKinect::LED_RED);
            break;
            
        case '4':
            kinect.setLed(ofxKinect::LED_BLINK_GREEN);
            break;
            
        case '5':
            kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
            break;
            
        case '0':
            kinect.setLed(ofxKinect::LED_OFF);
            break;
            
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == '1') iterator = 0;
    if (key == '2') iterator = 1;
    if (key == '3') iterator = 2;
    if (key == '4') iterator = 3;
    if (key == '5') iterator = 4;
    if (key == '6') iterator = 5;
    if (key == '7') iterator = 6;
    
    if (key == OF_KEY_UP) shapeSize += 5;
    if (key == OF_KEY_DOWN) shapeSize -= 5;
    //    if (key == OF_KEY_DOWN) shapeSize -= max(shapeSize - 5, 5);
    if (key == OF_KEY_LEFT) shapeAngle += 5;
    if (key == OF_KEY_RIGHT) shapeAngle -= 5;
    
    // if (key == 's' || key == 'S') saveCanvas(gd.timestamp(), 'png');
    if (key == 'd' || key == 'D') sizeMode = (sizeMode + 1) % 3;
    if (key == 'g' || key == 'G') {
        tileCount += 5;
        if (tileCount > 20) {
            tileCount = 10;
        }
        tileWidth = ofGetWidth() / tileCount;
        tileHeight = ofGetHeight() / tileCount;
    }
    
}

//
//    if (key == 's' || key == 'S') saveCanvas(gd.timestamp(), 'png');
//    if (key == 'd' || key == 'D') sizeMode = (sizeMode + 1) % 3;
//    if (key == 'g' || key == 'G') {
//        tileCount += 5;
//        if (tileCount > 20) {
//            tileCount = 10;
//        }
//        tileWidth = ofGetWidth() / tileCount;
//        tileHeight = height / tileCount;
//    }
//
//    if (key == '1') currentShape = shapes[0];
//    if (key == '2') currentShape = shapes[1];
//    if (key == '3') currentShape = shapes[2];
//    if (key == '4') currentShape = shapes[3];
//    if (key == '5') currentShape = shapes[4];
//    if (key == '6') currentShape = shapes[5];
//    if (key == '7') currentShape = shapes[6];
//
//    if (keyCode == UP_ARROW) shapeSize += 5;
//    if (keyCode == DOWN_ARROW) shapeSize = max(shapeSize - 5, 5);
//    if (keyCode == LEFT_ARROW) shapeAngle += 5;
//    if (keyCode == RIGHT_ARROW) shapeAngle -= 5;

//
//    if (key == 's' || key == 'S') saveCanvas(gd.timestamp(), 'png');
//    if (key == 'd' || key == 'D') sizeMode = (sizeMode + 1) % 3;
//    if (key == 'g' || key == 'G') {
//        tileCount += 5;
//        if (tileCount > 20) {
//            tileCount = 10;
//        }
//        tileWidth = width / tileCount;
//        tileHeight = height / tileCount;
//    }
//
//    if (key == '1') currentShape = shapes[0];
//    if (key == '2') currentShape = shapes[1];
//    if (key == '3') currentShape = shapes[2];
//    if (key == '4') currentShape = shapes[3];
//    if (key == '5') currentShape = shapes[4];
//    if (key == '6') currentShape = shapes[5];
//    if (key == '7') currentShape = shapes[6];
//
//    if (keyCode == UP_ARROW) shapeSize += 5;
//    if (keyCode == DOWN_ARROW) shapeSize = max(shapeSize - 5, 5);
//    if (keyCode == LEFT_ARROW) shapeAngle += 5;
//    if (keyCode == RIGHT_ARROW) shapeAngle -= 5;



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


//ofrotateRAD
