/* $Revision: 1.2 $ (generate intel)
 *
 *		        Register Handling Functions
 *
 *		allocate_register, free_register, push_register
 *		  pop_register, swap_registers, load_seg_reg
 *
 *	These functions will handle the operation of the registers for the x86 processor.
 *	They will genrate any code that is nessary for the moving of registers. swap 
 *	register is used to free specific registers that are needed for specific 
 *	instructions.
 *
 *		          Peter Antoine (c) 1999.
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"intel.h"

extern int		register_no;
extern int		initial_size;
extern FILE		*object_file;
extern int		es_dirty;
extern int		click_count;
extern int		max_save_space;
extern int		current_saved;
extern long		*save_space;
extern SEG_REGISTER	*seg_reg;
extern VIRT_REGISTER	*virt_reg;
extern REAL_REGISTER	*real_reg;

/*------------------------------------------------------------------*
 * allocate_register
 *
 * This will allocate a real register to an operand. It will cause
 * the generation of any code that is needed to save any registers.
 *------------------------------------------------------------------*/

void	allocate_register(INTEL_OPERAND *op)
{
	int	found = 0;
	int	count;

	/* find a virtual register */
	for(count=0;count<MAX_VIRTUAL_REGISTERS && !found;count++)
	{
		if (!virt_reg[count].inuse)
		{
			found = count;
			virt_reg[count].inuse = 1;
		};
	};

	if (!found)
		comp_error(FATAL_ERROR,"max'ed out the registers - how did you do that? send me the code!");
	else
		if (found > register_no)
			register_no = found;

	/* get a real register */
	find_register(found);

	/* set up the operand */
	op->reg_no = found;
	op->state.use_reg = 1;
}

/*------------------------------------------------------------------*
 * free_register
 *
 * This function will release a virtual_register and its assoicated
 * real_register.
 *------------------------------------------------------------------*/

void	free_register(INTEL_OPERAND *op)
{
	if (op->state.use_reg)
	{
		if (!virt_reg[op->reg_no].saved)
		{
			real_reg[virt_reg[op->reg_no].reg_no].virt_reg_no = 0;
			real_reg[virt_reg[op->reg_no].reg_no].inuse = 0;
		};
		virt_reg[op->reg_no].inuse = 0;
	};

	op->reg_no = 0;
	op->state.use_reg = 0;
}

/*------------------------------------------------------------------*
 * push_register
 *
 * This will save a named register. It will incress the size of the
 * saved space if it needs more.
 *------------------------------------------------------------------*/

int	push_register(int reg_no)
{
	if (virt_reg[reg_no].inuse && real_reg[reg_no].inuse)
	{
		current_saved++;
		
		if ( current_saved > max_save_space)
		{
			max_save_space = current_saved;
		};

		/* generate the save register code */
		fprintf(object_file,"\tmov\tdword [save_space+%d*4], %s\n",current_saved,INTEL_REG[reg_no]);
	
		/* mark the virtual register as saved and free the real register*/
		real_reg[reg_no].inuse = 0;
		virt_reg[real_reg[reg_no].virt_reg_no].saved = 1;
		virt_reg[real_reg[reg_no].virt_reg_no].save_space = current_saved;
	
		return 1;
	}else
		return 0;
}

/*------------------------------------------------------------------*
 * pop_register
 *
 * This function will restore a saved register and descress the
 * saved space pointer.
 *------------------------------------------------------------------*/

int	pop_register(int reg_no)
{
	if (virt_reg[reg_no].saved)
	{
		if (current_saved != virt_reg[real_reg[reg_no].virt_reg_no].save_space)
			printf("NOTE: Not working as as stack!!!!\n");

		find_register(reg_no);

		/* generate the load register code */
		fprintf(object_file,"\tmov\t%s, dword [save_space+%d*4]\n",INTEL_REG[virt_reg[reg_no].reg_no],current_saved);
		
		/* mark the real register as inuse */
		real_reg[virt_reg[reg_no].reg_no].inuse = 1;
		
		/* now free the save slot */
		current_saved--;
	
		return 1;
	}else
		return 0;
}

/*------------------------------------------------------------------*
 * swap_registers
 *
 * This function will swap two registers. If either of the registers
 * is in use it will do a move, if both are in use a swap, else it
 * will do nothing. It keeps the virtual table updated.
 *------------------------------------------------------------------*/

