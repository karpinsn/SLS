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
#include "IDecodingGLContext.h"

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

class OpenGLWidget;

using namespace wrench;
using namespace wrench::gl;
using namespace wrench::gl::utils;

class SixFringeCapture : public IDecodingGLContext, public ICapture
{
private:
  AxisDisplay m_axis;

  ShaderProgram m_gaussianFilterVertical;
  ShaderProgram m_gaussianFilterHorizontal;

  ShaderProgram m_phaseCalculator;
  ShaderProgram m_phaseFilter;
  ShaderProgram m_depthCalculator;
  ShaderProgram m_normalCalculator;
  ShaderProgram m_finalRender;

  GLenum m_phaseMap0AttachPoint;
  GLenum m_phaseMap1AttachPoint;
  GLenum m_depthMapAttachPoint;
  GLenum m_normalMapAttachPoint;
  GLenum m_referencePhaseAttachPoint;

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
  Texture m_phaseMap3;
  Texture m_depthMap;
  Texture m_normalMap;

  FBO m_imageProcessor;
  TextureDisplay m_textureDisplay;

  Camera m_camera;
  Arcball m_controller;
  shared_ptr<TriMesh> m_mesh;

  FPSCalculator m_fpsCalculator;
  FPSCalculator m_3dpsCalculator; // 3D frames per second

  bool haveFringeImages;
  bool m_haveReferencePhase;
  bool m_captureReferencePhase;
  int m_currentFringeLoad;
  int m_currentChannelLoad;
  float m_gammaCutoff;
  float m_scalingFactor;

  bool m_hasBeenInit;

  int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds

  OpenGLWidget* m_glContext;

  enum DisplayMode
  {
    Geometry,
    Phase
  };
  DisplayMode m_displayMode;

public:
  SixFringeCapture(void);
  ~SixFringeCapture();

  virtual void init(void);
  virtual void draw(void);
  virtual void resize(int width, int height);
  virtual void cameraSelectMode(int mode);
  virtual void mousePressEvent(int mouseX, int mouseY);
  virtual void mouseMoveEvent(int mouseX, int mouseY);

  void    init(float width, float height);
  void    resizeInput(float width, float height);
  bool    newImage(IplImage* image);
  void    swapBuffers(void);
  void    captureReferencePlane(void);
  void    setGammaCutoff(float gamma);
  void    setScalingFactor(float scalingFactor);
  void    show3D(void);
  void    showPhase(void);
  double  getFrameRate(void);
  double  get3DRate(void);
  MeshInterchange* decode(void);

  static string getCaptureName(void);

private:
  void _initShaders(float width, float height);
  void _initTextures(GLuint width, GLuint height);
  void _initLighting(void);

  void _drawCalculatePhase();
  void _drawFilterPhase();
  void _drawCalculateDepthMap();
  void _drawCalculateNormalMap();
};

#endif	// _SIX_FRINGE_CAPTURE_H_
