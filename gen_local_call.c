#include	<stdio.h>
#include	<stdlib.h>
#include	"cople.h"
#include	"codetree.h"

void	gen_local_call(CLAUSE* call_clause,LEVEL_LNK* stack,int regno,int addrno)
{
	short int	size;
	int	count = 0,num_params,spare;
	int	kount,par_count=0;
	char	*left_op;
	char	*name, *service_name;
	CLAUSE	*service_clause,*parameter;
	SYMBOL	*service;
	OPERAND	op;
	REF_LIST	*list;
	LIST_ENTRY	*walk;
	extern FILE	*object_file;
	extern char*	initial_space;


	name = (char*) calloc(32,sizeof(char));	/*max cople name 32 chars */
	left_op = calloc(10,sizeof(char));	

	list = (REF_LIST*) calloc(1,sizeof(REF_LIST));
	
	/* need to rebuild the unix style name from the cobos
	 * style name. Which is stored inside the CONSTANT* space.
	 */
	service_name = conv_cobos_to_unix_str(initial_space + call_clause->c_call.service->data.string);

	/* lets find that service - its name will be in the local
	 * list for the service or in the block above that holds
	 * all the services.
	 */
	service_clause = find_operand(service_name,d_service);

	if (service_clause->c_type == ct_null)
		printf("ERROR: cant fixup local call to service (%s)\n",service_name);
	else{
		service        = service_clause->c_variable.value.data.symbol;
		service_clause = service->block;
		num_params     = service_clause->c_service.num_locals;
		walk = call_clause->c_call.list->list;

		/* if there are parameters lets load the space that they
		 * are to be copied into.
		 */
		if (walk != NULL){
			fprintf(object_file,"\tmove.d\ta%d,a30\n",addrno);
			fprintf(object_file,"\tadd.d\ta%d,#param_space\n",addrno);
		};

		while(walk != NULL)
		{
			if (walk->data.argument.name != NULL)
			{
				/* get the name from local space */
				name = conv_cobos_to_unix_str(initial_space + ((CONSTANT*)walk->data.argument.name)->data.string);

				/* get the fixup for it in the parameter space */
				if ((parameter = find_operand(name,walk->data.argument.type)) == NULL)
					printf("ERROR: parameter(%s) not found in service (%s)\n",name,service_name);
				else{
					if (parameter->c_variable.value.data.symbol->type != walk->data.argument.type)
						printf("ERROR: parameter type mismatch - parameter %s.\n",name);
					else {
						op = gen_stat_code((CLAUSE*)walk->data.argument.value,stack,regno,addrno+1);
						left_op = get_op_string(op,addrno+1,&spare,left_op);
						fprintf(object_file,"\tmove.%c\ta%d(#%d),%s\n",
							INSTR_SIZE[walk->data.argument.type],
							addrno,
							parameter->c_variable.value.data.symbol->offset,
							left_op);
					
						/* need to hold the values that are references for later fix-ups */
						if (parameter->c_variable.value.data.symbol->access_type & a_reference)
						{
							if (  (((CLAUSE*)walk->data.argument.value)->c_variable.value.type != c_reference)
							   && (((CLAUSE*)walk->data.argument.value)->c_variable.value.type != c_addr))
								printf("ERROR: parameter must be a reference - parameter %s. \n",name);
							
							list[count].offset = parameter->c_variable.value.data.symbol->offset;
							list[count].op     = walk;
							count++;
						};
					};
				};

				/* keep code tidy */
				free(name);
			} else {
				/* if no name then just count on */
				if (par_count >= num_params)
					printf("ERROR: too many paramters passed to service %s.\n",service_name);
				else{
					/* must type check entry count to parameter counts type */
					if (walk->data.argument.type != service_clause->c_service.locals[par_count]->type)
						printf("ERROR: parameter type mismatch -  parameter %d.\n",par_count);
					else{
						/* generate the parameter pass */	
						op = gen_stat_code((CLAUSE*)walk->data.argument.value,stack,regno,addrno+1);
						left_op = get_op_string(op,addrno+1,&spare,left_op);
						fprintf(object_file,"\tmove.%c\ta%d(#%d),%s\n",
							INSTR_SIZE[walk->data.argument.type],
							addrno,
							service_clause->c_service.locals[par_count]->offset,
							left_op);
					
						/* need to hold the values that are references for later fix-ups */
						if (service_clause->c_service.locals[par_count]->access_type & a_reference)
						{
							if (  (((CLAUSE*)walk->data.argument.value)->c_variable.value.type != c_reference)
							   && (((CLAUSE*)walk->data.argument.value)->c_variable.value.type != c_addr))
								printf("ERROR: parameter must be a reference - parameter %d. \n",par_count);
						
							list[count].offset = service_clause->c_service.locals[par_count]->offset;
							list[count].op     = walk;
							count++;
						};

						par_count++;
					};
				};
			};

			walk = walk->next;
		};
	
		/* do the near call */
		fprintf(object_file,"\tpush.d\ta31\n");
		fprintf(object_file,"\tcall.d\t%s\n",service_name);
		fprintf(object_file,"\tpop.d\ta31\n");

		/* move the returned values to the variables that are the
		 * refreneces.
		 */
		for (kount=0; kount < count;kount++)
		{
			op = gen_stat_code(list[kount].op->data.argument.value,stack,regno,addrno+1);
			left_op = get_op_string(op,addrno+1,&spare,left_op);
			
			fprintf(object_file,"\tmove.%c\t%s,a%d(#%d)\n",
				INSTR_SIZE[list[kount].op->data.argument.type],
				left_op,
				addrno,
				list[kount].offset);
		};
	};

	free(service_name);
}

