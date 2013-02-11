/* $Revision: 1.2 $
 *
 *		           Generate Code
 *
 *	This function is part of the cople compiler. It controls the genration
 *	of the cople assembler. I am a big-engian type bloke so the CASM code
 *	is big endian. It is also noted that there is two types of code generated
 *	for the system code the code is not CASM code but native. This will
 *	depend on the system that the complier is for. 
 *
 *		Copyright (c) 1999 Peter Antoine.
 *
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	"cople.h"
#include	"codetree.h"

void	generate_code(CLAUSE* tree_base)
{
	extern int		max_memory_size, initial_size;
	extern int		version,release;
	extern PROCESSOR	target;
	extern FILE		*object_file;
	OPERAND		op;
	LEVEL_LNK		*stack;
	int		size=0,label=0;
	extern int		level_at;
	extern CODE_TYPE	program_type;

	/* need to start a new level thing */
	level_at = 0;

	/* pre-program code here:
	 * object      - look up table & object data space 
	 * application - start-up & fixup code
	 * system      - none.
	 */

	switch(program_type)
	{
	case p_object:	
		fprintf(object_file,"@cople version %d r%d - Object CASM \n\n\n",version,release);
		/* generate look up tables */
		fprintf(object_file,"\t.obj\n");
		gen_lookup_tables(tree_base);

		/* generate the data space stuff */
		fprintf(object_file,"data_space:\n");
		gen_the_dbs();

		fprintf(object_file,"variable_space:\n");
		if(max_memory_size > 0)
			fprintf(object_file,"\tdb.b\t$00 times %d\n",max_memory_size);
	
		/* the call the recursive generate */
		stack = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		gen_block_code(tree_base,stack,0,0);
		
		/* now generate the code for the whenevers */
		gen_when_code(stack);
		free_link(stack);
		break;

	case p_system:
		fprintf(object_file,";cople Version %d r%d - System CASM \n\n\n",version,release);
		stack = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		
		switch(target)
		{
		case INTEL_486:		generate_intel_code(tree_base,stack);
				break;
		
		default:	printf("ERROR: Processor that has been selected is not supported \n");
		};

		free_link(stack);
		break;

	case p_application:
		fprintf(object_file,"@cople Version %d r%d - Application CASM \n\n\n",version,release);
		fprintf(object_file,"\t.org\ta30\nstart:\tadd.d\ta30,#data_space\n");
		gen_fixup_code();
	
		/* the call the recursive generate */
		stack = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		gen_block_code(tree_base,stack,0,0);
	
		/* the write the trailer info here */
		gen_instr(null,op,op,0);	/* grab the loose labels */
		fprintf(object_file,"\tcall.d\ta29(#prog_exit)\n");

		/* now generate the code for the whenevers */
		gen_when_code(stack);

		fprintf(object_file,"data_space:\n");
		gen_the_dbs();

		fprintf(object_file,"variable_space:\n");
		if(max_memory_size > 0)
			fprintf(object_file,"\tdb.b\t$00 times %d\n",max_memory_size);
		
		/* lets now realease the link list */
		free_link(stack);
		break;
	};

	fprintf(object_file,"the_end:\n");
}
