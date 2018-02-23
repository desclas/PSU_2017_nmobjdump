/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** print data
*/

#include "objdump.h"

void print_hex(unsigned char nb)
{
	char tmp[2] = { '0', '0' };
	size_t i;

	i = 1;
	while (nb != 0)
	{
		if (nb % 16 > 9)
			tmp[i] = 'a' + ((nb % 16) - 10);
		else
			tmp[i] = '0' + (nb % 16);
		nb = ((nb - (nb % 16)) / 16);
		i--;
	}
	write(1, tmp, 2);
}

void print_data_str(unsigned char *str, size_t size)
{
	size_t i;

	for (i = 0; i != size; i++)
		if (str[i] >= 32 && str[i] <= 126)
			write(1, &str[i], 1);
		else
			write(1, ".", 1);
	for (; i != 16; i++)
		write(1, " ", 1);
	write(1, "\n", 1);
}

void print_data_hex(unsigned char *str, size_t size)
{
	size_t tmp;

	tmp = 0;
	while (tmp != size) {
		print_hex(str[tmp]);
		tmp++;
		if (tmp % 4 == 0)
			write(1, " ", 1);
	}
	if (tmp % 16 != 0) {
		for (size_t i = (tmp % 16) * 2; i != (16 * 2) + (3 -
				((tmp % 16) / 4)) + 1; i++)
			write(1, " ", 1);
	}
	write(1, " ", 1);
}

void print_data(size_t size, unsigned char *data, size_t addr)
{
	if (size == 0) {
		print_nbr(addr);
		print_data_hex(data, 0);
		print_data_str(data, 0);
	}
	while (size != 0) {
		print_nbr(addr);
		print_data_hex(data, (size > 15) ? 16 : size);
		print_data_str(data, (size > 15) ? 16 : size);
		data += ((size > 15) ? 16 : size);
		if (size <= 15)
			size = 0;
		else
			size -= 16;
		addr += 16;
	}
}

int is_pritable(char *str)
{
	char *tab[] = { ".bss", ".strtab", ".symtab", ".shstrtab",
	".rela.text", ".note.GNU-stack", ".rela.eh_frame", " " };

	for (size_t i = 0; tab[i][0] != ' '; i++)
		if (strcmp(tab[i], str) == 0)
			return (0);
	return (1);
}
