/*
 Filename:	SixFringeCapture.h
 Author:	Nikolaus Karpinsky
 Date Created:	09/19/12
 Last Edited:	09/19/12
 
 Revision Log:
 09/19/12 - Nik Karpinsky - Original creation.
 */

#ifndef _SIX_FRINGE_CAPTURE_H_
#define _SIX_FRINGE_CAPTURE_H_

#ifdef __APPLE__
#include <glew.h>
#include <OpenGL/gl.h>
#elif _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <qsemaphore.h>
#include <QMutex>
#include <cv.h>

#include "Capture\ICapture.h"

#include "TriMesh.h"
#include "PointCloudMesh.h"
#include "ImageIO.h"
#include "IGLContext.h"
#include "IDecoder.h"

#include <wrench/gl/Camera.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
#include <Wrench/gl/GaussProgram.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/FBO.h>

#include <wrench/gl/utils/AxisDisplay.h>
#include <wrench/gl/utils/FPSCalculator.h>

#include "OpenGLWidget.h"

#include <wrench/gl/OGLStatus.h>

#include <wrench/gl/utils/TextureDisplay.h>
#include <wrench/gl/utils/Arcball.h>

using namespace wrench;
using namespace wrench::gl;
using namespace wrench::gl::utils;

class SixFringeCapture : public ICapture
{
private:

  ShaderProgram m_phaseWrapper;
  ShaderProgram m_phaseUnwrapper;
  GaussProgram  m_gaussFilter;
  ShaderProgram m_phaseFilter;
  ShaderProgram m_depthCalculator;
  ShaderProgram m_normalCalculator;

  GLenum m_phaseMap0AttachPoint;
  GLenum m_phaseMap1AttachPoint;
  GLenum m_phaseMap2AttachPoint;
  GLenum m_depthMapAttachPoint;
  GLenum m_normalMapAttachPoint;
  GLenum m_referencePhaseAttachPoint;
  GLenum m_textureMapAttachPoint;

  shared_ptr<IplImage> m_fringeLoadingImage;
  Texture* m_fringeImages[2][2];  // Buffer and Images
  Texture m_fringeImage1; //  Fringe images for the front buffer
  Texture m_fringeImage2;
  Texture m_fringeImage3; //  Fringe images for the back buffer
  Texture m_fringeImage4;

  Texture m_referencePhase;
  Texture m_phaseMap0;
  Texture m_phaseMap1;
  Texture m_phaseMap2;
  Texture m_depthMap;
  Texture m_normalMap;
  Texture m_textureMap;

  FBO m_imageProcessor;

  FPSCalculator m_fpsCalculator;
  FPSCalculator m_3dpsCalculator; // 3D frames per second

  bool haveFringeImages;
  bool m_haveReferencePhase;
  bool m_captureReferencePhase;
  int m_currentFringeLoad;
  int m_currentChannelLoad;
  float m_gammaCutoff;
  float m_blackLevel;
  float m_scalingFactor;
  float m_shiftFactor;
  float m_width;
  float m_height;

  bool m_hasBeenInit;

  int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds

public:
  SixFringeCapture(void);

  virtual void init(void);
  virtual void draw(void);
  
  int	  getWidth();
  int	  getHeight();
  void    init(int width, int height);
  void    resizeInput(int width, int height);
  int	  getFringeCount();
  bool    newImage(IplImage* image);
  void    swapFringeBuffers(void);
  void	  loadReferencePlane(void* callbackInstance, shared_ptr<IplImage> (*imageLoaderFunction)(void* callbackInstance));
  void    captureReferencePlane(void);

  Texture& getDepthMap(void);
  Texture& getTextureMap(void);
  Texture& getNormalMap(void);
  void    setGammaCutoff(float gamma);
  void	  setBlackLevel(float blackLevel);
  void    setScalingFactor(float scalingFactor);
  void	  setShiftFactor(float shiftFactor);
  void	  setSaveStream(shared_ptr<SaveStream> saveStream);
  double  getFrameRate(void);
  double  get3DRate(void);
  void	  decode(void);

  static string getCaptureName(void);

private:
  void _initShaders(float width, float height);
  void _initTextures(GLuint width, GLuint height);

  void _wrapPhase( GLenum* drawBuffers );
  void _gaussianFilter( GLenum drawBuffer1, GLenum drawBuffer2, Texture& readBuffer1, Texture& readBuffer2 );
  void _unwrapPhase( GLenum drawBuffer, Texture& unfilteredPhase, Texture& filteredPhase );
  void _filterPhase( GLenum drawBuffer, Texture& readBuffer );
  void _calculateDepthMap( Texture& unwrappedPhase );
  void _calculateNormalMap( );
};

#endif	// _SIX_FRINGE_CAPTURE_H_
