#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //some path, may be absolute or relative to bin/data
    string imagePath = "images/";
    ofDirectory imageDir(imagePath);
    //populate the directory object
    imageDir.listDir();
    
    if(imageDir.numFiles() == 0){
        ofLogError("ofApp::setup()","Need image files in bin/data/images to work!");
    }
    
    //go through and create a batch for each image
    for(int i = 0; i < imageDir.numFiles(); i++){
        ofLogNotice(imageDir.getPath(i));
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
