#include "WebsocketOutputStream.h"

WebsocketOutputStream::WebsocketOutputStream(int port) : QThread(), m_port(port)
{ }

void WebsocketOutputStream::Open(void)
{
	//	Open our socket connection
	m_socket.start(m_port);

	//	Start our background thread that the socket can broadcast on
	m_running = true;
	start();
}

void WebsocketOutputStream::WriteStream(shared_ptr<MeshInterchange> mesh)
{
	//	Encoding properties
	int encodingProperties[] = {CV_IMWRITE_JPEG_QUALITY, 75, 0};
	//int encodingProperties[] = {CV_IMWRITE_PNG_COMPRESSION, 3, 0 };

	if(!m_transferImage)
	{
		//	Creates an OpenCV image that we can use to fetch the texture with
		m_transferImage = shared_ptr<IplImage>(
			cvCreateImage(
				cvSize(
					mesh->getTexture()->getWidth(),
					mesh->getTexture()->getHeight()),
				IPL_DEPTH_8U,
				3),
			[](IplImage* ptr) { cvReleaseImage(&ptr); });
	}

	mesh->getTexture()->transferFromTexture(m_transferImage.get());
	cvCvtColor(m_transferImage.get(), m_transferImage.get(), CV_RGB2BGR);

	auto buffer = shared_ptr<CvMat>(
					cvEncodeImage(".jpg", m_transferImage.get(), encodingProperties), 
					[](CvMat* ptr){cvReleaseMat(&ptr);});
	
	m_socket.broadcastData(buffer->data.ptr, buffer->width);
}

void WebsocketOutputStream::Close(void)
{
	//	Let the thread know that we are stopping
	m_running = false;

	//	Stop / close our socket connection
	m_socket.stop();
}

void WebsocketOutputStream::run()
{
	//	Tight loop here. The socket will ensure that we wont sit and spin idly
  while(m_running && 0 <= m_socket.yieldTime())
  { }
}