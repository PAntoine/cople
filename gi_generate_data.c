/* $Revision: 1.2 $	(generate intel)
 *
 *		Generate Data
 *	
 *	This function will generate the intial data from the inital space data. The
 *	code is almost identical to gen_the_dbs (sorry!).
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<stdio.h>

void	gi_generate_data(void)
{
	int	count=0,kount,pos,look_ahead,contig;
	char	left,right,start;
	char	hex[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	char	line[105];
	extern int	initial_size;
	extern int	max_save_space;
	extern char	*initial_space;
	extern FILE	*object_file;


	/* exception table first */
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");
	fprintf(object_file,"\tdd\twhen_label_0,when_label_0,when_label_0,when_label_0\n");

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
				pos  = kount*5;
				left  = (int) ((((char) 0xF0 & initial_space[kount+count]) >> 4) & 0x0F);
				right = (int) (((char) 0x0F & initial_space[kount+count]) & 0x0F);
			
				line[pos]   = '$';
				line[pos+1] = '0';
				line[pos+2] = hex[left];
				line[pos+3] = hex[right];
				line[pos+4] = ',';
			}

			line[pos+4] = '\n';
			line[pos+5] = '\0';
	
			fprintf(object_file,"\tdb\t%s",line);
			count += kount;
		}else{
			left  = (int) ((((char) 0xF0 & start) >> 4) & 0x0F);
			right = (int) (((char) 0x0F & start) & 0x0F);
		
			fprintf(object_file,"\ttimes %d db $0%c%c\n",
				contig,
				hex[left],
				hex[right]);
			
			count += contig;
		};
				
	};

	if (max_save_space != 0)
		fprintf(object_file,"save_space:\rtimes %d dd $00\n",max_save_space);
}
	

