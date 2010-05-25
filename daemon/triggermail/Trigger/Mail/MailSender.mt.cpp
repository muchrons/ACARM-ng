/*
 * MailSender.mt.cpp
 *
 */
#include <iostream>

#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/TestAccount.t.hpp"

using namespace std;
using namespace Trigger::Mail;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <recipient@ddress>"<<endl;
    return 1;
  }

  try
  {
    MailSender ms( getTestConfig1(argv[1]) );
    ms.send("message subject :)", "hello\nworld!");
    return 0;
  }
  catch(const std::exception &ex)
  {
    cerr<<argv[0]<<": exception: "<<ex.what()<<endl;
    return 2;
  }
}
