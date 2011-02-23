/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <math.h>

#include "MainWindowController.h"

#if defined(__APPLE__)
#include <glew.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

using namespace gmtl;
using namespace vrj;

void MainWindowController::init()
{
    std::cout << "---------- App:init() ---------------" << std::endl;
    // Initialize devices
    mHead.init("VJHead");
}

void MainWindowController::apiInit()
{
}

// Clears the viewport.  Put the call to glClear() in this
// method so that this application will work with configurations
// using two or more viewports per display window.
void MainWindowController::bufferPreDraw()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

void MainWindowController::preFrame()
{

}

//----------------------------------------------
//  Draw the scene.
//
// - Draws a box and a coordinate axis set on the box
// - Offset and rotate the box by a matrix that we create
//----------------------------------------------


void MainWindowController::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   // --- Setup for drawing --- //
   glMatrixMode(GL_MODELVIEW);
   m_decoder->draw();
   OGLStatus::logOGLErrors("simpleApp - Draw()");
}

void MainWindowController::intraFrame()
{

}

void MainWindowController::postFrame()
{

}

void MainWindowController::contextInit()
{
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7f, 0.7f,  0.7f, 1.0f };
   GLfloat mat_diffuse[] = { 1.0f,  0.5f,  0.8f, 1.0f };
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
//   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   // Init GLEW
   GLenum err = glewInit();
   if(GLEW_OK != err)
   {
      std::cout << "Failed to init GLEW: " << glewGetErrorString(err) << std::endl; 
   }
   std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;

   OGLStatus::logFBOStatus();
   m_decoder = new Holodecoder();
   m_decoder->init();
   //playMovie("~/Data/VeryFirst-Uncompressed.avi", m_decoder);
   OGLStatus::logOGLErrors("simpleApp - contextInit()");
}

void MainWindowController::timerEvent()
{
 	double elapsed = 1;//movieTimer.elapsed();
	
	if(elapsed >= 1.0/30.0)
	{
		//movieTimer.restart();
		//	Need to fetch the next frame
		
		IplImage* frame = m_aviIO.readAviFileFrame();

		if(frame)
		{
			m_decoder->setBackHoloBuffer(frame);
			m_decoder->swapBuffers();		
		}
	}
}

void MainWindowController::playMovie(string movieFile, Holodecoder* decoder)
{
	m_movieFilename = movieFile;

	if(!m_aviIO.aviFileOpen())
	{
		bool fileOpened = m_aviIO.readAviFile(m_movieFilename.c_str());
		
		if(fileOpened)
		{
			IplImage *frame = m_aviIO.readAviFileFrame();
			if(frame)
			{
				m_decoder->setBackHoloBuffer(frame);
				m_decoder->swapBuffers();
				//movieTimer.restart();
			}
		}
	}
}
