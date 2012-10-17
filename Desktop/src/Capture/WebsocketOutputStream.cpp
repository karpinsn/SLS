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
	int encodingProperties[] = {CV_IMWRITE_JPEG_QUALITY, 80, 0};

	auto buffer = shared_ptr<CvMat>(
					cvEncodeImage(".jpg", mesh->getIplImage(), encodingProperties), 
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
  while(m_running && m_socket.yieldTime())
  { }
}