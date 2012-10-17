#ifndef _VIDEO_IO_H_
#define _VIDEO_IO_H_

#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

#include <wrench/gl/Texture.h>

#include "MeshInterchange.h"

using namespace std;
using namespace wrench::gl;

class VideoIO
{
private:
    shared_ptr<CvVideoWriter> m_videoWriterHandle;
    bool m_videoWriterInUse;

    shared_ptr<CvCapture> m_videoReaderHandle;
    bool m_videoReaderInUse;

    shared_ptr<IplImage> m_imageHandle;
	shared_ptr<IplImage> m_floatImageHandle;

    unsigned int m_imageHeight;
    unsigned int m_imageWidth;

public:
    VideoIO(void);
    ~VideoIO();

    //	Save Avi File Methods
    bool openSaveStream(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps);
    bool saveStream(Texture& texture);
	bool saveStream(IplImage* image);
    bool saveStream(MeshInterchange& mesh);
    bool closeSaveStream(void);

    //	Read Avi File Methods
    bool openReadStream(const string &filename);
    IplImage* readStream();
    bool closeReadStream(void);
    bool readStreamIsOpen(void);
	int readStreamWidth(void);
	int readStreamHeight(void);
	float readStreamPosition(void);
	void setReadStreamPosition(float position);

private:
    void ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount);
	int _getTextureDepth(Texture& texture);
};

#endif  //_VIDEO_IO_H_
