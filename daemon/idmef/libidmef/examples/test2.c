/* compile with
** gcc -I/usr/include/libidmef -Wall -g -O0 `pkg-config --cflags --libs libxml-2.0` `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs libidmef` -o test2 test2.c
*/
#include <stdio.h>
#include <unistd.h>

#include <libidmef/idmef_types.h>
#include <libidmef/idmef_global.h>
#include <libidmef/idmef_create.h>
#include <libidmef/idmef_list.h>

/* test for memory usage within ilist.c */

int
main(int argc, char **argv)
{
	IDMEFPtr theNode = newNode (NULL), tmp;
        
        tmp = newNode_category ("unknown", NULL);
        libidmef_list_set_attribute (theNode, tmp);
        libidmef_idmefp_destroy (tmp, 1);

        tmp = newNode_location ("home", NULL);
        libidmef_list_add_element (theNode, tmp);

	libidmef_idmefp_destroy (theNode, 1);
        
	return (0);
}
