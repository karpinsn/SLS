/**
 @file
 @author Nikolaus Karpinsky
 */

#ifndef _I_DECODER_H_
#define _I_DECODER_H_

using namespace std;

class IDecoder
{
public:
  virtual ~IDecoder(void) { };
  virtual void decode()			  = 0;
  virtual Texture& getDepthMap()  = 0;
};

#endif	// _I_DECODER_H_
