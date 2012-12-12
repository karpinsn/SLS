 /*
 Filename:	ThreeFringeCapture.h
 Author:	Nikolaus Karpinsky
 Date Created:	07/22/12
 Last Edited:	07/22/12
 
 Revision Log:
 01/30/11 - Nik Karpinsky - Original creation.
 */

#ifndef _THREE_FRINGE_CAPTURE_H_
#define _THREE_FRINGE_CAPTURE_H_

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

#include <QMutex>
#include <cv.h>

#include "Capture\ICapture.h"

#include "TriMesh.h"
#include "PointCloudMesh.h"
#include "ImageIO.h"
#include "IGLContext.h"

#include <wrench/gl/Camera.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
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

class ThreeFringeCapture : public ICapture
{
private:
  ShaderProgram m_phaseCalculator;
  ShaderProgram m_phaseFilter;
  ShaderProgram m_depthCalculator;
  ShaderProgram m_normalCalculator;

  GLenum m_phaseMap0AttachPoint;
  GLenum m_phaseMap1AttachPoint;
  GLenum m_depthMapAttachPoint;
  GLenum m_normalMapAttachPoint;
  GLenum m_referencePhaseAttachPoint;

  shared_ptr<IplImage> m_fringeLoadingImage;
  Texture* m_fringeImages[2];  // Buffer and Images
  Texture m_fringeImage1; //  Fringe image for the front buffer
  Texture m_fringeImage2; //  Fringe image for the back buffer
  
  Texture m_referencePhase;
  Texture m_phaseMap0;
  Texture m_phaseMap1;
  Texture m_depthMap;
  Texture m_normalMap;

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

  bool m_hasBeenInit;

  int m_width;
  int m_height;

  int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds

public:
  ThreeFringeCapture(void);

  virtual void init(void);
  virtual void draw(void);

  int	  getWidth();
  int	  getHeight();
  void    init(int width, int height);
  void    resizeInput(int width, int height);
  int	  getFringeCount();
  bool    newImage(IplImage* image);
  void    swapBuffers(void);
  void	  loadReferencePlane(void* callbackInstance, shared_ptr<IplImage> (*imageLoaderFunction)(void* callbackInstance));
  Texture& getDepthMap(void);
  Texture& getTextureMap(void);
  Texture& getNormalMap(void);
  void    captureReferencePlane(void);
  void    setGammaCutoff(float gamma);
  void	  setBlackLevel(float blackLevel);
  void	  setShiftFactor(float shiftFactor);
  void    setScalingFactor(float scalingFactor);
  double  getFrameRate(void);
  double  get3DRate(void);
  void	  decode(void);
  static string getCaptureName(void);

private:
  void _initShaders(float width, float height);
  void _initTextures(GLuint width, GLuint height);

  void _drawCalculatePhase();
  void _drawFilterPhase();
  void _drawCalculateDepthMap();
  void _drawCalculateNormalMap();
};

#endif	// _THREE_FRINGE_CAPTURE_H_
