/*
 Filename:	MultiWavelengthCapture.h
 Author:	Nikolaus Karpinsky
 Date Created:	01/30/11
 Last Edited:	01/30/11
 
 Revision Log:
 01/30/11 - Nik Karpinsky - Original creation.
 */

#ifndef _MULTI_WAVELENGTH_CAPTURE_H_
#define _MULTI_WAVELENGTH_CAPTURE_H_

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

#include "TriMesh.h"
#include "PointCloudMesh.h"
#include "ImageIO.h"
#include "AbstractGLContext.h"

#include <wrench/gl/Camera.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/FBO.h>
#include <wrench/gl/AxisDisplay.h>

#include "OpenGLWidget.h"
#include "CameraGLContext.h"
#include <wrench/gl/OGLStatus.h>

#include <wrench/gl/Arcball.h>

class OpenGLWidget;

using namespace wrench;
using namespace wrench::gl;

class MultiWavelengthCapture : public AbstractGLContext
{
private:
  AxisDisplay m_axis;

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

  IplImage *m_fringeLoadingImage;
  Texture* m_fringeImages[2][3];  // Buffer and Images
  Texture m_fringeImage1; //  Fringe images for the front buffer
  Texture m_fringeImage2;
  Texture m_fringeImage3;

  Texture m_fringeImage4; //  Fringe images for the back buffer
  Texture m_fringeImage5;
  Texture m_fringeImage6;

  Texture m_referencePhase;
  Texture m_phaseMap0;
  Texture m_phaseMap1;
  Texture m_normalMap;

  FBO m_imageProcessor;

  CameraGLContext m_textureDisplay;
  bool m_displayPhase;
  bool m_displayFringe;

  Camera m_camera;
  Arcball m_controller;

  TriMesh* m_mesh;

  bool haveFringeImages;
  bool m_haveReferencePhase;
  bool m_captureReferencePhase;
  int m_currentFringeLoad;
  int m_currentChannelLoad;
  float m_gammaCutoff;

  bool m_hasBeenInit;

  int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds

  OpenGLWidget* m_glContext;

  bool m_show3D;
  bool m_showPhase;
  bool m_showFringe;

public:
  MultiWavelengthCapture(void);
  ~MultiWavelengthCapture();

  virtual void init(void);
  virtual void draw(void);
  virtual void resize(int width, int height);
  virtual void cameraSelectMode(int mode);
  virtual void mousePressEvent(int mouseX, int mouseY);
  virtual void mouseMoveEvent(int mouseX, int mouseY);

  void resizeInput(float width, float height);
  void newImage(IplImage* image);
  void swapBuffers(void);
  void captureReferencePlane(void);
  void setGammaCutoff(float gamma);
  void show3D(void);
  void showPhase(void);
  void showFringe(void);

private:
  void _initShaders(float width, float height);
  void _initTextures(GLuint width, GLuint height);
  void _initLighting(void);
};

#endif	// _MULTI_WAVELENGTH_CAPTURE_H_
