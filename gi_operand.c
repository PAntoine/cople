/* $Revision: 1.2 $ (generate intel)
 *
 *		    Generate Operand
 *
 *	This function genrate an intel operand string. It will also genrate any
 *	code that needs to be generated to recover and saved or outdated registers.
 *	This includes loading any segment registers that need loading.
 *
 *		Peter Antoine (c) 1999.
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"intel.h"
#include	"cople.h"
#include	"codetree.h"

char*	gi_operand(INTEL_OPERAND *op,char* result)
{
	int		count,found = 0, size = 0;
	char		number[11];
	extern int		click_count;
	extern FILE		*object_file;
	extern SEG_REGISTER	*seg_reg;
	extern VIRT_REGISTER	*virt_reg;
	extern REAL_REGISTER	*real_reg;

	click_count++;
	result[0] = '\0';

	if (op->state.parameter)
	{
		/* no reference - [ss:ebp+value] */

		/* if reference - les eax,[ss:ebp+offset] then [es:eax] */

		/* if dynamic - les eax, [ss:ebp+offset] then les eax, [es:eax] then [es:eax] */
	} else {
		if (op->state.reference)
		{
			size = 2;
			result[0] = '[';
			result[1] = '\0';
		};

		if (op->state.pointer)
		{
			/* if the operand is a pointer then the target value needs
			 * to be loaded. So I will load the value into a register
			 * then set the register bit so that the code lower will then
			 * use the register. Also the immediate value is not needed.
			 */

			allocate_register(op);
			fprintf(object_file,"\tmov\t%s, [%d]\n",INTEL_REG[virt_reg[op->reg_no].reg_no],op->value);
			op->state.immediate = 0;
		};

		if (op->state.dynamic)
		{
			/* check to see if the segment register has been loaded
			 * allready with the segment value.
			 */
			if (seg_reg[0].used_by == op->value)
				found = 1;
			if (seg_reg[1].used_by == op->value)
				found = 2;
			if (seg_reg[2].used_by == op->value)
				found = 3;

			/* if not - generate the load register code and set the
			 * register up.
			 */
			if (!found)
				found = load_seg_reg(op->value);

			/* add to string */
			size += 5;
			result[1] = INTEL_SEG[found][0];
			result[2] = INTEL_SEG[found][1];
			result[3] = ':';
			result[4] = '\0';

			/* update the click count */
			seg_reg[found-1].last_used = click_count;
		};

		if ( op->state.immediate )
		{
			sprintf(number,"%d",op->value);
			size += strlen(number);
			strcat(result,number);
		};

		if ( op->state.use_reg )
		{
			/* need to get back any registers that have been saved */
			if (virt_reg[op->reg_no].saved)
				pop_register(op->reg_no);

			/* gen the register string bit */
			size += 3;
			strcat(result,INTEL_REG[virt_reg[op->reg_no].reg_no]);

			/* update the click count */
			real_reg[virt_reg[op->reg_no].reg_no].last_used = click_count;
		};

		if (op->state.use_base)
		{
			/* there is a base offset to add */
			sprintf(number,"+%d",op->base);
			size += strlen(number);
			strcat(result,number);
		};
	
		if (op->state.reference)
		{
			size++;
			result[size-2] = ']';
			result[size-1] = '\0';
		};
	};
	return result;
}
