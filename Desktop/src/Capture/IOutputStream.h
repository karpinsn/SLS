/**
    @file 
    @author		Nikolaus Karpinsky

    @section DESCRIPTION
    IOutputStream is the interface for an output stream class such as a file 
    output stream or network output stream. This allows meshes to be output 
    to various mediums.
*/

#ifndef _I_OUTPUT_STREAM_H_
#define _I_OUTPUT_STREAM_H_

class IOutputStream
{
public:
    virtual ~IOutputStream(void) { }; 

    /**
     * Opens the output stream for writing.
     *
     * Opens the output stream for writing. Once opened, meshs may be outputted
     * using the WriteStream(...) method.
     */
    virtual void Open(void) = 0;

    /**
     * Writes the specified mesh out to the output stream.
     *
     * Writes the specified mesh out to the output stream. Before calling this
     * method, the output stream must be opened with a call to Open(). This
     * method should never be called after the stream is closed with Close()
     */
    virtual void WriteStream(MeshInterchange mesh) = 0;

    /*
     * Closes the output stream.
     *
     * Closes the output stream. Once closed an output stream cannot be written
     * to anymore.
     */
    virtual void Close(void) = 0;
};

#endif	// _I_OUTPUT_STREAM_H_
