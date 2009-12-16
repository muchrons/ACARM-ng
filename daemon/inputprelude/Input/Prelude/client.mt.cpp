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
  
  Client client("ACARM-wojek-acm");
  client.SetConfigFilename("/etc/prelude/default/client.conf");
  client.SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //client.SetFlags( Client::FLAGS_ASYNC_TIMER);
  client.Init();
  client.Start();

  IDMEF idmef=0;  

  std::cout << "Started" << std::endl;

  client >> idmef;

  if (idmef)
    std::cerr << 1;
  else
    std::cerr << 0;

  
  if (idmef)
    {
      std::cerr << RED;
      std::cout << idmef.ToString() << std::endl;
      //idmef.Set("alert.classification.text", "My classification text");

      IDMEFValue id=idmef.Get("alert.analyzer");

      if (id.GetType()==IDMEF_VALUE_TYPE_LIST)
	{
	  prelude_list_t *head=(prelude_list_t*)(&id);
	  prelude_list_t *tmp=0;
	  IDMEFValue *id2;

	  if(head)
	    prelude_list_for_each(head, tmp) 
	      {
		id2=prelude_list_entry(tmp,int,list);
		std::cerr << '*';
		id2=(IDMEFValue*)tmp;
		if(id2)
		  {
		    //std::cerr << id2->GetType() << std::endl;		    
		    //std::cout << (std::string)*id2 << std::endl;    
		  }		
	      }	      	  	  
	}
      /*
      id=idmef.Get("alert.analyzer");
      std::cout << id.GetType() << std::endl;

      id=idmef.Get("alert");
      std::cout << id.GetType() << std::endl;
      */

      std::cerr << WHITE;

      }
  return 0;
}
