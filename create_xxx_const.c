/* $Revision: 1.2 $
 *		CREATE_xxx_CONST
 *		CREATE_SYM_REF
 *		FREE_CONST
 *
 *	This function will create a constant this includes
 *	allocating the memory space needed to hold the data.
 *	This function will do all the type conversions that]
 *	are nessary from the data recivied from the lex function
 *	to the internal represetation for the COPLE langauge.
 *
 *		P.Antoine.
 *
 *	In create_sym_ref if a name is not found then it will
 *	return a null, this is OK, as a null is later read as a
 *	symbol not found.
 *
 *	In create string, inst, lookup the string data is placed
 *	in the initial_space (common) area. And the constant will
 *	then reference this data.
 */

#include	<malloc.h>
#include	<math.h>
#include	"cople.h"
#include	"codetree.h"

void	free_const(CONSTANT* data)
{
	free(data);
}
	
CONSTANT*	create_int_const(int data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_int;
	result->data.integer = data;

	return result;
}

CONSTANT*	create_ref_const(int data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_memory;
	result->data.integer = data;
	
	return result;
}


CONSTANT*	create_char_const(char data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_char;
	result->data.character = data;

	return result;
}

CONSTANT*	create_float_const(float data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_real;
	result->data.flt_point = data;

	return result;
}

CONSTANT*	create_bool_const(int data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_boolean;
	result->data.boolean = data;

	return result;
}

CONSTANT*	create_str_const(char* data)
{
	CONSTANT	*result;
	int	count;
	extern int	initial_size;
	extern char	*initial_space;

	result = calloc(1,sizeof(CONSTANT));
	count  = strlen(data);

	result->type = c_string;
	
	initial_space = realloc(initial_space,initial_size+count+2);
	(initial_space+initial_size)[0] = (char) count >> 8;
	(initial_space+initial_size)[1] = (char) count & 0x00FF;
	memcpy((initial_space+initial_size+2),data,count);
	
	result->data.string = initial_size;

	initial_size += count+2;

	return result;
}

CONSTANT*	create_inst_const(char* realm, char* name, int instance)
{
	CONSTANT	*result;
	int	count;
	long int	realm_off = 0,object;
	extern int	initial_size;
	extern char	*initial_space;

	result = calloc(1,sizeof(CONSTANT));
	result->type = c_instance;
	
	if (realm != NULL){	/* NULL realm - local object */
		count  = strlen(realm);
	
		initial_space = realloc(initial_space,initial_size+count+2);
		(initial_space+initial_size)[0] = (char) count >> 8;
		(initial_space+initial_size)[1] = (char) count & 0x00FF;
		memcpy((initial_space+initial_size+2),realm,count);
	
		realm_off = initial_size;
		initial_size += count+2;
	}

	count = strlen(name);
	initial_space = realloc(initial_space,initial_size+count+2);
	(initial_space+initial_size)[0] = (char) count >> 8;
	(initial_space+initial_size)[1] = (char) count & 0x00FF;
	memcpy((initial_space+initial_size+2),name,count);
		
	object = initial_size;
	initial_size += count+2;

	instance = instance;

	/* now need to create the reference pointer */

	initial_space = realloc(initial_space,initial_size+12);
	conv_int_to_bigendian(initial_space+initial_size,realm_off);
	conv_int_to_bigendian(initial_space+initial_size+4,object);
	conv_int_to_bigendian(initial_space+initial_size+8,(long int)instance);

	if (realm != NULL)	/* dont fixup if not realm - must be zero */
		add_to_fixup(initial_size);
	add_to_fixup(initial_size+4);
	
	/* tidy and exit */	
	result->data.integer = initial_size;
	initial_size += 12;
	
	return result;
}

CONSTANT*	create_list_const(LIST *data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_list;
	result->data.list = data;

	return result;
}

CONSTANT*	create_expr_const(CLAUSE *data)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_expr;
	result->data.expr = data;

	return result;
}

CONSTANT*	create_arg_const(DATA_TYPE type,CONSTANT *name,CLAUSE* clause)
{
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_arg;
	result->data.argument.type  = type;
	result->data.argument.name  = (void*) name;
	result->data.argument.value = (CLAUSE*) clause;

	return result;
}

CONSTANT*	create_lookup_const(CONSTANT* name, CLAUSE* clause)
{
	int	count;
	CONSTANT	*result;

	result = calloc(1,sizeof(CONSTANT));

	result->type = c_lookup;
	result->data.lookup.clause = (CLAUSE*) clause;
	result->data.lookup.name   = (CONSTANT*) name;

	return result;
}
CONSTANT*	create_sym_ref(char* name)
{
	CONSTANT		*result;
	SYMBOL		*found;

	result = calloc(1,sizeof(CONSTANT));
	found  = find_variable(name);

	result->type = c_addr;
	result->data.symbol = found;

	return result;
}

