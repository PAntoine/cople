/* $Revision: 1.2 $
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
#include	"codetree.h"
#include	"cople.h"

void	gen_forall_code(CLAUSE* clause,LEVEL_LNK* stack,int regno,int addrno)
{
	int	count= 0;
	OPERAND	op,op1,label1,label2;
	LIST_ENTRY	*list_entry;
	extern int	initial_size;
	extern FILE	*object_file;

	list_entry = clause->c_forall.lookup->list;

	while(list_entry != NULL)
	{
		/* push the name on the stack */
		fprintf(object_file,"\tmove.d\td%d,#%d\n",regno,((CONSTANT*)list_entry->data.lookup.name)->data.string);
		fprintf(object_file,"\tadd.d\td%d,a30\n",regno);
		fprintf(object_file,"\tpush.d\td%d\n");

		/* push the value */
		op = gen_stat_code((CLAUSE*)list_entry->data.lookup.clause,stack,regno,addrno);
		gen_instr(push,op,op1,addrno);

		list_entry = list_entry->next;
		count++;
	};

	/* need to get the object parameter */
	if (clause->c_forall.ref_var->c_variable.value.type == c_reference) 
	{
		if (clause->c_forall.ref_var->c_variable.value.data.symbol->access_type & a_global)
			op.offset = clause->c_forall.ref_var->c_variable.value.data.symbol->offset;
		else
			op.offset = initial_size + clause->c_forall.ref_var->c_variable.value.data.symbol->offset;
	
		fprintf(object_file,"\tpush.d\ta30(#%d)\n",op.offset);
	}else{
		fprintf(object_file,"\tmove.d\ta%d,#%d\n",addrno, clause->c_forall.ref_var->c_variable.value.data.integer);
		fprintf(object_file,"\tadd.d\ta%d,a30\n",addrno);
		fprintf(object_file,"\tpush.d\ta%d\n",addrno);	/* reference to the instance parameter (needs to write-back instance no. */
	};
	

	/* now lets do the function call */
	fprintf(object_file,"\tcall.d\ta29(#find_object)\n");
	fprintf(object_file,"\tadd.d\tsp,#%d\n",4+count*4);

	/* loop to here */
	label1 = get_label();
	put_label(label1);
	gen_labels();
	label2 = get_label();

	/* need to test instance number to see if any objects was found */
	fprintf(object_file,"\ttest.d\ta%d(#8),#0\n",addrno);
	gen_instr(branch_eq,label2,op1,addrno);

	/* now we need to generate the code inside the loop */
	gen_stat_code(clause->c_forall.action,stack,regno,addrno+1);

	/* now call find next */
	fprintf(object_file,"\tpush.d\ta%d\n",addrno);
	fprintf(object_file,"\tcall.d\ta29(#find_next_object)\n");
	fprintf(object_file,"\tadd.d\tsp,#4\n");

	/* now loop back to the check */
	gen_instr(branch,label1,op1,addrno);
	put_label(label2);
}
