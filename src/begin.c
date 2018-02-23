/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** begin
*/

#include "objdump.h"

void follow_hed(void *buff, Elf64_Phdr *hed, size_t nb)
{
	(void)buff;
	(void)hed;
	(void)nb;
}

int get_fd(int ac, char **av, int i)
{
	if (ac == 1)
		return (open("a.out", O_RDONLY));
	else
		return (open(av[i], O_RDONLY));
}

void print_archi(size_t mach)
{
	size_t nb_tab[] = { EM_NONE, EM_M32, EM_SPARC, EM_386, EM_68K, EM_88K,
	EM_860, EM_MIPS, EM_PARISC, EM_SPARC32PLUS, EM_PCP, EM_PPC64,
	EM_S390, EM_ARM, EM_SH, EM_SPARCV9, EM_IA_64, EM_X86_64, EM_VAX };

	char *str_tab[] = { "unknow", "aM32", "SAPAC", "i386", "m68K", "m88k",
	"i860", "MIPS", "PARISC", "SPARAC", "PPC", "PPC64", "S390", "ARM", "SH",
	"SPARCV9", "IA_64", "ax86-64", "VAX", " " };

	for (size_t i = 0; str_tab[i][0] != ' '; i++)
		if (mach == nb_tab[i])
			write(1, str_tab[i], strlen(str_tab[i]));
	write(1, ", flags ", 8);
}

void print_error(size_t nb, ...)
{
	va_list list;
	char *tmp;

	va_start(list, nb);
	for (size_t i = 0; i != nb; i++) {
		tmp = va_arg(list, char*);
		write(2, tmp, strlen(tmp));
	}
	va_end(list);
}

int is_elf(Elf64_Ehdr *elf)
{
	if (elf->e_ident[EI_MAG0] == ELFMAG0 && elf->e_ident[EI_MAG1] == ELFMAG1
		&& elf->e_ident[EI_MAG2] == ELFMAG2
		&& elf->e_ident[EI_MAG3] == ELFMAG3) {
			if (elf->e_ident[EI_CLASS] == ELFCLASS32)
				return (0);
			else if (elf->e_ident[EI_CLASS] == ELFCLASS64)
				return (1);
			else
				return (84);
		} else
			return (84);
}
