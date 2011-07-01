/*
 *  Logger.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "Logger.h"

void wrench::Logger::logError(const string &message)
{
    clog << "Error: " << message << endl;
}

void wrench::Logger::logDebug(const string &message)
{
    clog << "Debug: " << message << endl;
}

void wrench::Logger::logError(const char* Format, ... )
{
    va_list Arguments;
    va_start(Arguments, Format);
    double FArg;
    int IArg;
    for(int i = 0; Format[i] != '\0'; ++i )
    {
        if (Format[i] == 'f')
        {
            clog << va_arg(Arguments, double);

        }
        else if (Format[i] == 'i')
        {
            clog << va_arg(Arguments, int);
        }

        switch(Format[i])
        {
            // string
        case 's':

            clog << va_arg( Arguments, char * );
            break;
            // character
        case 'c':

            clog << ((char) va_arg( Arguments, int ));

            break;

            // integer
        case 'd':

            clog << va_arg( Arguments, int );
            break;
        case 'f':
            clog << va_arg(Arguments, double);
            break;
        default:
            break;


        }
    }
    va_end(Arguments);
}

