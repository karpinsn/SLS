/**
@file
@author Nikolaus Karpinsky
@since  09/30/10
*/

#ifndef _SPATIAL_HOLOENCODER_H_
#define _SPATIAL_HOLOENCODER_H_

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <wrench/gl/Shader.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Camera.h>
#include <wrench/gl/utils/Arcball.h>
#include <wrench/gl/Texture.h>

#include "AbstractMesh.h"
#include "IGLContext.h"
#include "MeshInterchange.h"
#include "IEncoder.h"

using namespace std;
using namespace wrench::gl;

class SpatialHoloencoder : public IGLContext, public IEncoder
{
private:
  ShaderProgram m_encoderShader;
  ShaderProgram m_depthShader;
  ShaderProgram m_depth2HoloShader;

  bool m_draw2Holoimage;

  int m_width;
  int m_height;

  unique_ptr<Camera> m_camera;
  Arcball m_controller;

  glm::mat4 m_projectorModelView;
  glm::mat4 m_translate;
  glm::mat4 m_scale;

  shared_ptr<MeshInterchange> m_currentData;

  GLenum m_depthAttachPoint;
  GLenum m_holoimageAttachPoint;
  Texture m_depthMap;
  Texture m_holoimage;

  FBO m_offscreenFBO;

  bool m_hasBeenInit;

public:

  /**
  * Creates a new SpatialHoloencoder that can be used to encode any type of MeshInterchange
  */
  SpatialHoloencoder(void);

  /**
  *	Initializes the SpatialHoloencoder with default values (width and height = 512).
  */
  virtual void init();

  /**
  *	Initializes the SpatialHoloencoder with the specified values.
  *
  *	@param width Width of the encoded holoimage (Does not need to match the input mesh)
  * @param height Height of the encoded holoimage (Does not need to match the input mesh)
  */
  virtual void init(int width, int height);

  /**
  *	Draw method that encodes the holoimage and displays. This may only be called from the OpenGL draw thread.
  */
  virtual void draw(void);

  /**
  * Resizes the holoencoder scene. Typically an OpenGL callback when 
  * the user changes the canvas size
  *
  *	@param width Width of the scene
  * @param height Height of the scene
  */
  virtual void resize(int width, int height);
  virtual void cameraSelectMode(int mode);
  virtual void mousePressEvent(int mouseX, int mouseY);
  virtual void mouseMoveEvent(int mouseX, int mouseY);

  /**
  *	Sets the mesh to encode. 
  *
  *	@param current shared_ptr to a MeshInterchange to encode
  */
  void setCurrentMesh(shared_ptr<MeshInterchange> current);

  /**
  * Encodes the current mesh. This may only be called from the OpenGL draw 
  * thread. The resulting Holoencoded scene will be rendered to a texture 
  * which can then be fetched using getEncodedData()
  */
  void encode(void);

  /**
  * Returns the last encoded scene as a mesh interchange
  *
  *	@return Last Holoencoded MeshInterchange
  */
  MeshInterchange* getEncodedData(void);

  /**
  *	Attempts to scale and translate the scene to fit the geometry to the current mesh
  * that was set using setCurrentMesh()
  */
  void autoFitTransforms(void);

private:
  // Initializes the frame buffer object used in the holoencoding process
  void _initFBO(void);
  // Initializes the shaders used in the holoencoding process
  void _initShaders(void);

};

#endif	// _HOLOENCODER_H_
