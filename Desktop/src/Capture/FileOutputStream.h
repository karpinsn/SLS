/**
    @file 
    @author		Nikolaus Karpinsky

    @section DESCRIPTION
    FileOutputStream is an IOutputStream that streams its data to a video file
*/

#ifndef _FILE_OUTPUT_STREAM_H_
#define _FILE_OUTPUT_STREAM_H_

#include "IOutputStream.h"
#include "MeshInterchange.h"

#include "VideoIO.h"

class FileOutputStream : public IOutputStream
{
private:
	VideoIO m_io;

public:
    
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

#endif	// _FILE_OUTPUT_STREAM_H_
