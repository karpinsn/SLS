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

//----------------------------------------
// Simple OpenGL sample application
//
// main.cpp
//----------------------------------------

#define USE_VRJ

#include "MainWindowController.h"

// --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>

//  Tells facade classes to use context specific data


using namespace vrj;

int main(int argc, char* argv[])
{
   #if ! defined(VRJ_USE_COCOA)
   if (argc <= 1)
   {
      std::cerr << "Missing command line options\n";
      return EXIT_FAILURE;
   }
   #endif

   vrj::Kernel* kernel = vrj::Kernel::instance();
   MainWindowController application;          // Instantiate an instance of the app

   // Initialize the kernel.
   kernel->init(argc, argv);

   // Load any config files specified on the command line
   for (int i = 1; i < argc; ++i)
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();
   kernel->setApplication(&application);
   kernel->waitForKernelStop();

   return EXIT_SUCCESS;
}
