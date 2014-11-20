//
//  ofxLoaderSpool.h
//  ofxLoaderSpool
//
//  Created by  Tyler Parker on 11/14/14.
//
//

#ifndef __ofxLoaderSpool__ofxLoaderSpool__
#define __ofxLoaderSpool__ofxLoaderSpool__

#include "ofMain.h"
#include "ProgressiveTextureLoadQueue.h"
#include "ofxMSATimer.h"
#include "ofxLoaderBatch.h"


class ofxLoaderSpool{
    public:
        ofxLoaderSpool();
        ~ofxLoaderSpool();
        void setup();
        void update();

        void addBatch(string id);
        void addImageToBatch(string id, string imageFilename);
        void clearBatch(string id);
        void loadBatch(string id);
        bool isBatchReady(string id);
        bool isBatchDrawable(string id);
        int getBatchesSize();
        ofxLoaderBatch* getBatch(string id);

        ofTexture* getTexture(string imageFilename);

    private:
        void textureReady(ofxProgressiveTextureLoad::textureEvent& arg);
        void textureDrawable(ofxProgressiveTextureLoad::textureEvent& arg);

        bool isTextureReady(string imageFilename);
        bool isTextureReady(ofTexture * tex);

        bool isTextureDrawable(string imageFilename);
        bool isTextureDrawable(ofTexture * tex);

        string getTextureFilename(ofTexture * tex);

        ofTexture* loadImage(string imageFilename);
        void clearImage(string imageFilename);
        void clearImage(ofTexture * tex);

        ProgressiveTextureLoadQueue * q;
        map<string, ofTexture*> textures;
        map<string, ofxLoaderBatch*> batches;
        map<ofTexture*, bool> ready;
        map<ofTexture *, bool> drawable;
        map<ofTexture *, ofxProgressiveTextureLoad *> loaders;
        map<ofTexture*, string> imageFilenames;

};

#endif /* defined(__ofxLoaderSpool__ofxLoaderSpool__) */