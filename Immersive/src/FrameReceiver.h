#ifndef _FRAME_RECEIVER_H
#define _FRAME_RECEIVER_H

#include <boost/asio.hpp>

const short multicast_port = 30001;

class FrameReceiver
{
private:
  boost::asio::ip::udp::socket m_socket;
  boost::asio::ip::udp::endpoint m_senderEndpoint;

public:

};

#endif
