/**
    @file 
    @author		Nikolaus Karpinsky

    @section DESCRIPTION
    SaveStream is a class that is used to encode and stream MeshInterchange 
    meshs out to a specified output stream.
*/

#ifndef _SAVE_STREAM_H_
#define _SAVE_STREAM_H_

class SaveStream
{
private:
    // Encoder that we are using to encode our data
    shared_ptr<IEncoder> m_encoder;
    //  Output stream that we are using to output our data
    shared_ptr<IOutputStream> m_outStream;

public:
    SaveStream();

    /**
     * Opens a new save stream with the specified encoder and output stream
     *
     * Opens a new save stream with the specified encoder and output stream.
     * From here, encodeAndStream() may be called with the data to output followed
     * by close() to close the output stream.
     *
     * @param encoder Encoder to use to encode the data specified in encodeAndStream()
     * @param outStream Output stream to write the encoded data to
     */
    void open(shared_ptr<IEncoder> encoder, shared_ptr<IOutputStream> outStream);

    /**
     * Encodes the specified mesh and streams it out to the save stream.
     *
     * Encodes the speicifed mesh with the encoder specified by open and then
     * streams it out to the output stream speicifed in open.
     *
     * @param mesh Mesh to encode and write out to the output stream
     */
    void encodeAndStream(MeshInterchange* mesh);

    /**
     * Closes this SaveStream.
     */
    void close();
};

#endif	// _SAVE_STREAM_H_
