/*
 * Protocol.mt.cpp
 *
 */
#include <string>
#include <iostream>
#include <ctime>

#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/NetTCP.hpp"
#include "Trigger/SnortSam/Ver14/TwoFish.hpp"
#include "Trigger/SnortSam/Ver14/Protocol.hpp"

using namespace std;
using namespace Trigger::SnortSam;
using namespace Trigger::SnortSam::Ver14;

namespace
{
struct CallbacksImpl: public Ver14::Protocol::Callbacks
{
  virtual uint32_t assignID(void)
  {
    return time(NULL);
  }
}; // struct CallbacksImpl
} // unnamed namespace


int main(int argc, char **argv)
{
  if(argc!=1+5)
  {
    cerr<<argv[0]<<" <host> <port> <key> <from> <to>"<<endl;
    return 1;
  }
  const string    host=argv[1];
  const uint16_t  port=Commons::Convert::to<uint16_t>(argv[2]);
  const string    key =argv[3];
  const NetIO::IP from=NetIO::IP::from_string(argv[4]);
  const NetIO::IP to  =NetIO::IP::from_string(argv[5]);

  CallbacksImpl        callbacks;
  std::auto_ptr<NetIO> netIO(new NetTCP(host, port, 10));
  cout<<argv[0]<<": initializing protocol..."<<endl;
  Ver14::Protocol      proto(Who::SRC|Who::DST, How::IN|How::OUT, 20, key, netIO, callbacks);
  cout<<argv[0]<<": blocking traffic from "<<from<<" to "<<to<<endl;
  proto.block(from, to);
  cout<<argv[0]<<": deinitializing protocol"<<endl;
  proto.deinit();
  cout<<argv[0]<<": all done"<<endl;

  return 0;
} // main()
