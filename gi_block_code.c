/* $revision$ (generate intel)
 *
 *		      Generate Block Code
 *
 *	This function will generate the code for an "enter" block. It only will
 *	cause a "stack frame" to be created if the amount of data is greater than
 *	zero. If not it will carry on the block in the previous stack frame. It
 *	will then call for all instructions in the levels instruction list the
 *	generate instruction code function, which really does all the work.
 *
 *		Copyright (c) 1999 Peter Antoine.
 *
 *	Note: The data is not on the stack, but in the programs data area.
 *	 what happens is the code knows where the data is. So, if a program
 *	 has more than on sub-block the same area will be re-used, this means
 *	 that these sub-blocks must be re-initialised each time they are entered. 
 *		
*/

#include	<stdio.h>
#include	"intel.h"
#include	"codetree.h"

void	gi_block_code(CLAUSE *clause, LEVEL_LNK *stack,int regno)
{
	int	size,count;
	CLAUSE	*hold;
	OPERAND	op,op1;
	LEVEL_LNK	*new_stack;
	extern int	initial_size;
	extern int	level_at;
	extern FILE	*object_file;
	extern CLAUSE	*current_block;

	/* if there is local space, it needs to be cleared (al-la pascal)
	 * and if any of the variables are initialised with a value this
	 * will also need to be done.
	 */
	
	level_at++;
	
	if ((clause->c_block.local_size != 0) && (level_at > 1))
	{	
		/* this gens code that will clear a block space */
		fprintf(object_file,"\txor\teax, eax\n");
		fprintf(object_file,"\tmov\tedi,$0%d\n",clause->c_block.local_base);

		/* do the lumps of 4 */
		if( (clause->c_block.local_size / 4) != 0)
		{
			fprintf(object_file,"\tmov\tecx,dword $0%d\n",(clause->c_block.local_size/4));
			fprintf(object_file,"\trep stosd\n");
		};

		/* anything left over */
		if ( (clause->c_block.local_size % 4) != 0)
		{
			fprintf(object_file,"\tmov\tecx,dword $0%d\n",(clause->c_block.local_size%4));
			fprintf(object_file,"\trep stosb\n");
		};	

		/* now need to walk the locals list to find any stuff that needs copying */
		for (count=0; count < clause->c_block.num_locals;count++)
		{
			if ((clause->c_block.locals[count]->data != 0) &&
			    (clause->c_block.locals[count]->offset != clause->c_block.locals[count]->data))
			{
				if (clause->c_block.locals[count]->type == d_array)
				{
					/* do the lumps of 4 */
					if( (clause->c_block.locals[count]->size / 4) != 0)
					{
						fprintf(object_file,"\tmov\tecx,dword $0%d\n",
						(clause->c_block.locals[count]->size/4));
						fprintf(object_file,"\tmov\tedi,$0%d\n",
							clause->c_block.locals[count]->offset);
						fprintf(object_file,"\tmov\tesi,$0%d\n",
							clause->c_block.locals[count]->data);
		
						fprintf(object_file,"\trep stosd\n");
					};

					/* anything left over */
					if ( (clause->c_block.locals[count]->size % 4) != 0)
					{
						fprintf(object_file,"\tmov\tecx,dword $0%d\n",
							(clause->c_block.locals[count]->size%4));
						fprintf(object_file,"\tmov\tedi,$0%d\n",
							clause->c_block.locals[count]->offset);
						fprintf(object_file,"\tmov\tesi,$0%d\n",
							clause->c_block.locals[count]->data);

						fprintf(object_file,"\trep stosb\n");
					};
				} else {
					/* simple copy */
					fprintf(object_file,"\tmov\t%s eax, [$0%d]\n",
						INTEL_SIZE[clause->c_block.locals[count]->type],
						clause->c_block.locals[count]->offset);

					fprintf(object_file,"\tmov\t%s [$0%d], eax\n",
						INTEL_SIZE[clause->c_block.locals[count]->type],
						clause->c_block.locals[count]->data);
				};
			};
		};
	};
	 
	/* need to keep track of the block tree for find_variable and
	 * find_operand that needs an up to date current_block. Note:
	 * am not using owner field as this will end the walk in service
	 * blocks - which we dont want to do.
	 */
	hold = current_block;
	current_block = clause;
	
	/* this is the real work */
	for( count=0; count<clause->c_block.num_instr;count++)
		gi_stat_code(clause->c_block.instr_list[count],stack,regno,0);

	/* now need to do any whenever scope changes */
	for(count=0;count<clause->c_block.num_when;count++)
		fprintf(object_file,"\tmov\tdword [%d], when_label_%d\n",
			clause->c_block.whenc_list[count].exception_num * 4,
			clause->c_block.whenc_list[count].old_label);

	/* walk back up the tree. */
	current_block = hold;
	level_at--;
}
