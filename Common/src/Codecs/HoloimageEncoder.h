#ifndef _HOLOIMAGE_ENCODER_H_
#define _HOLOIMAGE_ENCODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

#include "Holodecoder.h"
#include "Holoencoder.h"

using namespace std;

class HoloimageEncoder : public Codec
{
private:
  Holoencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  string m_filename;

  IplImage* m_image;

public:
  HoloimageEncoder();
  ~HoloimageEncoder();
  void initCodec(string& filename, int width, int height);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  void process(MeshInterchange* data);
  void previewProcess(MeshInterchange* data);
  void autoFitData(void);
  static string codecName(void);
};

#endif  //_HOLOIMAGE_ENCODER_H_
