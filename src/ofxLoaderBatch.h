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
    
    ofxLoaderBatch(string _id);
    ~ofxLoaderBatch();
    
    void setParentLoadQueue(ProgressiveTextureLoadQueue * _q);
    
    bool isDrawable();
    bool isReady();
    
    void load();
    void clear();
    
    // Textures
    void addTexture(string _filename, string _id);
    void initTexture(string _id);
    
    ofTexture* getTexture(string _id);
    string getTextureFilename(ofTexture * tex);
    string getTextureId(ofTexture * tex);
    
    ofTexture* loadTexture(string _id);
    
    void textureReady(ofxProgressiveTextureLoad::textureEvent& arg);
    void textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg);
    
    bool isTextureReady(string _id);
    bool isTextureReady(ofTexture * tex);
    bool isTextureDrawable(string _id);
    bool isTextureDrawable(ofTexture * tex);

    void clearTexture(string _id);
    void clearTexture(ofTexture * tex);
    
    // Batches
    void addBatch(string _id);
    ofxLoaderBatch* getBatch(string _id);
    
    // FIELDS //
    string id;
    
    // Yo dawg, I heard  you like ofxLoaderBatches...
    map<string, ofxLoaderBatch*> batches;
    
    map<ofTexture*, bool> ready;
    map<ofTexture *, bool> drawable;
    
    // String key is id (which can default to the image filename)
    map<string, ofTexture*> textures;
    
    // <ofTexture*, id>
    map<ofTexture*, string> ids;
    
    // <id, filename>
    map<string, string> filenames;
    
    // From the parent ofxLoaderSpool
    ProgressiveTextureLoadQueue * q;
};


#endif /* defined(__ofxLoaderBatch__) */
