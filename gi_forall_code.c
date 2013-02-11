/* $Revision: 1.2 $ (generate_intel)
 *
 *		      Generate Forall Code
 *
 *	This code will generate the forall call code. It uses two system calls
 *	the first: find_object, this will find a named object. It also returns
 *	the instance number of the first object of that name found. The other
 *	is find_next_object that takes the object and gets the next object in
 *	the named realm (or local if no name is given). The object handling is
 *	done by the systems object sub-systems.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	"intel.h"

void	gi_forall_code(CLAUSE* clause,LEVEL_LNK* stack)
{
	int	count= 0,label1,label2,value,stack_size=0;
	char	opStr[50];
	INTEL_OPERAND	op,op1;
	LIST_ENTRY	*list_entry;
	extern int	current_label;
	extern int	initial_size;
	extern FILE	*object_file;

	list_entry = clause->c_forall.lookup->list;

	label1 = ++current_label;
	label2 = ++current_label;

	while(list_entry != NULL)
	{
		/* push the name on the stack */
		fprintf(object_file,"\tpush\tdword %d\n",((CONSTANT*)list_entry->data.lookup.name)->data.string);

		/* push the value */
		op = gi_stat_code((CLAUSE*)list_entry->data.lookup.clause,stack,0,0);
		gi_operand(&op,opStr);
		fprintf(object_file,"\tpush\tdword %s\n",opStr);

		stack_size += 8;

		list_entry = list_entry->next;
		count++;
	};

	/* need to get the object parameter */
	op = gi_stat_code(clause->c_forall.ref_var,stack,0,0);
	gi_operand(&op,opStr);
	fprintf(object_file,"\tpush\tdword %s\n",opStr);	/* NEED TO PUSH THE REFERENCE of this value */

	/* now lets do the function call */
	fprintf(object_file,"\tcall\tfind_object:dword 0\n");
	fprintf(object_file,"\tadd\tesp,%d\n",count*4);

	/* for the last paramter */
	stack_size += 4;

	if (stack_size > stack->count)
		stack->count = stack_size;

	/* loop to here */
	fprintf(object_file,"label_%d:\n",label1);

	/* need to test instance number to see if any objects was found */
	fprintf(object_file,"\ttest\tdword %s,0\n",opStr);
	fprintf(object_file,"\tjne\tlabel_%d\n",label2);

	/* now we need to generate the code inside the loop */
	gi_stat_code(clause->c_forall.action,stack,0,0);

	/* now call find next */
	fprintf(object_file,"\tpush\tdword %s\n",opStr);
	fprintf(object_file,"\tcall\tfind_next_object:dword 0\n");
	fprintf(object_file,"\tadd\tdword esp,4\n");

	/* now loop back to the check */
	fprintf(object_file,"\tjmp\tlabel_%d\n",label1);
	fprintf(object_file,"label_%d:\n",label2);
}
