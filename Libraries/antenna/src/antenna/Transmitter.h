/**
  * @file
  * @author Nikolaus Karpinsky
  * @date   07/08/2011
  *
  * Class used to transmit TCP packets across a network.
  */

#ifndef _ANTENNA_TRANSMITTER_H_
#define _ANTENNA_TRANSMITTER_H_

namespace antenna
{
class Transmitter
{
public:
  Transmitter(void);
  ~Transmitter();

  void init(void);
};
}

#endif //_ANTENNA_TRANSMITTER_H_
