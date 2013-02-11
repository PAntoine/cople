/* $Revision: 1.2 $
 *		COBOS STRING FUNCTIONS
 *
 *	This file holds the functions that are needed to manipulate the cobos type
 *	strings. This includes converting from unix (C) style strings that are 
 *	zero ended, to the size up front style used within COBOS and of course 
 *	COPLE.
 *
 *		(c) 1998-99 Peter Antoine.
 *
 *	Note: COBOS string format is:
 *
 *		[size]{string}
 *
 *	size is 16 bits in length.
 *
 */

#include	<malloc.h>
#include	<stdio.h>

void	conv_unix_to_cobos_str(char* destination, char *source, int max_length)
{
	int	size,count;


	/* make space for the string */
	
	size = strlen(source);
	
	if(size > max_length)
		size = max_length + 2;
	else
		size += 2;
	
	destination = malloc(size);
	
	/* set the string size */
	
	destination[0] = size >> 8;
	destination[1] = size & 0x00FF;

	/* now copy the string */

	memcpy(destination+2,source,size-2);
}

void	copy_cobos_str(char *destination, char *source)
{
	int	size;

	/* avoid casting confusion */
	size  = destination[0];
	size *= 256;
	size += destination[1];

	destination = malloc(size+2);

	memcpy(destination,source,size+2);
}

char*	conv_cobos_to_unix_str(char* cobos_string)
{
	short int	size;
	char	*string;

	/* first two bytes are the size (in bigendian format) */
	size  = cobos_string[0];
	size *= 256;
	size += cobos_string[1];

	/* now for some loverly memory */
	string = calloc(size+1,sizeof(char));
	
	/* now do the copy */
	memcpy(string,cobos_string+2,size);

	return string;
}
	
