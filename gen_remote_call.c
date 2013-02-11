/* $Revision: 1.2 $
 *
 *		       Generate Remote Call
 *
 *	This function will generate the remote call code. Note this function will
 *	generate the stack pushs. This function generates a call to SAPI function
 *	service_call and this will do the actual service call. This function will
 *	also do any reference write-backs that are nessary. Any parameter that
 *	has a value the can be refrenced will have the reference flag for the
 *	parameter set to 1, else 0.
 *
 *	Paramter Format:
 *
 *		dword	parameter name reference
 *		dword	parameter value/reference
 *		byte	1 if reference else 0
 *
 *		Copyright (c) 1999 Peter Antoine	
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"codetree.h"
#include	"cople.h"

 
void	gen_remote_call(CLAUSE* call_clause,LEVEL_LNK* stack,int regno,int addrno)
{
	int	count=0;
	OPERAND	op,op1;
	LIST_ENTRY	*walk;
	extern int	initial_size;
	extern FILE	*object_file;

	walk = call_clause->c_call.list->list;

	while(walk != NULL)
	{
		/* push the name pointer - if not a named paramter it will
		 * just be zero - which is OK. This is a run-time fixup.
		 */
		
		if (walk->data.argument.name == 0)
			fprintf(object_file,"\tpush.d\t#0\n");
		else {
			fprintf(object_file,"\tmove.d\ta%d,a30\n",addrno);
			fprintf(object_file,"\tadd.d\ta%d,#%d\n",addrno,((CONSTANT*)walk->data.argument.name)->data.string);
			fprintf(object_file,"\tpush.d\ta%d\n",addrno);
		};
		
		/* Now need to push the reference for the variables that 
		 * have to be placed, or the offset. It will also set the
		 * flag for the type of variable set.
		 */
		
		op = gen_stat_code(walk->data.argument.value,stack,regno,addrno);
		
		/* For all far calls - where the parameter is a variable then it 
		 * is possible that it can be a reference parameter. It is not
		 * possible to tell till the call has been made to the remote
		 * system - so all values that can be referenced must have the
		 * base value passed so - if they are references then the values
		 * can be passed.
		 */
		switch(op.type)
		{
		case o_array_ref:
			fprintf(object_file,"\tmove.d\ta%d,a30\n",addrno);
			if (op.offset != 0)
				fprintf(object_file,"\tadd.d\ta%d,#%d\n",addrno,op.offset);
			
			fprintf(object_file,"\tadd.d\ta%d,d%d\n",addrno,op.reg);
			fprintf(object_file,"\tpush.d\ta%d\n",addrno);
			
			fprintf(object_file,"\tpush.b\t#01\n");	/* referencable*/
			break;

		case o_variable:
			fprintf(object_file,"\tmove.d\ta%d,a30\n",addrno);
			fprintf(object_file,"\tadd.d\ta%d,#%d\n",addrno,op.offset);
			fprintf(object_file,"\tpush.d\ta%d\n",addrno);
			
			fprintf(object_file,"\tpush.b\t#01\n");	/* referencable*/
			break;

		case o_parameter:
			fprintf(object_file,"\tmove.d\ta%d,a31\n",addrno);
			if (op.offset != 0)
				fprintf(object_file,"\tadd.d\ta%d,#%d\n",addrno,op.offset);
			
			fprintf(object_file,"\tpush.d\ta%d\n",addrno);
			
			fprintf(object_file,"\tpush.b\t#01\n");	/* referencable (possibly - but wont fail)*/
			break;

		case o_reg:	fprintf(object_file,"\tpush.d\td%d\n",op.reg);
			
			fprintf(object_file,"\tpush.b\t#00\n");	/*not referencable*/
			break;

		case o_constant:
			fprintf(object_file,"\tpush.d\t#%d\n",op.data);
			
			fprintf(object_file,"\tpush.b\t#00\n");	/*not referencable*/
			break;

		case o_var_ref: fprintf(object_file,"\tmove.d\td%d,a30\n",op.reg);
			fprintf(object_file,"\add.d\td%d,#%d\n",op.reg,op.offset);
			fprintf(object_file,"\tpush.d\td%d\n",op.reg);
			
			fprintf(object_file,"\tpush.b\t#00\n");	/*not referencable*/
			break;

		case o_ptr_ref:	fprintf(object_file,"\tmove.d\ta%d,a30(#%d)\n",addrno,op.offset);
			fprintf(object_file,"\tpush.d\ta%d\n",addrno);
			
			fprintf(object_file,"\tpush.b\t#00\n");	/*not referencable*/
			break;
		};

		count++;
		walk = walk->next;
	};

	/* Now push the number of parameters that have been pushed */	
	op.type      = o_constant;
	op.offset    = 0;
	op.data_type = c_int;
	op.data      = count;
	gen_instr(push,op,op1,addrno);

	/* need to get the object parameter */
	if (call_clause->c_call.object->c_variable.value.type == c_reference) 
	{
		if (call_clause->c_call.object->c_variable.value.data.symbol->access_type & a_global)
			op.offset = call_clause->c_call.object->c_variable.value.data.symbol->offset;
		else
			op.offset = initial_size + call_clause->c_call.object->c_variable.value.data.symbol->offset;
	
		fprintf(object_file,"\tmove.d\ta%d,a30(#%d)\n",addrno,op.offset);
	}else{
		fprintf(object_file,"\tmove.d\ta%d,#%d\n",addrno, call_clause->c_call.object->c_variable.value.data.integer);
		fprintf(object_file,"\tadd.d\ta%d,a30\n",addrno);
	};
	
	fprintf(object_file,"\tpush.d\ta%d(#0)\n",addrno);
	fprintf(object_file,"\tpush.d\ta%d(#4)\n",addrno);
	fprintf(object_file,"\tpush.d\ta%d(#8)\n",addrno);
	
	/* And finally - the service name */
	fprintf(object_file,"\tpush.d\ta30(#%d)\n",call_clause->c_call.service->data.string);

	/* And now do the call */
	fprintf(object_file,"\tcall.d\ta29(#service_call)\n");
	fprintf(object_file,"\tadd.d\tsp,#%d\n",20 + (count*9));	/* tidy the stack */
}
