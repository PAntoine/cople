/* $Revision: 1.2 $
 *
 *		        XXX service level
 *
 *	These two functions will enter and exit a passer service level. The service
 *	level does not much but hold the service stuff together. There will be a
 *	code block on the inside of the service. This will hold all the locals and
 *	means none of the well tested code needs to be duplicated or altered.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	"codetree.h"
#include	"cople.h"
#include	<malloc.h>

void	new_service_entry(SYMBOL *service_name,PARAMETER_LIST *parameter_list)
{
	int	count,data_size = 0;
	SYMBOL	*symbol;
	CLAUSE	*new_service;
	extern CLAUSE	*current_block;

	new_service = (CLAUSE*) calloc(1,sizeof(CLAUSE));

	new_service->c_service.type  = ct_service;
	new_service->c_service.owner = current_block;
	new_service->c_service.name  = service_name;

	service_name->block = new_service;	/* used to find the service */

	current_block = new_service;
	
	if (parameter_list != NULL)
	{
		for(count=0;count < parameter_list->list_size;count++)
		{
			symbol = add_symbol(parameter_list->list[count].name);
		
			if (symbol != NULL)
			{
				symbol->type        = parameter_list->list[count].type;
				symbol->offset      = data_size;
				symbol->size        = DATA_TYPE_SIZE[parameter_list->list[count].type];
				symbol->access_type = a_param;
				symbol->block       = current_block;

				if (parameter_list->list[count].def != NULL)
				{
					symbol->access_type |= a_const;

					if (parameter_list->list[count].def->type == c_char)
						symbol->data = (int) parameter_list->list[count].def->data.character;
					else 
						symbol->data = parameter_list->list[count].def->data.integer;
				}else
					symbol->data = 0;

				if (parameter_list->list[count].a_ref)
					symbol->access_type |= a_reference;

				data_size += DATA_TYPE_SIZE[parameter_list->list[count].type];
			};
		};
	
		/* free the list */
		free(parameter_list->list);
		free(parameter_list);
	};
}

CLAUSE*	end_service_level(CLAUSE* code)
{
	CLAUSE	*old_hold;
	extern CLAUSE	*current_block;

	old_hold = current_block;
	current_block->c_service.code = code;
	current_block = current_block->c_service.owner;

	return old_hold;
}
	
