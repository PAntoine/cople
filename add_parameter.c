/* $Revision: 1.2 $
 *		ADD_PARAMETER
 *
 *	This function will add a parameter to the parameter list
 *	that is passed in as the first parameter.
 *
 * 		(c) 1999 P.Antoine.
 *			
 *	MAX size for parameter names is 32 bytes.
 */

#include	<malloc.h>
#include	"codetree.h"

void	add_parameter(PARAMETER_LIST *list, DATA_TYPE type,char *param_name,CONSTANT *def_value,int a_ref)
{
	list->list_size++;
	list->list = realloc(list->list,(sizeof(PARAMETER) * list->list_size));

	list->list[list->list_size-1].type   = type;
	list->list[list->list_size-1].def    = def_value;
	list->list[list->list_size-1].offset = 0;
	list->list[list->list_size-1].a_ref  = a_ref;
	list->list[list->list_size-1].name   = (char*) malloc((sizeof(char)*strlen(param_name))+1);

	strcpy(list->list[list->list_size-1].name,param_name);
}

