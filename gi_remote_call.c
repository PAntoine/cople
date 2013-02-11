/* $Revision: 1.2 $ (generate_intel)
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
#include	"intel.h"
 
void	gi_remote_call(CLAUSE* call_clause,LEVEL_LNK* stack)
{
	int	count=0,stack_size=0;
	char	opStr[50];
	INTEL_OPERAND	op = {0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
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
			fprintf(object_file,"\tpush\tdword 0\n");
		else {
			fprintf(object_file,"\tpush\tdword %d\n",((CONSTANT*)walk->data.argument.name)->data.string);
		};
		
		/* Now need to push the reference for the variables that 
		 * have to be placed, or the offset. It will also set the
		 * flag for the type of variable set. As we are passing
		 * references - lest set the op to be a pointer.
		 */
		
		op = gi_stat_code(walk->data.argument.value,stack,0,0);
		op.state.pointer = 1;
		gi_operand(&op,opStr);
	
		fprintf(object_file,"\tpush\tdword %s\n",opStr);
		
		/* For all far calls - where the parameter is a variable then it 
		 * is possible that it can be a reference parameter. It is not
		 * possible to tell till the call has been made to the remote
		 * system - so all values that can be referenced must have the
		 * base value passed so - if they are references then the values
		 * can be passed. If a service or a constant generates the parameter
		 * then it cannot be referenced.
		 */

		if ( (walk->data.argument.a_type & a_const) || (walk->data.argument.a_type & a_service))
		{
			fprintf(object_file,"\tpush\tbyte 00\n");	/*not referencable*/
		} else {
			fprintf(object_file,"\tpush\tbyte 01\n");	/*referencable*/
		};

		stack_size += 5;
		count++;
		walk = walk->next;
	};

	/* Now push the number of parameters that have been pushed */	
	fprintf(object_file,"\tpush\tword %d\n",count);

	/* need to get the object parameter - 3 dwords*/
	op = gi_stat_code(call_clause->c_call.object,stack,0,0);
	gi_operand(&op,opStr);
	fprintf(object_file,"\tpush\tdword %s\n",opStr);

	op.value += 4;
	gi_operand(&op,opStr);
	fprintf(object_file,"\tpush\tdword %s\n",opStr);
	
	op.value += 4;
	gi_operand(&op,opStr);
	fprintf(object_file,"\tpush\tdword %s\n",opStr);
	
	/* And finally - the service name - reference local*/
	fprintf(object_file,"\tpush\tdword %d\n",call_clause->c_call.service->data.string);

	/* And now do the call */
	fprintf(object_file,"\tcall\tservice_call:dword 0\n");
	fprintf(object_file,"\tadd\tesp,%d\n",20 + (count*9));	/* tidy the stack */

	/* do the stack thing */
	stack_size += 18;

	if (stack_size > stack->count)
		stack->count = stack_size;
}
