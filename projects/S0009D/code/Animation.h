#pragma once
#include "Vector4D.h"
#include <fstream>

namespace CoreAnimation
{
#pragma pack(push, 1)

#define NEBULA3_NAX3_MAGICNUMBER 'NA01'

    //------------------------------------------------------------------------------
    /**
        NAX3 file format structs.
        NOTE: keep all header-structs 4-byte aligned!
    */
    struct Nax3Header
    {
        unsigned int magic;
        unsigned int numClips;
        unsigned int numKeys;
    };

    struct Nax3Clip
    {
        unsigned short numCurves;
        unsigned short startKeyIndex;
        unsigned short numKeys;
        unsigned short keyStride;
        unsigned short keyDuration;
        unsigned char preInfinityType;          // CoreAnimation::InfinityType::Code
        unsigned char postInfinityType;         // CoreAnimation::InfinityType::Code
        unsigned short numEvents;
        char name[50];                          // add up to 64 bytes size for Nax3Clip
    };

    struct Nax3AnimEvent
    {
        char name[47];
        char category[15];
        unsigned short keyIndex;
    };

    struct Nax3Curve
    {
        unsigned int firstKeyIndex;
        unsigned char isActive;                 // 0 or 1
        unsigned char isStatic;                 // 0 or 1
        unsigned char curveType;                // CoreAnimation::CurveType::Code
        unsigned char _padding;                 // padding byte
        float staticKeyX;
        float staticKeyY;
        float staticKeyZ;
        float staticKeyW;

    };

#pragma pack(pop)
} // namespace CoreAnimation

class Curve
{
public:
    unsigned int firstKeyIndex;
    bool isActive;
    bool isStatic;
    unsigned int curveType;
    Vector4D staticKey;
};

class Clip
{
public:
    unsigned short numCurves;
    unsigned short startKeyIndex;
    unsigned short numKeys;
    unsigned short keyStride;
    unsigned short keyDuration;
    unsigned char preInfinityType;
    unsigned char postInfinityType;
    unsigned short numEvents;
    char* name;
    Curve* curves;
};
typedef Vector4D* keys;

using namespace std;
using namespace CoreAnimation;
class Animation
{
public:
	Animation() { }
	~Animation() { }
	Vector4D getKey(unsigned int clipIndex, float i, unsigned int curveIndex, unsigned int type)
	{
        int flooredI = (int)floor(i);
        float diff = i - flooredI;

        Clip clip = clips[clipIndex];
        Curve cu = clip.curves[curveIndex];
        unsigned int curve = cu.firstKeyIndex;

        if (type == 0) // lerp for vectors.
            return Vector4D::Lerp( keyBuffer[curve + (flooredI + 0) % clip.numKeys * clip.keyStride], keyBuffer[curve + (flooredI + 1) % clip.numKeys * clip.keyStride], diff);
        else if (type == 1) // slerp for quaternions.
            return Vector4D::Slerp(keyBuffer[curve + (flooredI + 0) % clip.numKeys * clip.keyStride], keyBuffer[curve + (flooredI + 1) % clip.numKeys * clip.keyStride], diff);
        return keyBuffer[curve + (flooredI + 0) % clip.numKeys * clip.keyStride];
	}

	void loadAnimations(char* filename)
    {
        ifstream file;
        file.open(filename, ifstream::in | ifstream::binary);
        file.seekg(0, file.end);
        unsigned int length = file.tellg();
        file.seekg(0, file.beg);
        char* ptr = new char[length];
        file.read(ptr, length);
        file.close();



        Nax3Header* h = (Nax3Header*)ptr;
        header = h;
        keyBuffer = (Vector4D*)malloc(h->numKeys * sizeof(Vector4D));
        ptr += sizeof(Nax3Header);

        clips = new Clip[h->numClips];
        for (int i = 0; i < h->numClips; ++i) {
            Nax3Clip* naxClip = (Nax3Clip*)ptr;
            Clip cl;
            cl.numCurves = naxClip->numCurves;
            cl.startKeyIndex = naxClip->startKeyIndex;
            cl.numKeys = naxClip->numKeys;
            cl.keyStride = naxClip->keyStride;
            cl.keyDuration = naxClip->keyDuration;
            cl.preInfinityType = naxClip->preInfinityType;
            cl.postInfinityType = naxClip->postInfinityType;
            cl.name = naxClip->name;
            ptr += sizeof(Nax3Clip);


            // move the pointer to skip the events since they are not used
            for (int j = 0; j < naxClip->numEvents; ++j) {
                ptr += sizeof(Nax3AnimEvent);
            }


            // Add the curves to the clips
            unsigned int curveIndex;
            cl.curves = new Curve[cl.numCurves];
            for (curveIndex = 0; curveIndex < cl.numCurves; curveIndex++)
            {
                Nax3Curve* naxCurve = (Nax3Curve*)ptr;
                ptr += sizeof(Nax3Curve);
                Curve cu;
                cu.firstKeyIndex = naxCurve->firstKeyIndex;
                cu.isActive = naxCurve->isActive;
                cu.isStatic = naxCurve->isStatic;
                cu.curveType = naxCurve->curveType;
                cu.staticKey = Vector4D(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
                cl.curves[curveIndex] = cu;

            }
            clips[i] = cl;
            // load the blob of data that contains the keyframes
            memcpy(keyBuffer, ptr, h->numKeys * sizeof(Vector4D));
        }
    }

    Nax3Header* header;

    Clip* clips;
    keys keyBuffer;

private:

};