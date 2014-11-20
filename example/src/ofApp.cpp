#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //some path, may be absolute or relative to bin/data
    string imagePath = "images/";
    imageDir = ofDirectory(imagePath);
    //populate the directory object
    imageDir.listDir();
    
    if(imageDir.numFiles() == 0){
        ofLogError("ofApp::setup","Need image files in bin/data/images to work!");
    }
    
    // Initializes the loader queue and such
    loaderSpool.setup();
    
    //go through and create a batch for each image
    for(int i = 0; i < imageDir.numFiles(); i++){
        //Using filename as batch id
        ofxLoaderBatch* batch = new ofxLoaderBatch(imageDir.getName(i));
        
        // Using the filename as image id as well (only one per batch)
        batch->addTexture(imageDir.getPath(i),imageDir.getName(i));
        
        loaderSpool.addBatch(batch);
    }
    
    // Loading an image
    loaderSpool.loadBatch(imageDir.getName(0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(loaderSpool.isBatchDrawable(imageDir.getName(0))){
        loaderSpool.getBatch(imageDir.getName(0))->getTexture(imageDir.getName(0))->draw(0,0, ofGetWidth(), ofGetHeight());
    }
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
