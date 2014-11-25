//
//  ofxLoaderBatch.h
//
//  Created by  Tyler Parker on 11/17/14.
//
//

#ifndef __ofxLoaderBatch__
#define __ofxLoaderBatch__

#include "ofMain.h"
#include "ProgressiveTextureLoadQueue.h"

class ofxLoaderBatch{
public:
    
    
    
    // FUNCTIONS //
    
    ofxLoaderBatch();
    ofxLoaderBatch(string _id);
    ~ofxLoaderBatch();
    
    string getId();
    void setId(string _id);
    
    ProgressiveTextureLoadQueue * getParentLoadQueue();
    void setParentLoadQueue(ProgressiveTextureLoadQueue * _q);
    
    
    bool isDrawable();
    bool isReady();
    
    void load();
    void clear();
    
    // Textures
    void addTexture(string _filename, string _textureId);
    void initTexture(string _id);
    
    ofTexture* getTexture(string _textureId);
    bool hasTexture(string _textureId);
    string getTextureFilename(ofTexture * tex);
    string getTextureId(ofTexture * tex);
    
    ofTexture* loadTexture(string _textureId);
    
    void textureReady(ofxProgressiveTextureLoad::textureEvent& arg);
    void textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg);
    
    bool isTextureReady(string _textureId);
    bool isTextureReady(ofTexture * tex);
    bool isTextureDrawable(string _textureId);
    bool isTextureDrawable(ofTexture * tex);

    void clearTexture(string _textureId);
    void clearTexture(ofTexture * tex);
    
    // Batches
    ofxLoaderBatch* addBatch(string _id);
    ofxLoaderBatch* addBatch(ofxLoaderBatch* _batch);
    ofxLoaderBatch* getBatch(string _id);
    
    
    
private:
    // FIELDS //
    string id;
    
    // Yo dawg, I heard  you like ofxLoaderBatches...
    map<string, ofxLoaderBatch*> batches;
    
    map<ofTexture*, bool> ready;
    map<ofTexture *, bool> drawable;
    
    // String key is id (which can default to the image filename)
    map<string, ofTexture*> textures;
    
    // <ofTexture*, textureId>
    map<ofTexture*, string> ids;
    
    // <textureId, filename>
    map<string, string> textureFilenames;
    
    // From the parent ofxLoaderSpool
    ProgressiveTextureLoadQueue * q;
};


#endif /* defined(__ofxLoaderBatch__) */
