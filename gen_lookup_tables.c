/* $Revision: 1.2 $
 *
 *		     Generate Lookup Tables
 *
 *	This function will generate the table information for the assembler to build
 *	the lookup and paramter tables from.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"codetree.h"
#include	"cople.h"

void	gen_lookup_tables(CLAUSE* root)
{
	int	count,kount,count2,kount2;
	char	*type;
	extern FILE	*object_file;

	/* get the number of services */
	count = root->c_block.num_instr;
	fprintf(object_file,"\t.lookup\t%d\n\n",count);

	/* get the service headers */
	for (kount=0;kount<count;kount++)
	{
		fprintf(object_file,"\t.service\t%s\n",root->c_block.instr_list[kount]->c_service.name->name);
	};

	/* now do the paramter tables */
	for (kount=0;kount<count;kount++)
	{
		fprintf(object_file,"\n\t.parameters\t%s, %d\n",
			root->c_block.instr_list[kount]->c_service.name->name,
			root->c_block.instr_list[kount]->c_service.num_locals);

		count2 = root->c_block.instr_list[kount]->c_service.num_locals;
		for(kount2=0;kount2<count2;kount2++)
		{
			if (root->c_block.instr_list[kount]->c_service.locals[kount2]->access_type & a_reference)
				type = "ref";
			else
				type = "var";
		
			if ((root->c_block.instr_list[kount]->c_service.locals[kount2]->access_type & a_const))
			{	
				if (root->c_block.instr_list[kount]->c_service.locals[kount2]->type != d_string)
					fprintf(object_file,"\t.parameter\t%s,%s,%s,%d,%d\n",
						root->c_block.instr_list[kount]->c_service.locals[kount2]->name,
						type,
						type_name[root->c_block.instr_list[kount]->c_service.locals[kount2]->type],
						root->c_block.instr_list[kount]->c_service.locals[kount2]->offset,
						root->c_block.instr_list[kount]->c_service.locals[kount2]->data);
				else
					fprintf(object_file,"\t.parameter\t%s,%s,%s,%d,#data_space+%d\n",
						root->c_block.instr_list[kount]->c_service.locals[kount2]->name,
						type,
						type_name[root->c_block.instr_list[kount]->c_service.locals[kount2]->type],
						root->c_block.instr_list[kount]->c_service.locals[kount2]->offset,
						root->c_block.instr_list[kount]->c_service.locals[kount2]->data);
			} else 
					fprintf(object_file,"\t.parameter\t%s,%s,%s,%d, -\n",
						root->c_block.instr_list[kount]->c_service.locals[kount2]->name,
						type,
						type_name[root->c_block.instr_list[kount]->c_service.locals[kount2]->type],
						root->c_block.instr_list[kount]->c_service.locals[kount2]->offset);
		};
	};
}
