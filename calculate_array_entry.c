/* $Revision: 1.2 $
 *
 *		      CALCULATE_ARRAY_ENTRY
 *
 *	This function will take an argument list and generate a clause
 *	that can be used to calculate the entry in the array. It will
 *	genrate a INT constant clause for an interger array. Or, it
 *	will create a code tree that will generate the code for the 
 *	elements of the list that are variable references.
 *
 *	To make the run-time more efficient any numberical entries into
 *	the array are precalculated and are added last.
 *	
 *		Copyright (c) 1999 Peter Antoine.
 *
 *
 */

#include	<memory.h>
#include	"codetree.h"
#include	"cople.h"

CLAUSE*	calculate_array_entry(SYMBOL *symbol, LIST *arguments)
{
	int	*product;
	int	kount,count;
	CLAUSE	*last,*n_multi,*tail,*prod,*temp;
	CONSTANT	*tree_tail;
	LIST_ENTRY	*walk;

	/* lets first calculate the dimensional offsets, which are
	 * to be multiplied by each entry.
	 */

	product = (int*) calloc(symbol->num_dim,sizeof(int));
	for (count=0;count<symbol->num_dim;count++)
		product[count] = DATA_TYPE_SIZE[symbol->array_type];

	if (symbol->num_dim > 1)
		for(count = 1; count < symbol->num_dim; count++)
			for(kount=0; kount<count; kount++)
				product[kount] *= (int) symbol->dimensions[count];

	/* now calc the tree. Need a start point for the array calculation
	 * and this will be the last element in the list as this is to be
	 * added to the array calulation. This will be the total tree if the
	 * tree is only made up of integer constants.
	 */

	tree_tail = (CONSTANT*) calloc(1,sizeof(CONSTANT));
	tree_tail->type = c_int;
	tree_tail->data.integer = 0;

	last = new_const_clause(tree_tail);
	tail = last;
	free(tree_tail);

	/* now lets generate the code tree for the calculation of the
	 * array reference.
	 */

	walk = arguments->list;
	
	for( count = 0; count < symbol->num_dim && walk != NULL ; count++)
	{
		/* if integer dont waste run-time calculating the reference might
		 * as well do it here in the compiler. But, for other types of
		 * clause then they must be calculated at run-time.
		 */
		if (((CLAUSE*)walk->data.expr)->c_variable.value.type == c_int)
			tail->c_variable.value.data.integer += (((CLAUSE*)walk->data.expr)->c_variable.value.data.integer*product[count]);
		else {
			/* build a cluase to hold the product for the dimemsion */
			prod = (CLAUSE*) calloc(1,sizeof(CLAUSE));
			prod->c_type = ct_variable;
			prod->c_variable.value.type = c_int;
			prod->c_variable.value.data.integer = product[count];
				
			/* lets copy the data away from the list */
			temp = (CLAUSE*) calloc(1,sizeof(CLAUSE));
			memcpy(temp,(CLAUSE*)walk->data.expr,sizeof(CLAUSE));
			
			if(count != symbol->num_dim-1)
			{
				n_multi = new_binary_clause(multi,temp,prod);
				last    = new_binary_clause(add,n_multi,last);
			} else{
				free(prod);	/* prod n-1 = 1 */
				last  = new_binary_clause(add,temp,last);
			};
		};

		walk = walk->next;
	};

	free(product);
	return last;
}
				
	
