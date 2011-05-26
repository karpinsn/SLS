/*
 *  JAICamera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "JAICamera.h"

lens::JAICamera::JAICamera(void)
{
	m_cameraImage = NULL;
}

void lens::JAICamera::init(void)
{
	_openFactory();
}

void lens::JAICamera::open(void)
{
	_openCamera();

	J_STATUS_TYPE	retval = J_ST_SUCCESS;
	int64_t int64Val;
	SIZE	ViewSize;
	int64_t pixelFormat;
	
	// Get Width from the camera
    retval = J_Camera_GetValueInt64(m_camera, NODE_NAME_WIDTH, &int64Val);
    ViewSize.cx = (LONG)int64Val;     // Set window size cx

    // Get Height from the camera
    retval = J_Camera_GetValueInt64(m_camera, NODE_NAME_HEIGHT, &int64Val);
    ViewSize.cy = (LONG)int64Val;     // Set window size cy

    // Get pixelformat from the camera
    retval = J_Camera_GetValueInt64(m_camera, NODE_NAME_PIXELFORMAT, &int64Val);
    pixelFormat = int64Val;

    // Calculate number of bits (not bytes) per pixel using macro
    int bpp = J_BitsPerPixel(pixelFormat);

	// Open stream
    retval = J_Image_OpenStream(m_camera, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&lens::JAICamera::streamCallBack), &m_thread, (ViewSize.cx*ViewSize.cy*bpp)/8);
    if (retval != J_ST_SUCCESS) 
	{
        //	Could not open the stream
        return;
    }

    // Start Acquision
    retval = J_Camera_ExecuteCommand(m_camera, NODE_NAME_ACQSTART);
}

void lens::JAICamera::close(void)
{
	_closeCamera();
}

//--------------------------------------------------------------------------------------------------
// StreamCBFunc
//--------------------------------------------------------------------------------------------------
void lens::JAICamera::streamCallBack(J_tIMAGE_INFO * pAqImageInfo)
{
    // We only want to create the OpenCV Image object once and we want to get the correct size from the Acquisition Info structure
    if (m_cameraImage == NULL)
    {
        // Create the Image:
        // We assume this is a 8-bit monochrome image in this sample
        m_cameraImage = cvCreateImage(cvSize(pAqImageInfo->iSizeX,pAqImageInfo->iSizeY), IPL_DEPTH_8U, 1);
    }

	// Copy the data from the Acquisition engine image buffer into the OpenCV Image obejct
    memcpy(m_cameraImage->imageData, pAqImageInfo->pImageBuffer, m_cameraImage->imageSize);

	notifyObservers(m_cameraImage);
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
