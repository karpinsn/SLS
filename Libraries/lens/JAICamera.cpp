/*
 *  JAICamera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "JAICamera.h"

void lens::JAICamera::init(void)
{
	_openFactory();
}

void lens::JAICamera::open(void)
{
	_openCamera();
}

void lens::JAICamera::close(void)
{
	_closeCamera();
}

std::string lens::JAICamera::cameraName(void)
{
	return "JAI Camera Driver";
}

void lens::JAICamera::_openFactory(void)
{
	J_STATUS_TYPE status = J_Factory_Open("", &m_factory);
	if(status != J_ST_SUCCESS) 
	{
		//	Unable to open the factory. Show an error
	}
}
void lens::JAICamera::_openCamera(void)
{
	J_STATUS_TYPE	status = J_ST_SUCCESS;
	bool8_t			hasChanged = false;
	uint32_t        tSize;

	// Update camera list
    status = J_Factory_UpdateCameraList(m_factory, &hasChanged);
    if (status != J_ST_SUCCESS)
    {
		//	Could not update the camera list
		return;
    }

	uint32_t        cameraCount;
    // Get the number of Cameras
    status = J_Factory_GetNumOfCameras(m_factory, &cameraCount);
    if (status != J_ST_SUCCESS)
    {
        // Could not determine the number of cameras
        return;
    }
    if (cameraCount == 0)
    {
        //	There is no camera
        return;
    }

    // Get camera ID
    tSize = (uint32_t)sizeof(m_cameraID);
    status = J_Factory_GetCameraIDByIndex(m_factory, 0, m_cameraID, &tSize);
    if (status != J_ST_SUCCESS)
    {
        //	Could not get a camera ID
        return;
    }

    // Open camera
    status = J_Camera_Open(m_factory, m_cameraID, &m_camera);
    if (status != J_ST_SUCCESS)
    {
        //	Could not open the camera
        return;
    }
    
	//	Camera opened
}

void lens::JAICamera::_closeFactory(void)
{
}

void lens::JAICamera::_closeCamera(void)
{
	if (m_camera)
	{
		// close camera
		J_STATUS_TYPE status = J_Camera_Close(m_camera);
		if(status != J_ST_SUCCESS) 
		{
			//	Show some error message
		}
		else
		{
			m_camera = NULL;
		}
	}
}