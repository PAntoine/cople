/* $Revision: 1.2 $
 *
 *		   ADD_VARIABLE
 * 
 *	This function will add a variable to the local variable
 *	list, and to the symbol table. If the variable has an 
 *	inital value this will also be stored in the symbol table.
 *
 *		(c) 1998 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"

SYMBOL*	add_variable(char *sym_name,DATA_TYPE type,CONSTANT *data, int a_type)
{
	int	size;
	char	*string;
	SYMBOL	*new_symbol;
	long int	*put_here;
	extern int	level_at;
	extern int	current_memory;
	extern int	initial_size;
	extern char	*initial_space;
	
	/* First create the symbol entry, and check to see if the
	 * symbol is valid to be created here.
	 */

	new_symbol = add_symbol(sym_name);

	if (new_symbol != NULL){
		new_symbol->type        = type;
		new_symbol->size        = DATA_TYPE_SIZE[type];
		new_symbol->num_dim     = 0;
		new_symbol->dimensions  = NULL;

		/* initailised data goes at the front of memory and is static
		 * but other variables location can be used by more than one
		 * variable. The offset in variables is finally calc'ed by using
		 * initial_size + offset. Where initialised is just offset.
		 */
		if ( data == NULL)
		{
			new_symbol->data        = 0;
			new_symbol->access_type = (a_var | a_type);
			
			/* memory refs are all the same size */
			if (a_type & a_ptr)
				size = DATA_TYPE_SIZE[c_memory];
			else
				size = DATA_TYPE_SIZE[type];

			/* Is the variable GLOBAL that is at the first level of the
			 * function - if so then it will need space in the initial_space.
			 */
		
			if (level_at == 1)
			{
				new_symbol->offset = initial_size;
				
				initial_space = realloc(initial_space,initial_size + size);
				memset(initial_space+initial_size,'\0',size);
				initial_size += size;
				new_symbol->access_type |= a_global;
			}else{
				new_symbol->offset = current_memory;
				current_memory += size;
			};
		} else if (data->type == c_memory || data->type == c_reference)
		{
			/* for memory reference variables they need to have the
			 * initial value stored in the initial space + this location
			 * needs to be added to the fix-up list. The rest of the 
			 * variable is treated as normal. All memory references are
			 * the same size.
			 */

			size = DATA_TYPE_SIZE[type];
			initial_space = realloc(initial_space,initial_size + size);
			
			if (data->type == c_memory)
				conv_int_to_bigendian(initial_space+initial_size,data->data.integer);
			else
				conv_int_to_bigendian(initial_space+initial_size,data->data.symbol->offset);

			add_to_fixup(initial_size);
			
			new_symbol->data        = initial_size;
			new_symbol->access_type = (a_const | a_type);
		
			/* only update current memory if not global */
			if (level_at != 1)
			{
				new_symbol->offset = current_memory;
				current_memory    += size;
			}else{
				new_symbol->offset       = initial_size;
				new_symbol->access_type |= a_global;
			};

			initial_size   += size;
		}else{
			/* simple initialised variables - data inside the initial
			 * space and the offset in the local space.
			 */

			switch(type)
			{
			case d_string:	size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				put_here  = (long int*) (initial_space+initial_size);
				*put_here = (long int) data->data.string;
				add_to_fixup(initial_size);
				break;
			
			case d_instance:
				size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				put_here = (long int*) (initial_space+initial_size);
				*put_here = (long int) data->data.integer;
				add_to_fixup(initial_size);
				break;
		
			case d_int:	size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				conv_int_to_bigendian(initial_space+initial_size,data->data.integer);
				break;

			case d_char:	size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				memcpy(initial_space+initial_size,&data->data.character,size);
				break;

			case d_boolean:	size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				memcpy(initial_space+initial_size,&data->data.boolean,size);
				break;
		
			case d_real:	size = DATA_TYPE_SIZE[type];
				initial_space = realloc(initial_space,initial_size + size);
				memcpy(initial_space+initial_size,&data->data.flt_point,size);
				break;
		
			default:	yyerror("WARNING: using a variable type not implemented as constants (sorry!)");
			};

			new_symbol->data        = initial_size;
			new_symbol->access_type = (a_const | a_type);
		
			/* only update current memory if not global */
			if (level_at != 1)
			{
				new_symbol->offset = current_memory;
				current_memory    += size;
			}else{
				new_symbol->offset       = initial_size;
				new_symbol->access_type |= a_global;
			};

			initial_size   += size;
		};
	} else {
		yyerror("ERROR: invalid symbol or out of memory. \n");
	};
	
	return new_symbol;
}

