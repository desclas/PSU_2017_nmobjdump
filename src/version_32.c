/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** version 32
*/

#include "objdump.h"

void follow_sec32(void *buf, char *tab, Elf32_Shdr *sec, size_t nb)
{
	for (size_t i = 1; i != nb; i++) {
		if (is_pritable(&tab[sec[i].sh_name]) == 1) {
			write(1, "Contents of section ", 20);
			write(1, &tab[sec[i].sh_name],
				strlen(&tab[sec[i].sh_name]));
			write(1, ":\n", 2);
			print_data(sec[i].sh_size, buf + sec[i].sh_offset,
				sec[i].sh_addr);
		}
	}
}

void print_first32(Elf32_Ehdr *elf, char *name)
{
	write(1, name, strlen(name));
	write(1, ":     file format elf", 21);
	write(1, "32\n", 3);
	write(1, "architecture: ", 14);
	print_archi(elf->e_machine);
	print_nbr_fill(elf->e_flags, 8);
	write(1, "\nstart address 0x", 17);
	print_nbr_fill(elf->e_entry, 16);
	write(1, "\n\n", 2);
}

void start_reading32(void *buf, int *nb, char *name)
{
	Elf32_Ehdr *elf;
	Elf32_Shdr *sec;
	char *tmp;

	if (*nb != 0)
		write(1, "\n", 1);
	print_first32(buf, name);
	*nb += 1;
	elf = buf;
	if (elf->e_shoff != 0) {
		sec = buf + (size_t)elf->e_shoff +
			(elf->e_shstrndx * sizeof(Elf32_Shdr));
		tmp = buf + sec->sh_offset;
		sec = buf + (size_t)elf->e_shoff;
		follow_sec32(buf, tmp, sec, elf->e_shnum);
	} else
		follow_hed(buf, buf +elf->e_phoff, elf->e_phnum);
}

