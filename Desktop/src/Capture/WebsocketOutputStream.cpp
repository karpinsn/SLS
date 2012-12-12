#include "WebsocketOutputStream.h"

WebsocketOutputStream::WebsocketOutputStream(int port) : m_port(port)
{ }

void WebsocketOutputStream::Open(void)
{
	//	Open our socket connection
	m_socket.start(m_port);

	//	Start our background thread that the socket can broadcast on
	m_socketProcessorThread = new QThread(this);
	m_socketProcessor = new WebsocketProcessor(m_socket);
	m_socketProcessor->moveToThread(m_socketProcessorThread);

	//  Connect the thread and its timer 
	connect(m_socketProcessorThread, SIGNAL(started()),  m_socketProcessor, SLOT(processSocket()));
	connect(m_socketProcessor, SIGNAL(finished()), m_socketProcessorThread, SLOT(quit()));
	connect(m_socketProcessorThread, SIGNAL(finished()), m_socketProcessorThread, SLOT(deleteLater()));
	connect(m_socketProcessor, SIGNAL(finished()), m_socketProcessor, SLOT(deleteLater())); 

	//	Start our background thread that will process the output stream
	m_streamProcessorThread = new QThread(this);
	m_streamProcessor = new OutstreamProcessor(m_socket, m_imageBuffer);
	m_streamProcessor->moveToThread(m_streamProcessorThread);

	//	Connect the thread and its timer
	connect(m_streamProcessorThread, SIGNAL(started()), m_streamProcessor, SLOT(processOutputStream()));
	connect(m_streamProcessor, SIGNAL(finished()), m_streamProcessorThread, SLOT(quit()));
	connect(m_streamProcessorThread, SIGNAL(finished()), m_streamProcessorThread, SLOT(deleteLater()));
	connect(m_streamProcessor, SIGNAL(finished()), m_streamProcessor, SLOT(deleteLater()));

	m_socketProcessorThread->start();
	m_streamProcessorThread->start();
}

void WebsocketOutputStream::WriteStream(shared_ptr<MeshInterchange> mesh)
{
	//	Encoding properties
	//int encodingProperties[] = {CV_IMWRITE_JPEG_QUALITY, 90, 0};


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
	m_imageBuffer.pushFrame(m_transferImage.get());

	//cvCvtColor(m_transferImage.get(), m_transferImage.get(), CV_RGB2BGR);
	/*
	auto buffer = shared_ptr<CvMat>(
					cvEncodeImage(".png", m_transferImage.get(), encodingProperties), 
					[](CvMat* ptr){cvReleaseMat(&ptr);});
	
	m_socket.broadcastData(buffer->data.ptr, buffer->width);*/
}

void WebsocketOutputStream::Close(void)
{
	//	Let the threads know that we are stopping
	m_socketProcessor->stop();
	m_streamProcessor->stop();

	//	Stop / close our socket connection
	m_socket.stop();
}

void WebsocketProcessor::stop(void)
{
  m_running = false;
}

void WebsocketProcessor::processSocket()
{
	//	Tight loop here. The socket will ensure that we wont sit and spin idly
  while(m_running && 0 <= m_socket.yieldTime())
  { }

  emit finished();
}

void OutstreamProcessor::stop(void)
{
  m_running = false;
}

void OutstreamProcessor::processOutputStream(void)
{
  int encodingProperties[] = {CV_IMWRITE_PNG_COMPRESSION, 3, 0 };

  while(m_running)
  {
	//	Do our image pulling stuff
	auto frame = m_buffer.popFrame();
	cvCvtColor(frame.get(), frame.get(), CV_RGB2BGR);
	auto buffer = shared_ptr<CvMat>(
				cvEncodeImage(".png", frame.get(), encodingProperties), 
				[](CvMat* ptr){cvReleaseMat(&ptr);});
	m_socket.broadcastData(buffer->data.ptr, buffer->width);
  }
}
