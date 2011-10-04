/*
 * NetTCP.mt.cpp
 *
 */
#include <iostream>

#include "Commons/Convert.hpp"
#include "Trigger/SnortSam/NetTCP.hpp"

using namespace std;
using namespace Trigger::SnortSam;


int main(int argc, char **argv)
{
  if(argc!=1+3)
  {
    cerr<<argv[0]<<" <host> <ip> <timeout>"<<endl;
    return 1;
  }
  const std::string  host   =argv[1];
  const uint16_t     port   =Commons::Convert::to<uint16_t>(argv[2]);
  const unsigned int timeout=Commons::Convert::to<unsigned int>(argv[3]);

  cout<<argv[0]<<": connecting..."<<endl;
  NetTCP        n(host, port, timeout);

  cout<<argv[0]<<": sending..."<<endl;
  const uint8_t buf[]={'e','v','i','l','\n'};
  n.send(buf, sizeof(buf));

  cout<<argv[0]<<": receiving..."<<endl;
  DataRef r=n.receive(sizeof(buf));

  cout<<argv[0]<<": got "<<r.size()<<" bytes: |";
  for(size_t i=0; i<r.size(); ++i)
    cout<<r[i];
  cout<<"|"<<endl;

  return 0;
}
