/* $Revision: 1.2 $
 *
 *		       Generate Fix-up Code
 *
 *	This function will generate the fix-up code at the front of the function.
 *	It basically adds the functional data offset which is set at load time,
 *	and adds this the the elements in its fix-up list.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"cople.h"

void	gen_fixup_code(void)
{
	int		count=0;
	LEVEL_LNK		*walk,*last;
	RANGE_FIXUP		*rwalk,*rlast;
	extern LEVEL_LNK	*fix_up_list;
	extern RANGE_FIXUP	*range_list;
	extern FILE		*object_file;

	walk = fix_up_list;

	while(walk != NULL)
	{
		fprintf(object_file,"\tadd.d\ta30(#%d),a30\n",walk->count);
		last = walk;
		walk = walk->next;

		free(last);
	};


	rwalk = range_list;

	while(rwalk != NULL)
	{
		fprintf(object_file,"\tmove.d\td0,#%d\n",rwalk->count);
		fprintf(object_file,"\tmove.d\td1,#%d\n",rwalk->base);
		fprintf(object_file,"f_%d:\tadd.d\ta30(d1)+,a30\n",count);
		fprintf(object_file,"\tloop.d\td0,f_%d\n",count++);
		rlast = rwalk;
		rwalk = rwalk->next;

		free(rlast);
	};
}
