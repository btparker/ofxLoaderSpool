//
//  ofxLoaderSpool.cpp
//  ofxLoaderSpool
//
//  Created by  Tyler Parker on 11/14/14.
//
//

#include "ofxLoaderSpool.h"

ofxLoaderSpool::ofxLoaderSpool(){
    
}

void ofxLoaderSpool::setup(){
    q = ProgressiveTextureLoadQueue::instance();
    
    q->setTexLodBias(-0.5); //negative gives you lower mipmaps >> sharper
    q->setScanlinesPerLoop(64);
    q->setTargetTimePerFrame(10.0f);
    q->setNumberSimultaneousLoads(3);
    q->setVerbose(false);
};

void ofxLoaderSpool::update(){
}


void ofxLoaderSpool::textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg){
    ofLogNotice() << imageFilenames[arg.tex] << " texture Drawable!";
    drawable[arg.tex] = true;
}


void ofxLoaderSpool::textureReady(ofxProgressiveTextureLoad::textureEvent& arg){
    if (arg.loaded){
        //ofLogNotice() << "textureReady!";
        ready[arg.tex] = true;
    }else{
        ofLogError() << imageFilenames[arg.tex] << " texture load failed!";
    }
}

ofTexture* ofxLoaderSpool::loadImage(string imageFilename){
    ofTexture* t = new ofTexture(); //create your own texture to got data loaded into; it will be cleared!
    textures[imageFilename] = t;
    ready[t] = false;
    imageFilenames[t] = imageFilename;
    ofxProgressiveTextureLoad * loader = q->loadTexture(imageFilename,
                                                        t,					/*tex to load into*/
                                                        true,				/*MIP-MAPS!*/
                                                        CV_INTER_AREA);		/*Resize Quality*/
    
    ofAddListener(loader->textureReady, this, &ofxLoaderSpool::textureReady);
    ofAddListener(loader->textureDrawable, this, &ofxLoaderSpool::textureDrawable);
    
//    loaders[t] = loader;
    return t;
}

ofTexture* ofxLoaderSpool::getTexture(string imageFilename){
    return textures[imageFilename];
}

bool ofxLoaderSpool::isTextureReady(string imageFilename){
    return isTextureReady(textures[imageFilename]);
}

bool ofxLoaderSpool::isTextureReady(ofTexture *tex){
    return ready[tex];
}

bool ofxLoaderSpool::isTextureDrawable(string imageFilename){
    return isTextureDrawable(textures[imageFilename]);
}

bool ofxLoaderSpool::isTextureDrawable(ofTexture *tex){
    return drawable[tex];
}

string ofxLoaderSpool::getTextureFilename(ofTexture *tex){
    return imageFilenames[tex];
}

void ofxLoaderSpool::clearImage(ofTexture *tex){
//    loaders[tex]->stopLoadingAsap();
    if(ready.count(tex) > 0){
        tex->clear();
        ready[tex] = false;
        drawable[tex] = false;
    }
//    delete loaders[tex];
//    delete textures[imageFilenames[tex]];
}

void ofxLoaderSpool::clearImage(string imageFilename){
    clearImage(textures[imageFilename]);
}

void ofxLoaderSpool::addBatch(string id){
    ofxLoaderBatch* batch = new ofxLoaderBatch(id);
    batches[id] = batch;
}

void ofxLoaderSpool::addImageToBatch(string id, string imageFilename){
    batches[id]->imageFilenames.push_back(imageFilename);
}

void ofxLoaderSpool::clearBatch(string id){
    if(batches.count(id)>0){
        for(int i = 0; i < batches[id]->imageFilenames.size(); i++){
            clearImage(batches[id]->imageFilenames[i]);
        }
    }
}

void ofxLoaderSpool::loadBatch(string id){
    if(batches.count(id)>0){
        for(int i = 0; i < batches[id]->imageFilenames.size(); i++){
            loadImage(batches[id]->imageFilenames[i]);
        }
    }
}

bool ofxLoaderSpool::isBatchReady(string id){
    bool ready = true;
    for(int i = 0; i < batches[id]->imageFilenames.size(); i++){
        ready = ready && isTextureReady(batches[id]->imageFilenames[i]);
    }
    return ready;
}

bool ofxLoaderSpool::isBatchDrawable(string id){
    bool drawable = true;
    for(int i = 0; i < batches[id]->imageFilenames.size(); i++){
        drawable = drawable && isTextureDrawable(batches[id]->imageFilenames[i]);
    }
    return drawable;
}

ofxLoaderBatch* ofxLoaderSpool::getBatch(string id){
    return batches[id];
}

int ofxLoaderSpool::getBatchesSize(){
    return batches.size();
}

ofxLoaderSpool::~ofxLoaderSpool(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string k =  iter->first;
        delete batches[k];
    }
    
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        clearImage(iter->first);
    }
}