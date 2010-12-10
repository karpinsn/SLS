/*
 Filename:		Camera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/type_ptr.hpp>

#include <math.h>

using namespace std;

#define DegToRad 3.14159f/180.0f
#define RadToDeg 180.0f/3.14159f

/* 
 *  Simple trackball-like motion adapted from 
 *  "Mathematical Elements for Computer Graphics" by David F. Rogers &
 *  Alan J. Adams 1976. Section 3-8, pp. 55 ~ 59.
 *  Implemented by Tsung-Pin Yeh 10/26/2005. Modified 10/11/2006
*/

namespace wrench
{
	namespace gl 
	{
		class Camera
		{
			
		public:
			static const int ROTATE_MODE = 1;
			static const int ZOOM_MODE = 2;
			static const int PAN_MODE = 3;
			static const int TWIST_MODE = 4;

			Camera();
			~Camera(void);

			/* functions */
			void setMode(int mode);
			void init();
			void init(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
			void initRotatedCam(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
			
			void applyMatrix();
			void reshape(int width, int height);
			void mousePressed(int x, int y);
			void mouseRelease(int x, int y);
			void mouseMotion(int x, int y);
			
		protected:
			GLint     tb_mode;  // 1: rotate
								// 2: Zoom
								// 3: Twist
								// 4: Pan

			GLboolean tb_moving;
			GLfloat   tb_currPos[2];
			GLfloat   tb_prevPos[2];
			
			glm::mat4   tb_transform;
			glm::mat4   tb_accuTransform;

			float	  m_viewLength;
			GLuint    tb_width;
			GLuint    tb_height;
			int		  m_currentView;
				
			void _getScreenXYZ(float sx[3], float sy[3], float sz[3]);
			void _rotate(float dx, float dy);
			void _zoom(float dx, float dy);
			void _twist(float dx, float dy);
			void _pan(float dx, float dy);
		};
	}
}

#endif	// _CAMERA_H_
