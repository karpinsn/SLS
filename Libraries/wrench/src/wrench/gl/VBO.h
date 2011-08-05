/**
 @filename		VBO.h
 @author		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	10/27/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/09/10 - Nik Karpinsky - Moved file into the Wrench library
 */

#ifndef _WRENCH_GL_VBO_H_
#define _WRENCH_GL_VBO_H_

//  Used to calculate buffer offsets for the buffer object pointers
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
#define m_vboID             (*vrjVBOID)
#else
      GLuint m_vboID;
#endif

      GLint   m_componentSize;    // Defined in glVertexAttribPointer as size
      GLenum  m_componentType;    // Defined in glVertexAttribPointer as type
      GLenum  m_target;           // Defined in glBufferData as target
    public:
      VBO(void);
      ~VBO();

	  /**
	  *	Initalizes the VBO.
	  * 
	  *	@param compSize - Number of components per vertex. Defined in glVertexAttribPointer as size.
	  *	@param type - Component type. Defined in glVertexAttribPointer as type.
	  * @param target - Defined in glBufferData as target.
	  *
	  * @return True or false based on whether or not initalization was successful.
	  */
      bool init(GLint compSize, GLenum type, GLenum target);  // Here compSize is the number of components per vertex

	  /**
	  * Buffers the data to the GPU.
	  * 
	  *	@param size - Size of the data to buffer over.
	  * @param data - Pointer to the first element of the data.
	  * @param usage - Defined in glBufferData as usage.
	  */
      void bufferData(GLsizei size, const GLvoid* data, GLenum usage);

	  /**
	  *	Returns the number of components per vertex. Defined in glVertexAttribPointer as size.
	  */
      GLint getComponentSize(void);
      
      /**
      *	Returns the type of the components. Defined in glVertexAttribPointer as type.
      *
      *	@return Type of component used by this VBO.
      */
      GLenum getComponentType(void);
      
      /**
      *	Returns the target of the VBO.
      */
      GLenum getTarget(void);

      void bind(void);
      void unbind(void);
    };
  }
}

#endif	// _WRENCH_GL_VBO_H_
