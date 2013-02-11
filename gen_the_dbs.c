/* $Revision: 1.2 $
 *
 *		     Generate the Data Byte's
 *
 *	This function will convert the data_space char array into the
 *	db.b data list for the program. This function will do a big
 *	endian conversion. So, the high byte of all data bytes go
 *	first.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<stdio.h>

void	gen_the_dbs(void)
{
	int	count=0,kount,pos,look_ahead,contig;
	char	left,right,start;
	char	hex[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	char	line[85];
	extern int	initial_size;
	extern char	*initial_space;
	extern FILE	*object_file;


	/* exception table first */
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");
	fprintf(object_file,"\tdb.d\t#when_label_0,#when_label_0,#when_label_0,#when_label_0\n");

	while(count<initial_size)
	{
		start = initial_space[count];
		look_ahead = count+1;
		contig = 1;
		
		while(start == initial_space[look_ahead] && look_ahead < initial_size)
		{
			contig++;
			look_ahead++;
		};
			
		if (contig < 5)
		{
			for(kount=0;kount<20 && (kount+count)<initial_size;kount++)
			{
				pos  = kount*4;
				left  = (int) ((((char) 0xF0 & initial_space[kount+count]) >> 4) & 0x0F);
				right = (int) (((char) 0x0F & initial_space[kount+count]) & 0x0F);
			
				line[pos]   = '$';
				line[pos+1] = hex[left];
				line[pos+2] = hex[right];
				line[pos+3] = ',';
			}

			line[pos+3] = '\n';
			line[pos+4] = '\0';
	
			fprintf(object_file,"\tdb.b\t%s",line);
			count += kount;
		}else{
			left  = (int) ((((char) 0xF0 & start) >> 4) & 0x0F);
			right = (int) (((char) 0x0F & start) & 0x0F);
		
			fprintf(object_file,"\tdb.b\t$%c%c times %d\n",
				hex[left],
				hex[right],
				contig);
			
			count += contig;
		};
				
	};
}
	

