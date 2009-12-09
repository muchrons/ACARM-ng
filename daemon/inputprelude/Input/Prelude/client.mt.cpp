#include "PreludePP/idmef.hpp"
#include "PreludePP/prelude-client.hpp"

#define YELLOW "\033[1;33m"
#define WHITE "\033[0m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"

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

  std::cout << RED;

  //idmef.Set("alert.classification.text", "My classification text");

  IDMEFValue id,*id2;
  prelude_list_t *head, *tmp;

  id=idmef.Get("alert.analyzer");

  head=(prelude_list_t*)(&id);
  if(head)
    prelude_list_for_each(head, tmp) 
      {
	id2=(IDMEFValue*)tmp;
	if(id2)
	  {
	    std::cout << id2->GetType() << std::endl;
	    std::cout << (std::string)*id2 << std::endl;    
	  }
      }
  


  id=idmef.Get("alert.analyzer");
  std::cout << id.GetType() << std::endl;

  id=idmef.Get("alert");
  std::cout << id.GetType() << std::endl;


  std::cout << WHITE;
  std::cout << idmef.ToString() << std::endl;

  return 0;
}
