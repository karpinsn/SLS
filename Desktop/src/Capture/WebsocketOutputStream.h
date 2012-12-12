/**
    @file 
    @author		Nikolaus Karpinsky

    @section DESCRIPTION
    WebsocketOutputStream is an IOutputStream that streams its data to a websocket
*/

#ifndef _WEBSOCKET_OUTPUT_STREAM_H_
#define _WEBSOCKET_OUTPUT_STREAM_H_

//	Needed so that windows.h does not include Winsock.h
#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif
	#include <WinSock2.h>
	#include <windows.h>
#endif

#include <QThread>

#include <cv.h>
#include <highgui.h>

#include "IOutputStream.h"
#include "MeshInterchange.h"
#include "ImageBuffer.h"

#include <antenna/BaseStation.h>

class WebsocketProcessor : public QObject
{
  Q_OBJECT

  private:
	bool m_running;
	antenna::BaseStation& m_socket;

  public:
	WebsocketProcessor(antenna::BaseStation& socket) : m_socket(socket), m_running(true) { };
	void stop(void);

  signals:
	void finished(void);

  public slots:
    void processSocket();
};

class OutstreamProcessor : public QObject
{
  Q_OBJECT

  private:
	bool m_running;
	antenna::BaseStation& m_socket;
	ImageBuffer& m_buffer;

  public:
	OutstreamProcessor(antenna::BaseStation& socket, ImageBuffer& buffer) : m_socket(socket), m_buffer(buffer), m_running(true) { };
	void stop(void);

  signals:
	void finished(void);

  public slots:
    void processOutputStream(void);
};

class WebsocketOutputStream : public QObject, public IOutputStream
{
  Q_OBJECT

private:
	//	Socket and port that we are connected with
	antenna::BaseStation	m_socket;
	const int				m_port;
	
	QThread*				m_socketProcessorThread;
	QThread*				m_streamProcessorThread;

	WebsocketProcessor*		m_socketProcessor;
	OutstreamProcessor*		m_streamProcessor;

	ImageBuffer				m_imageBuffer;
	shared_ptr<IplImage>	m_transferImage;

public:
    WebsocketOutputStream(int port);

    /**
     * Opens the output stream for writing.
     *
     * Opens the output stream for writing. Once opened, meshs may be outputted
     * using the WriteStream(...) method.
     */
    void Open(void);

    /**
     * Writes the specified mesh out to the output stream.
     *
     * Writes the specified mesh out to the output stream. Before calling this
     * method, the output stream must be opened with a call to Open(). This
     * method should never be called after the stream is closed with Close()
     */
    void WriteStream(shared_ptr<MeshInterchange> mesh);

    /*
     * Closes the output stream.
     *
     * Closes the output stream. Once closed an output stream cannot be written
     * to anymore.
     */
    void Close(void);

protected:
	void run(void);
};

#endif	// _WEB_SOCKET_OUTPUT_STREAM_H_
