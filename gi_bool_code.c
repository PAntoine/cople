/* $Revision: 1.2 $ (generate_intel)
 *
 *		  Generate boolean Instructions
 *
 *	This function will genrate the boolean instructions for the compiler. It will 
 *	free the registers that are needed for the intel specific instructions that
 *	need certain registers.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include <stdio.h>
#include "intel.h"


INTEL_OPERAND	gi_bool_code(INST_TYPE instr, INTEL_OPERAND left, INTEL_OPERAND right,int jmp_label)
{
	char		opStr1[50];
	char		opStr2[50];
	char		tempStr[50];
	extern FILE		*object_file;
	INTEL_OPERAND		temp={0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
	INTEL_OPERAND		temp2={0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
	INTEL_OPERAND		result={0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
	extern int		es_dirty;
	extern SEG_REGISTER	*seg_reg;
	extern VIRT_REGISTER	*virt_reg;
	extern REAL_REGISTER	*real_reg;

	switch(instr)
	{
	case store:	 /* cant move memory to memory */
		if (left.state.reference && right.state.reference)
		{
			if (!right.state.use_reg)
			{
				allocate_register(&result);
				gi_operand(&right,opStr2);
				gi_operand(&result,opStr1);
				fprintf(object_file,"\tmov\t%s %s,%s\n",INTEL_SIZE[left.type],opStr1,opStr2);
			}else{
				gi_operand(&right,opStr2);
				
				/* dont want any nasty surprises */
				right.state.reference = 0;
				right.state.dynamic   = 0;
				right.state.pointer   = 0;
				
				gi_operand(&right,opStr1);
				fprintf(object_file,"\tmov\t%s %s,%s\n",INTEL_SIZE[left.type],opStr1,opStr2);
			};
				
		}else
			gi_operand(&right,opStr1);
		
	
		/* now move the data */
		gi_operand(&left,opStr2);
		fprintf(object_file,"\tmov\t%s %s,%s\n",INTEL_SIZE[left.type],opStr2,opStr1);
		free_register(&result);
		break;
	case add:	
	case sub:
	case and:
	case or:		
	case xor:
	case equal:
	case not_eq:
	case less_than:
	case less_equal:
	case greater_than:	/* some data types i.e. strings need a function call to check*/
	case greater_equal:	
		if (left.type == d_string)
		{
			/* genrate the string code */
			if (instr == add || instr == sub)
			{
				comp_error(WARNING,"String add/subs not supported yet - next relaese");
			} else {
				/* get the string registers */
				push_register(esi);
				push_register(edi);
				push_register(ecx);
					
				real_reg[esi].inuse = 1;
				real_reg[edi].inuse = 1;
				real_reg[ecx].inuse = 1;
				
			/*	fprintf(object_file,"\tmov\t[ds_save],ds\n");	next release */
			/*	fprintf(object_file,"\tmov\t[es_save],es\n");	next release */

				/* we want the pointers for the strings and not the
				 * content. So we remove the reference part of the
				 * string.
				 * NOTE: dynamic strings will be treated difference
				 * will have to load the seg register.
				 */

				left.state.pointer = 0;
				right.state.pointer = 0;
				 
				gi_operand(&left,opStr1);
				gi_operand(&right,opStr2);
				
			/*	fprintf(object_file,"\tlds\tesi,%s \n",opStr1);	next release */
			/*	fprintf(object_file,"\tles\tedi,%s \n",opStr2);	next release */
				
				fprintf(object_file,"\tmov\tesi,%s \n",opStr1);
				fprintf(object_file,"\tmov\tedi,%s \n",opStr2);
				
				/* load the size */
				fprintf(object_file,"\tmovzx\tecx,byte [esi]\n");
				fprintf(object_file,"\trep cmpsb\n");

				/* if jmp_label == 0 then return the value in op else jcc to jmp_label*/
				if (jmp_label == 0)
					fprintf(object_file,"\tset%s\t%s\n",INTEL_INSTR[instr],opStr1);
				else
					fprintf(object_file,"\tj%s\tnear label_%d\n",INTEL_INSTR[instr],jmp_label);
			
				/* release the real registers */
				real_reg[esi].inuse = 0;
				real_reg[edi].inuse = 0;
				real_reg[ecx].inuse = 0;
			
			/*	fprintf(object_file,"\tmov\tds,[ds_save]\n");	next release */
			/*	fprintf(object_file,"\tmov\tes,[es_save]\n");	next release */
			};
		
		} else {
			/* if the left op is not a reg, get a reg and copy left to reg */
			if (!left.state.use_reg)
			{
				/* get a register and move left to it */
				result.type = left.type;
				allocate_register(&result);
				gi_operand(&left,opStr1);
				gi_operand(&result,opStr2);
				fprintf(object_file,"\tmov\t%s %s,%s\n",INTEL_SIZE[left.type],opStr2,opStr1);
			}else
				result = left;
	
			/* now compare left, to right */
			gi_operand(&result,opStr1);
			gi_operand(&right,opStr2);
	
			/* do the comparesion - the result is returned in result */
			if (instr == equal || instr == not_eq || instr == less_than ||
				instr == less_equal || instr == greater_than || instr == greater_equal) 
			{
				fprintf(object_file,"\tcmp\t%s %s,%s\n", INTEL_SIZE[left.type],opStr1,opStr2);
				
				/* if jmp_label == 0 then return the value in op else jcc to jmp_label*/
				if (jmp_label == 0)
					fprintf(object_file,"\tset%s\t%s\n",INTEL_INSTR[instr],opStr1);
				else
					fprintf(object_file,"\tj%s\tnear label_%d\n",INTEL_INSTR[instr],jmp_label);
			
			/* do the add/sub/xor/or/and and return in result */
			} else {
				fprintf(object_file,"\t%s\t%s %s,%s\n",
					INTEL_INSTR[instr],INTEL_SIZE[left.type],opStr1,opStr2);
			};
		};
		if (!(result.state.use_reg && left.state.use_reg && left.reg_no == result.reg_no))
			free_register(&left);
		break;

	case multi:
		if (left.type == d_real)
		{
			/* floating point stuff */
		}else{
			if (!left.state.use_reg)
			{
				/* get a register and move left to it */
				result.type = left.type;
				allocate_register(&result);
				gi_operand(&result,opStr1);
				gi_operand(&left,opStr2);

				if (INTEL_SIZE[left.type] != "dword")
					fprintf(object_file,"\tmovzx\t%s %s,%s\n",
						INTEL_SIZE[left.type],
						opStr1,
						opStr2);
				else
					fprintf(object_file,"\tmov\tdword %s,%s\n",
						opStr1,
						opStr2);
			}else{
				/* set up like above */
				result = left;
				gi_operand(&result,opStr1);
			};

			gi_operand(&right,opStr2);

			fprintf(object_file,"\t%s\t%s %s,%s\n",
				INTEL_INSTR[instr],
				INTEL_SIZE[left.type],
				opStr1,opStr2);
		};
		
		if (!(result.state.use_reg && left.state.use_reg && left.reg_no == result.reg_no))
			free_register(&left);
		break;

	case modulo:		
	case divide:	
		/* pure numerics - both floats and ints */
		if (left.type == d_real)
		{
			/* floating point stuff */
		}else{
			/* need to assign edx - make sure its not used.*/
			push_register(edx);
			real_reg[edx].inuse = 1;
			real_reg[edx].last_used = -1;
			real_reg[edx].virt_reg_no = -1;
			fprintf(object_file,"\txor\tedx, edx\n");

			/* need to move left to eax.
			 * If nessary need to free it first, then load it.
			 */
		
			if (left.state.use_reg && virt_reg[left.reg_no].reg_no == eax)
				result = left;
			else{
				/* if left is a register then swap it with eax */
				if (left.state.use_reg){
					swap_registers(eax,virt_reg[left.reg_no].reg_no);
			
					real_reg[eax].inuse = 1;
					real_reg[eax].last_used = -1;
					real_reg[eax].virt_reg_no = -1;
					result = left;
				}else{
					/* left is not a register - need to load it */
					allocate_register(&result);
				
					if(virt_reg[result.reg_no].reg_no != eax)
						swap_registers(eax,virt_reg[result.reg_no].reg_no);
					
					/* mark eax as used */
					real_reg[eax].inuse = 1;
					real_reg[eax].last_used = -1;
					real_reg[eax].virt_reg_no = -1;
					
					/* load eax */
					gi_operand(&result,opStr1);
					gi_operand(&left,opStr2);
					
					if (INTEL_SIZE[left.type] == "dword")
						fprintf(object_file,
							"\tmov\tdword %s, %s\n",
							opStr1,opStr2);
					else
						fprintf(object_file,
							"\tmovzx\t%s %s, %s\n",
							INTEL_SIZE[left.type],
							opStr1,opStr2);
				};
			};
			
			/* need to check for immediate divisor - if so need to
			 * make it a register.
			 */
			if (right.state.immediate && !right.state.reference)
			{
				allocate_register(&temp);
				gi_operand(&temp,opStr1);
				gi_operand(&right,opStr2);
				
				if (INTEL_SIZE[right.type] == "dword")
					fprintf(object_file,"\tmov\tdword %s, %s\n",
						opStr1,opStr2);
				else
					fprintf(object_file,"\tmovzx\t%s %s, %s\n",
						INTEL_SIZE[right.type],
						opStr1,opStr2);
			}else 
				/* need to get the operand for the divisor */
				gi_operand(&right,opStr1);		
			
			/* do div/modulo */
			fprintf(object_file,"\t%s\t%s %s\n",
				INTEL_INSTR[instr],
				INTEL_SIZE[left.type],
				opStr1);

			/* if modulo swap edx, eax */
			if( instr == modulo)
				fprintf(object_file,"\txchg\tdword eax, edx\n");

			/* free the registers that i am using */
			if (!(left.state.use_reg && virt_reg[left.reg_no].reg_no == eax))
			{
				real_reg[eax].inuse = 0;
				real_reg[edx].virt_reg_no = 0;
				real_reg[edx].last_used   = 0;
			};
			
			real_reg[edx].inuse = 0;
			real_reg[edx].virt_reg_no = 0;
			real_reg[edx].last_used   = 0;
			free_register(&temp);
		};
		break;
	
	case load:	comp_error(FATAL_ERROR,"Generated a load instruction - interesting!");
		break;

	default:
		comp_error(FATAL_ERROR,"found a new binary instruction have we?");
	};

	/* release the registers here - they will be freed later but
	 * this is more register efficent to free them here.
	 */
	 
	if (!result.state.use_reg || left.reg_no != result.reg_no)
		free_register(&left);
	
	if (!(result.state.use_reg && right.state.use_reg && right.reg_no == result.reg_no))
		free_register(&right);

	return result;
}

