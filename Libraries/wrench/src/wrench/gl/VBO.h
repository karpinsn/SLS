/**
  * @file
  * @author Nikolaus Karpinsky
  * @date   10/27/10
  *
  * Class that represents a vertex buffer object in OpenGL
  *
  * Class that represents a Vertex Buffer Object (VBO) in OpenGL. Typicall usage involves
  * initalizing with init(), sending the vertex data to the GPU using bufferData(),
  * and then adding it to a Vertex Array Object (VAO) to be drawn with the other VBOs
  */

#ifndef _WRENCH_GL_VBO_H_
#define _WRENCH_GL_VBO_H_

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

#include "../Logger.h"
#include "OGLStatus.h"
#include "Converter.h"

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

namespace wrench 
{
namespace gl
{
class VBO
{
private:
#ifdef USE_VRJ
  vrj::opengl::ContextData<GLuint>  vrjVBOID;
#define m_vboID                   (*vrjVBOID)
#else
  GLuint m_vboID;
#endif

  GLint   m_componentSize;    // Defined in glVertexAttribPointer as size
  GLenum  m_componentType;    // Defined in glVertexAttribPointer as type
  GLenum  m_target;           // Defined in glBufferData as target
public:

  /**
    * Default constructor for the VBO.
    *
    * Default constructor for the VBO. After creation, init() must be called before
    * the VBO is added to a VAO.
    */
  VBO(void);

  /**
    * Destructor for the VBO which frees the VBO on the GPU if it has been initalized.
    *
    * Destructor for the VBO which frees the VBO on the GPU if it has been initalized.
    * If the VBO has not been initalized then nothing is done in the destructor.
    */
  ~VBO();

  /**
    *  Initalizes the VBO.
    *
    *  @param compSize - Number of components per vertex. Defined in glVertexAttribPointer as size.
    *  @param type - Component type. Defined in glVertexAttribPointer as type.
    *  @param target - Defined in glBufferData as target.
    *
    *  @return True or false based on whether or not initalization was successful.
    */
  bool init(GLint compSize, GLenum type, GLenum target);  // Here compSize is the number of components per vertex

  /**
    *  Buffers the data to the GPU.
    *
    *  @param size - Size of the data to buffer over.
    *  @param data - Pointer to the first element of the data.
    *  @param usage - Defined in glBufferData as usage.
    */
  void bufferData(GLsizei size, const GLvoid* data, GLenum usage);

  /**
    *	Returns the number of components per vertex. Defined in glVertexAttribPointer as size.
    *
    *  @return Components per vertex. Defined in glVertexAttribPointer as size.
    */
  GLint getComponentSize(void);

  /**
    *  Returns the type of the components. Defined in glVertexAttribPointer as type.
    *
    *  @return Type of component used by this VBO.
    */
  GLenum getComponentType(void);

  /**
    *  Returns the target of the VBO.
    *
    *  @return Target used by this VBO
    */
  GLenum getTarget(void);

  /**
    * Binds the VBO as the current VBO
    */
  void bind(void);

  /**
    * Unbinds the VBO, binding 0 as the current VBO
    */
  void unbind(void);
};
}
}

#endif	// _WRENCH_GL_VBO_H_
