#include "PreludePP/idmef.hpp"
#include "PreludePP/prelude-client.hpp"


using namespace Prelude;

int main(int argc, char * argv[])
{
  int ret;

  ret = prelude_init(&argc, argv);
  if ( ret < 0 ) {
    prelude_perror(ret, "unable to initialize the prelude library");
    return -1;
  }
  
  Client client("ACARM-wojek");
  client.SetConfigFilename("/etc/prelude/default/client.conf");
  client.SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //client.SetFlags( Client::FLAGS_ASYNC_TIMER);
  client.Init();
  client.Start();

  IDMEF idmef;  

  std::cout << "Started" << std::endl;

  client >> idmef;

  std::cout << "Got" << std::endl;

  std::cout << idmef << std::endl;




  return 0;
}