void	swap_registers(int reg_1, int reg_2)
{
	int	save_reg;

	if (virt_reg[real_reg[reg_1].virt_reg_no].inuse && virt_reg[real_reg[reg_2].virt_reg_no].inuse)
	{
		save_reg = real_reg[reg_1].virt_reg_no;

		/* swap 2 to 1 */
		virt_reg[real_reg[reg_1].virt_reg_no].reg_no = reg_2;
		real_reg[reg_1].virt_reg_no = real_reg[reg_2].virt_reg_no;
		
		/* swap save to 1 */
		virt_reg[real_reg[reg_2].virt_reg_no].reg_no = reg_1;
		real_reg[reg_2].virt_reg_no = save_reg;
		
		/* new gen the swap code */
		fprintf(object_file,"\txchg\tdword %s, %s\n",INTEL_REG[reg_1],INTEL_REG[reg_2]);
	}
	else if (virt_reg[real_reg[reg_1].virt_reg_no].inuse)
	{
		/* move (real) reg1 to reg2 - also need to mark reg1 as
		 * not in use, and reg2 as being inuse. virt for reg1 changes.
		 */
		virt_reg[real_reg[reg_1].virt_reg_no].reg_no = reg_2;
		real_reg[reg_2].virt_reg_no = real_reg[reg_1].virt_reg_no;
		real_reg[reg_1].virt_reg_no = 0;
		
		/* mov reg1 to reg2 */
		fprintf(object_file,"\tmov\tdword %s, %s\n",INTEL_REG[reg_2],INTEL_REG[reg_1]);
	}
	else if (virt_reg[real_reg[reg_2].virt_reg_no].inuse)
	{
		/* move (real) reg2 to reg1 - also need to mark reg2 as
		 * not in use, and reg1 as being inuse. virt for reg2 changes.
		 */
		virt_reg[real_reg[reg_2].virt_reg_no].reg_no = reg_1;
		real_reg[reg_1].virt_reg_no = real_reg[reg_2].virt_reg_no;
		real_reg[reg_2].virt_reg_no = 0;
		
		/* mov reg2 to reg1 */
		fprintf(object_file,"\tmov\tdword %s, %s\n",INTEL_REG[reg_1],INTEL_REG[reg_2]);
	};
}

/*------------------------------------------------------------------*
 * find_register
 *
 * This function will find a free register and allocate to the 
 * virtual register number. If no registers are free it will dump
 * a register and use that.
 *------------------------------------------------------------------*/

void	find_register(int regi_no)
{
	int	count,found = 0;
	int	oldest = -1;

	for (count=0;count<6 && !found;count++)
	{
		/* real register is not in use */
		if (real_reg[count].virt_reg_no == 0)
		{
			virt_reg[regi_no].inuse = 1;
			virt_reg[regi_no].saved = 0;
			virt_reg[regi_no].reg_no = count;
			real_reg[count].virt_reg_no = regi_no;

			found = 1;
		};
	};

	/* no free registers need to unload one */
	if (!found)
	{
		for (count=0;count<6;count++)
		{
			/* find the register that was last used */
			if (oldest > real_reg[count].last_used)
			{
				oldest = real_reg[count].last_used;
				found  = count;
			};
		};

		/* do the unload and set the virtual register */
		push_register(found);
		virt_reg[regi_no].inuse = 1;
		virt_reg[regi_no].saved = 0;
		virt_reg[regi_no].reg_no = found;
		real_reg[found].inuse = 1;
		real_reg[found].virt_reg_no = regi_no;
		
	};
}

/*------------------------------------------------------------------*
 * load_seg_reg
 *
 * This function will load the segment register with the segment
 * value. If register 3 (ES) is loaded then it will set the ES
 * dirty bit as ES shadows the DS and it is used in the system
 * code.
 *------------------------------------------------------------------*/

int	load_seg_reg(unsigned long s_reg)
{
	int	found;
	unsigned int	oldest = -1;

	
	if (seg_reg[0].last_used < oldest)
	{
		oldest = seg_reg[0].last_used;
		found = 1;
	};
	
	if (seg_reg[1].last_used < oldest)
	{
		oldest = seg_reg[1].last_used;
		found = 2;
	};

	if (seg_reg[2].last_used < oldest)
	{
		es_dirty = 1;
		oldest = seg_reg[2].last_used;
		found = 3;
	};
	
	seg_reg[found-1].used_by = s_reg;
	seg_reg[found-1].last_used = click_count;
	
	fprintf(object_file,"\tmov\tword %s, [%d]\n",INTEL_SEG[found],s_reg);
	
	return found-1;
}

