/* $Revision: 1.2 $
 *
 *		  Convert integer to bigendian
 *
 *	This function makes sure that the size of an integer is 4 bytes
 *	(long int) is by standard 4 bytes but int is variable. It also
 *	will make sure that the integer is stored high byte first. On
 *	intel platforms this is not normally the case.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

int	conv_int_to_bigendian(char *data,long int value)
{
	data[3] = (char) (value & 0xFF);
	data[2] = (char) (value >> 8);
	data[1] = (char) (value >> 16);
	data[0] = (char) (value >> 24);
}
