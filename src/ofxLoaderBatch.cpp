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

ProgressiveTextureLoadQueue * ofxLoaderBatch::getParentLoadQueue(){
    return q;
}

void ofxLoaderBatch::setParentLoadQueue(ProgressiveTextureLoadQueue *_q){
    q = _q;
}

string ofxLoaderBatch::getId(){
    return id;
}

ofxLoaderBatch* ofxLoaderBatch::addBatch(string _batchId){
    ofxLoaderBatch* batch = new ofxLoaderBatch(_batchId);
    return addBatch(batch);
}

ofxLoaderBatch* ofxLoaderBatch::addBatch(ofxLoaderBatch* _batch){
    if(batches.count(_batch->getId()) > 0){
        ofLogError("Batch '"+getId()+"'::addBatch", "A batch with id "+_batch->getId()+" already exists!");
        return;
    }
    batches[_batch->getId()] = _batch;
    batches[_batch->getId()]->setParentLoadQueue(getParentLoadQueue());
    return batches[_batch->getId()];
}

void ofxLoaderBatch::addTexture(string _textureFilename, string _textureId = ""){
    if(_textureId.length() == 0){
        _textureId = _textureFilename;
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
        return;
    }
    return batches[_batchId];
}

void ofxLoaderBatch::textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg){
    drawable[arg.tex] = true;
    ofLogNotice("Batch '"+getId()+"'",("Texture '"+ids[arg.tex] + "' drawable"));
}


void ofxLoaderBatch::textureReady(ofxProgressiveTextureLoad::textureEvent& arg){
    if (arg.loaded){
        ready[arg.tex] = true;
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
    drawable[t] = false;
}

ofTexture* ofxLoaderBatch::loadTexture(string _textureId){
    if(textures.count(_textureId) == 0){
        ofLogError("Batch '"+getId()+"'::loadTexture", "No texture found with id "+_textureId);
        return NULL;
    }
    initTexture(_textureId);
    ofxProgressiveTextureLoad * loader = q->loadTexture(textureFilenames[_textureId],
                                                        getTexture(_textureId),     /*tex to load into*/
                                                        true,               /*MIP-MAPS!*/
                                                        CV_INTER_AREA);     /*Resize Quality*/

    ofLogNotice("Batch '"+getId()+"'",("Texture '"+_textureId + "' loading..."));
    ofAddListener(loader->textureReady, this, &ofxLoaderBatch::textureReady);
    ofAddListener(loader->textureDrawable, this, &ofxLoaderBatch::textureDrawable);
    
    return getTexture(_textureId);
}

ofTexture* ofxLoaderBatch::getTexture(string _textureId){
    if(textures.count(_textureId) == 0){
        ofLogError("Batch '"+getId()+"'::getTexture", "No texture found with id "+_textureId);
        return;
    }
    return textures[_textureId];
}

bool ofxLoaderBatch::isTextureReady(string _textureId){
    return isTextureReady(textures[_textureId]);
}

bool ofxLoaderBatch::isTextureReady(ofTexture *tex){
    return ready[tex];
}

bool ofxLoaderBatch::isTextureDrawable(string _textureId){
    return isTextureDrawable(textures[_textureId]);
}

bool ofxLoaderBatch::isTextureDrawable(ofTexture *tex){
    return drawable[tex];
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
    drawable[textures[_textureId]] = false;
    if(textures[_textureId]->isAllocated()){
        textures[_textureId]->clear();
    }
    ofLogNotice("Batch '"+getId()+"'",("Texture '"+_textureId + "' ('"+textureFilenames[_textureId]+"') cleared"));
}

ofxLoaderBatch::~ofxLoaderBatch(){
}

