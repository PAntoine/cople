/* $Revision: 1.2 $
 *		NEW PARAM LIST
 *
 *	This function will create and return a parameter list structure
 *	that can be used to build a parameter list. This list will be
 *	passed on so that code can be generated for the parameter list.
 *
 *
 * 		(c) 1999 P.Antoine.
 *					*/

#include	<malloc.h>
#include	"codetree.h"

PARAMETER_LIST*	new_param_list(void)
{
	PARAMETER_LIST	*result;

	result = calloc(1,sizeof(PARAMETER_LIST));

	if (result == NULL){
		printf("ERROR: out of memory.\n");
	};
	
	return result;
}
