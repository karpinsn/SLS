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
 * Free Software Found--------------------
[0001739/000] vrjKERN:New applicatation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SIMPLE_APP
#define _SIMPLE_APP

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>


#include "Holodecoder.h"

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

#include <vrj/Draw/OpenGL/App.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

//#include <boost/timer.hpp>

#include <wrench/gl/OGLStatus.h>

using namespace vrj;

class MainWindowController : public opengl::App
{
public:
   MainWindowController()
   {
      startMovie = true;
   }

   virtual ~MainWindowController (void) {;}

public: // ---- INITIALIZATION FUNCTIONS ---- //
   
   virtual void init();
   virtual void apiInit();

public:
   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preFrame();
   //       draw();
   //        intraFrame();      // Drawing is happening while here
   //       sync();
   //        postFrame();      // Drawing is now done
   //
   //       UpdateDevices();
   //  }
   //------------------------------------

   virtual void bufferPreDraw();
   virtual void preFrame();
   virtual void intraFrame();
   virtual void postFrame();

public: // ----- OpenGL FUNCTIONS ---- //

   virtual void contextInit();
   virtual void draw();

private:
   Holodecoder *m_decoder;
   //boost::timer movieTimer;
   ImageIO 	m_aviIO;
   string	m_movieFilename;
   bool startMovie;

   void timerEvent();
   void playMovie(string movieFile, Holodecoder* decoder);

public:
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
};


#endif
