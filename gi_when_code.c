/* $Revision: 1.2 $ (generate_intel)
 *
 *		     Generate Whenever Code
 *
 *	This function will generate the whenever code that is called by
 *	the functions. All whenever code are called and are expected to
 *	return.
 *
 *		Copyright (c) 1999 Peter Antoine 
 */

#include	"cople.h"
#include	"codetree.h"
#include	"intel.h"
#include	<stdio.h>
#include	<stdlib.h>

void	gi_when_code(LEVEL_LNK *stack)
{
	int		label = 0;
	WHENEVER_LIST		*walk_list;
	extern FILE		*object_file;
	extern WHENEVER_LIST	*when_list;

	walk_list = when_list;

	/* default - is the code that will handle undefined exceptions */
	fprintf(object_file,"when_label_0:\tret\n");

	while(walk_list != NULL)
	{
		++label;
		fprintf(object_file,"when_label_%d: ",label);
		gi_stat_code(walk_list->when_code,stack,0,0);
		walk_list = walk_list->next;
		fprintf(object_file,"\tret\n");
	};
}
