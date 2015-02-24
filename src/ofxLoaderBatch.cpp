//
//  ofxLoaderBatch.cpp
//
//  Created by  Tyler Parker on 11/17/14.
//
//

#include "ofxLoaderBatch.h"
ofxLoaderBatch::ofxLoaderBatch(){
}

ofxLoaderBatch::ofxLoaderBatch(string _id){
    id = _id;
}

ProgressiveTextureLoadQueue * ofxLoaderBatch::getParentLoadQueue(){
    return q;
}

void ofxLoaderBatch::setParentLoadQueue(ProgressiveTextureLoadQueue *_q){
    q = _q;
}

string ofxLoaderBatch::getId(){
    return id;
}

void ofxLoaderBatch::setId(string _id){
    id = _id;
}

ofxLoaderBatch* ofxLoaderBatch::addBatch(string _batchId){
    ofxLoaderBatch* batch = new ofxLoaderBatch(_batchId);
    return addBatch(batch);
}

ofxLoaderBatch* ofxLoaderBatch::addBatch(ofxLoaderBatch* _batch){
    if(batches.count(_batch->getId()) > 0){
        ofLogError("Batch '"+getId()+"'::addBatch", "A batch with id "+_batch->getId()+" already exists!");
        return NULL;
    }
    batches[_batch->getId()] = _batch;
    batches[_batch->getId()]->setParentLoadQueue(getParentLoadQueue());
    return batches[_batch->getId()];
}

void ofxLoaderBatch::addTexture(string _textureFilename, string _textureId = ""){
    if(_textureId.length() == 0){
        _textureId = _textureFilename;
    }
    if(!ofFile(_textureFilename).exists()){
        ofLogError("Batch '"+getId()+"'::addTexture", "Cannot add texture '"+_textureId+"', file '"+_textureFilename+"' could not be found!");
        return;
    }
    if(textureFilenames.count(_textureId) > 0){
        ofLogError("Batch '"+getId()+"'::addTexture", "A texture with id "+_textureId+" already exists!");
        return;
    }
    ofLogNotice("Batch '"+getId()+"'",("Adding texture '"+_textureId + "' ('"+_textureFilename+"')"));

    textureFilenames[_textureId] = _textureFilename;
    initTexture(_textureId);
}

ofxLoaderBatch* ofxLoaderBatch::getBatch(string _batchId){
    if(batches.count(_batchId) == 0){
        ofLogError("Batch '"+getId()+"'::getBatch", "No batch found with id "+_batchId);
        return NULL;
    }
    return batches[_batchId];
}

void ofxLoaderBatch::textureReady(ofxProgressiveTextureLoad::textureEvent& arg){
    if (arg.fullyLoaded){
        ready[arg.tex] = true;
        loading[arg.tex] = false;
        ofLogNotice("Batch '"+getId()+"'",("Texture '"+ids[arg.tex] + "' ready"));
    }else{
        ofLogError("Batch '"+getId()+"'::textureReady",("Texture '"+ids[arg.tex] + "' ('"+textureFilenames[ids[arg.tex]]+"') load failed"));
    }

}

void ofxLoaderBatch::initTexture(string _textureId){
    if(textureFilenames.count(_textureId) == 0){
        ofLogError("Batch '"+getId()+"'::initTexture", "No texture found with id "+_textureId);
        return;
    }
    if(textures.count(_textureId) != 0){
        clearTexture(_textureId);
    }
    ofTexture* t = new ofTexture(); //create your own texture to got data loaded into; it will be cleared!
    textures[_textureId] = t;
    ids[t] = _textureId;
    ready[t] = false;
    ofLogNotice("ofxLoaderBatch::initTexture","Init texture "+_textureId);
}

ofTexture* ofxLoaderBatch::loadTexture(string _textureId){
    if(textures.count(_textureId) == 0){
        ofLogError("Batch '"+getId()+"'::loadTexture", "No texture found with id "+_textureId);
        return NULL;
    }
    else if(isTextureLoading(_textureId)){
        ofLogWarning("Batch '"+getId()+"'::loadTexture", "Texture '"+_textureId+"' is already loading");
        return NULL;
    }
    else if(isTextureReady(_textureId)){
        ofLogWarning("Batch '"+getId()+"'::loadTexture", "Texture '"+_textureId+"' already loaded");
        return getTexture(_textureId);
    }
    ofxProgressiveTextureLoad * loader = q->loadTexture(textureFilenames[_textureId],
                                                        getTexture(_textureId),     /*tex to load into*/
                                                        false,               /*MIP-MAPS!*/
                                                        CV_INTER_AREA);     /*Resize Quality*/
    ofLogNotice("Batch '"+getId()+"'",("Texture '"+_textureId + "' loading..."));
    loading[getTexture(_textureId)] = true;
    ofAddListener(loader->textureReady, this, &ofxLoaderBatch::textureReady);
    return getTexture(_textureId);
}

ofTexture* ofxLoaderBatch::getTexture(string _textureId){
    if(!hasTexture(_textureId)){
        ofLogError("Batch '"+getId()+"'::getTexture", "No texture found with id "+_textureId);
        return NULL;
    }
    return textures[_textureId];
}

bool ofxLoaderBatch::hasTexture(string _textureId){
    return textures.count(_textureId) != 0;
}

bool ofxLoaderBatch::isTextureReady(string _textureId){
    return isTextureReady(textures[_textureId]);
}

bool ofxLoaderBatch::isTextureReady(ofTexture *tex){
    return ready[tex];
}

bool ofxLoaderBatch::isTextureLoading(string _textureId){
    return isTextureLoading(textures[_textureId]);
}

bool ofxLoaderBatch::isTextureLoading(ofTexture *tex){
    return loading[tex];
}

string ofxLoaderBatch::getTextureFilename(ofTexture *tex){
    return textureFilenames[ids[tex]];
}

string ofxLoaderBatch::getTextureId(ofTexture *tex){
    return ids[tex];
}

void ofxLoaderBatch::clear(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _batchId =  iter->first;
        batches[_batchId]->clear();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _textureId =  iter->first;
        clearTexture(_textureId);
    }
}

void ofxLoaderBatch::load(){
    startTime = ofGetElapsedTimef();
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _batchId =  iter->first;
        batches[_batchId]->load();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _textureId =  iter->first;
        loadTexture(_textureId);
    }
}

bool ofxLoaderBatch::isReady(){
    bool isBatchReady = true;
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _batchId =  iter->first;
        isBatchReady = isBatchReady && batches[_batchId]->isReady();
    }

    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _textureId =  iter->first;
        isBatchReady = isBatchReady && isTextureReady(_textureId);
    }
    return isBatchReady;
}

void ofxLoaderBatch::clearTexture(ofTexture *tex){
    if(ids.count(tex) == 0){
        ofLogError("Batch '"+getId()+"'::clearTexture", "No texture found with given pointer");
        return;
    }

    clearTexture(ids[tex]);
}

void ofxLoaderBatch::clearTexture(string _textureId){
    if(textures.count(_textureId) == 0){
        ofLogError("Batch '"+getId()+"'::clearTexture", "No texture found with id "+_textureId);
        return;
    }
    
    ready[textures[_textureId]] = false;
    textures[_textureId]->clear();
    ofLogNotice("Batch '"+getId()+"'",("Texture '"+_textureId + "' ('"+textureFilenames[_textureId]+"') cleared"));
}

ofxLoaderBatch::~ofxLoaderBatch(){
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        ids.erase(iter->second);
        ready.erase(iter->second);
        iter->second->clear();
    }
    textures.clear();
}

