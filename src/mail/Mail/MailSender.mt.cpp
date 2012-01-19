/*
 * MailSender.mt.cpp
 *
 */
#include <iostream>

#include "Mail/MailSender.hpp"
#include "Mail/TestAccount.t.hpp"

using namespace std;
using namespace Mail;

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    cerr<<argv[0]<<" <recipient@ddress>"<<endl;
    return 1;
  }

  try
  {
    MailSender ms( getTestConfig1() );
    ms.send("message subject :)", "hello\nworld!", Mail::Config::Recipients(argv[1]));
    return 0;
  }
  catch(const std::exception &ex)
  {
    cerr<<argv[0]<<": exception: "<<ex.what()<<endl;
    return 2;
  }
}
