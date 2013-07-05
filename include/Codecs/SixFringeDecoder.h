#ifndef _SIX_FRINGE_DECODER_H_
#define _SIX_FRINGE_DECODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"
#include "Capture\SixFringeCapture.h"

using namespace std;

class SixFringeDecoder : public Codec
{
private:
  VideoIO				  m_io;
  SixFringeCapture		  m_coder;
  bool					  m_calculateReference;
  EncodingOpenGLWidget*	  m_glWidget;
  string				  m_filename;

  shared_ptr<IplImage>	  m_shortReference;
  shared_ptr<IplImage>	  m_longReference;
  int					  m_returnImage;

public:  
  SixFringeDecoder(string& filename);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);

  int getWidth(void);
  int getHeight(void);
  float getStreamLocation(void);

  void setReferencePlane(shared_ptr<IplImage> shortWavelength, shared_ptr<IplImage> longWavelength);
  void setGammaCutoff(float gammaValue);
  void setScalingFactor(float scaling);

  static string codecName(void);

private:
  bool _streamUntilNewFrame(void);
  static shared_ptr<IplImage> _referenceFrameCallback(void* callbackInstance);
  shared_ptr<IplImage> _referenceFrame(int returnImage);
};

#endif  //_SIX_FRINGE_DECODER_H_
