//
//  ofxLoaderBatch.cpp
//
//  Created by  Tyler Parker on 11/17/14.
//
//

#include "ofxLoaderBatch.h"

ofxLoaderBatch::ofxLoaderBatch(string _id){
    id = _id;
}

void ofxLoaderBatch::setParentLoadQueue(ProgressiveTextureLoadQueue *_q){
    q = _q;
}

void ofxLoaderBatch::addBatch(string id){
    ofxLoaderBatch* batch = new ofxLoaderBatch(id);
    batches[id] = batch;
}

void ofxLoaderBatch::addTexture(string _filename, string _id = ""){
    if(_id.length() == 0){
        _id = _filename;
    }
    ofLogNotice("Batch '"+id+"'",("Adding texture '"+_id + "' with file '"+_filename+"'"));

    filenames[_id] = _filename;
    initTexture(_id);
}

ofxLoaderBatch* ofxLoaderBatch::getBatch(string _id){
    if(batches.count(_id) == 0){
        ofLogError("Batch '"+id+"'::getBatch", "No batch found with id "+_id);
        return;
    }
    return batches[_id];
}

void ofxLoaderBatch::textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg){
    drawable[arg.tex] = true;
    ofLogNotice("Batch '"+id+"'",("Texture '"+ids[arg.tex] + "' drawable"));
}


void ofxLoaderBatch::textureReady(ofxProgressiveTextureLoad::textureEvent& arg){
    if (arg.loaded){
        ready[arg.tex] = true;
    }else{
        ofLogError("Batch '"+id+"'::textureReady",("Texture '"+ids[arg.tex] + "' load failed"));
    }
}

void ofxLoaderBatch::initTexture(string _id){
    if(filenames.count(_id) == 0){
        ofLogError("Batch '"+id+"'::initTexture", "No texture found with id "+_id);
        return;
    }
    
    ofTexture* t = new ofTexture(); //create your own texture to got data loaded into; it will be cleared!
    textures[_id] = t;
    ids[t] = _id;
    ready[t] = false;
    drawable[t] = false;
}

ofTexture* ofxLoaderBatch::loadTexture(string _id){
    if(textures.count(_id) == 0){
        ofLogError("Batch '"+id+"'::loadTexture", "No texture found with id "+_id);
        return NULL;
    }
    initTexture(_id);
    ofxProgressiveTextureLoad * loader = q->loadTexture(filenames[_id],
                                                        getTexture(_id),		/*tex to load into*/
                                                        true,				/*MIP-MAPS!*/
                                                        CV_INTER_AREA);		/*Resize Quality*/
    
    ofAddListener(loader->textureReady, this, &ofxLoaderBatch::textureReady);
    ofAddListener(loader->textureDrawable, this, &ofxLoaderBatch::textureDrawable);
    
    return getTexture(_id);
}

ofTexture* ofxLoaderBatch::getTexture(string _id){
    if(textures.count(_id) == 0){
        ofLogError("Batch '"+id+"'::getTexture", "No texture found with id "+_id);
        return;
    }
    return textures[_id];
}

bool ofxLoaderBatch::isTextureReady(string _id){
    return isTextureReady(textures[_id]);
}

bool ofxLoaderBatch::isTextureReady(ofTexture *tex){
    return ready[tex];
}

bool ofxLoaderBatch::isTextureDrawable(string _id){
    return isTextureDrawable(textures[_id]);
}

bool ofxLoaderBatch::isTextureDrawable(ofTexture *tex){
    return drawable[tex];
}

string ofxLoaderBatch::getTextureFilename(ofTexture *tex){
    return filenames[ids[tex]];
}

string ofxLoaderBatch::getTextureId(ofTexture *tex){
    return ids[tex];
}

void ofxLoaderBatch::clear(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _id =  iter->first;
        batches[_id]->clear();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _id =  iter->first;
        clearTexture(_id);
    }
}

void ofxLoaderBatch::load(){
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _id =  iter->first;
        batches[_id]->load();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _id =  iter->first;
        loadTexture(_id);
    }
}

bool ofxLoaderBatch::isDrawable(){
    bool isBatchDrawable = true;
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _id =  iter->first;
        isBatchDrawable = isBatchDrawable && batches[_id]->isDrawable();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _id =  iter->first;
        isBatchDrawable = isBatchDrawable && isTextureDrawable(_id);
    }
    return isBatchDrawable;
}

bool ofxLoaderBatch::isReady(){
    bool isBatchReady = true;
    for(map<string,ofxLoaderBatch*>::iterator iter = batches.begin(); iter != batches.end(); ++iter)
    {
        string _id =  iter->first;
        isBatchReady = isBatchReady && batches[_id]->isReady();
    }
    for(map<string,ofTexture*>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
    {
        string _id =  iter->first;
        isBatchReady = isBatchReady && isTextureReady(_id);
    }
    return isBatchReady;
}

void ofxLoaderBatch::clearTexture(ofTexture *tex){
    if(ids.count(tex) == 0){
        ofLogError("Batch '"+id+"'::clearTexture", "No texture found with given pointer");
        return;
    }

    clearTexture(ids[tex]);
}

void ofxLoaderBatch::clearTexture(string _id){
    if(textures.count(_id) == 0){
        ofLogError("Batch '"+id+"'::clearTexture", "No texture found with id "+_id);
        return;
    }
    
    ready[textures[_id]] = false;
    drawable[textures[_id]] = false;
    textures[_id]->clear();
    ofLogNotice("Batch '"+id+"'",("Texture '"+_id + "' cleared"));
}

ofxLoaderBatch::~ofxLoaderBatch(){
}

