#ifndef _XYZM_Decoder_H_
#define _XYZM_Decoder_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "XYZFileIO.h"
#include "XYZMesh.h"

using namespace std;

class XYZMDecoder : public Codec
{
private:
  XYZFileIO m_io;
  EncodingOpenGLWidget* m_glWidget;

  const QListWidget *m_xyzmList;
  int m_listPosition;

public:
  XYZMDecoder(const QListWidget* xyzmList);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);

  int getWidth(void);
  int getHeight(void);
  float getStreamLocation(void);

  static string codecName(void);
};

#endif  //_XYZM_Decoder_H_
