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
	#include <WinSock2.h>
	#include <windows.h>
#endif

#include "IOutputStream.h"
#include "MeshInterchange.h"

#include <antenna/BaseStation.h>

class WebsocketOutputStream : public IOutputStream
{
private:
	string m_filename;
	int m_width;
	int m_height;

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
};

#endif	// _WEB_SOCKET_OUTPUT_STREAM_H_
