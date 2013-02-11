/* $revision$
 *
 *		      Generate Block Code
 *
 *	This function will generate the code for an "enter" block. It only will]
 *	cause a "stack frame" to be created if the amount of data is greater than
 *	zero. If not it will carry on the block in the previous stack frame. It
 *	will then call for all instructions in the levels instruction list the
 *	generate instruction code function, which really does all the work.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	<stdio.h>
#include	"codetree.h"

void	gen_block_code(CLAUSE *clause, LEVEL_LNK *stack,int regno,int addrno)
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
		op = get_label();
		op1.type  = o_reg;
		op1.reg   = regno;
		fprintf(object_file,"\n\tmove.d\td%d,#%d\n",regno,clause->c_block.local_size);
		fprintf(object_file,"\txor.b\td%d,d%d\n",regno+1,regno+1);
		fprintf(object_file,"\tmove.d\td%d,#%d\n",regno+2,clause->c_block.local_base+initial_size);
		put_label(op);
		gen_labels();
		fprintf(object_file,"\tmove.b\td%d,a30(d%d)+\n",regno+1,regno+2);
	 	gen_instr(loop,op1,op,addrno);

		/* now need to walk the locals list to find any stuff that needs copying */
		for (count=0; count < clause->c_block.num_locals;count++)
		{
			if ((clause->c_block.locals[count]->data != 0) &&
			    (clause->c_block.locals[count]->offset != clause->c_block.locals[count]->data))
			{
				if (clause->c_block.locals[count]->type == d_array)
				{
					/* this code generates the copy from common to local */
					op1.type  = o_reg;
					op1.reg   = regno+2;
					fprintf(object_file,"\tmove.d\td%d,#%d\n",regno,
						clause->c_block.locals[count]->offset+initial_size);
					fprintf(object_file,"\tmove.d\td%d,#%d\n",regno+1,
						clause->c_block.locals[count]->data);
					fprintf(object_file,"\tmove.d\td%d,#%d\n",regno+2,
						clause->c_block.locals[count]->size);
					op = get_label();
					put_label(op);
					gen_labels();

					fprintf(object_file,"\tmove.b\ta30(d%d)+,a30(d%d)+\n",
							regno,regno+1);
	 				gen_instr(loop,op1,op,addrno);
				} else {
					/* simple copy */
					fprintf(object_file,"\tmove.%c\ta30(#%d),a30(#%d)\n",
						INSTR_SIZE[clause->c_block.locals[count]->type],
						clause->c_block.locals[count]->offset+initial_size,
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
		gen_stat_code(clause->c_block.instr_list[count],stack,regno,addrno);

	/* now need to do any whenever scope changes */
	for(count=0;count<clause->c_block.num_when;count++)
		fprintf(object_file,"\tmove.d\ta30(#%d),#when_label_%d\n",
			clause->c_block.whenc_list[count].exception_num * 4,
			clause->c_block.whenc_list[count].old_label);

	/* walk back up the tree. */
	current_block = hold;
	level_at--;
}
