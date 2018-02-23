/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print nbr
*/

#include "objdump.h"

void my_revstr(char *str)
{
	char tmp[10];
	int i;
	size_t count;

	strcpy(tmp, str);
	i = strlen(str) - 1;
	count = 0;
	while (i != -1) {
		str[count] = tmp[i];
		i--;
		count++;
	}
}

void print_nbr_end(char *str)
{
	my_revstr(str);
	write(1, " ", 1);
	write(1, str, strlen(str));
	write(1, " ", 1);
	free(str);
}

void print_nbr(size_t nb)
{
	char *str;
	size_t i;

	str = calloc(10, sizeof(char));
	i = 0;
	while (nb != 0) {
		if (nb % 16 > 9)
			str[i] = 'a' + ((nb % 16) - 10);
		else
			str[i] = '0' + (nb % 16);
		nb = ((nb - (nb % 16)) / 16);
		i++;
	}
	if (strlen(str) < 4)
		for (size_t x = strlen(str); x != 4; x++)
			str[x] = '0';
	print_nbr_end(str);
}

void print_nbr_addr(char *str)
{
	my_revstr(str);
	write(1, "0x", 2);
	write(1, str, strlen(str));
	free(str);
}

void print_nbr_fill(size_t nb, size_t until)
{
	char *str;
	size_t i;

	str = calloc(10, sizeof(char));
	i = 0;
	while (nb != 0) {
		if (nb % 16 > 9)
			str[i] = 'a' + ((nb % 16) - 10);
		else
			str[i] = '0' + (nb % 16);
		nb = ((nb - (nb % 16)) / 16);
		i++;
	}
	if (strlen(str) < until)
		for (size_t x = strlen(str); x != until; x++)
			str[x] = '0';
	print_nbr_addr(str);
}
