#include <fcntl.h>
#include <stdio.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void my_revstr(char *str)
{
	char tmp[10];
	size_t i;
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
	char tab[100][100] = { ".bss", ".strtab", ".symtab", ".shstrtab",
	".rela.text", ".data", ".note.GNU-stack", ".rela.eh_frame", " " };

	for (size_t i = 0; tab[i][0] != ' '; i++)
		if (strcmp(tab[i], str) == 0)
			return (0);
	return (1);
}

void followSec(void *buf, char *tab, Elf64_Shdr *sec, size_t nb)
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

void followHed(void *buff, Elf64_Phdr *hed, size_t nb)
{
	char tab[100][100] = {"SHT_NULL", "SHT_PROGBITS", "SHT_SYMTAB", "SHT_STRTAB", "SHT_RELA", "SHT_HASH", "SHT_DYNAMIC", "SHT_NOTE", "SHT_NOBITS", "SHT_REL", "SHT_SHLIB", "SHT_DYNSYM", "SHT_LOPROC", "SHT_HIPROC", "SHT_LOUSER", "SHT_HIUSER", " "};
	long unsigned int nbb[100] = {SHT_NULL, SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_RELA, SHT_HASH, SHT_DYNAMIC, SHT_NOTE, SHT_NOBITS, SHT_REL, SHT_SHLIB, SHT_DYNSYM, SHT_LOPROC, SHT_HIPROC, SHT_LOUSER, SHT_HIUSER, 0};

	for (size_t x = 0; tab[x][0] != ' '; x++)
		printf("name = %s = %lu\n", tab[x], nbb[x]);

	for (size_t i = 0; i != nb; i++) {
		printf("vaddr = %lu\n", hed[i].p_vaddr);
		printf("off = %lu\n", hed[i].p_offset);
		printf("addr = %p\n", hed[i].p_paddr);
		printf("vaddr = %p\n", hed[i].p_vaddr);
		if (hed[i].p_offset != 0) {
			printf("file = %lu\n", hed[i].p_filesz);
			printf("mem = %lu\n", hed[i].p_memsz);
		}
	}
}

int getFd(int ac, char **av, int i)
{
	if (ac == 1)
		return (open("a.out", O_RDONLY));
	else
		return (open(av[i], O_RDONLY));
}

void startReading(void *buf, int *nb)
{
	Elf64_Ehdr *elf;
	Elf64_Shdr *sec;
	char *tmp;

	if (*nb != 0)
		write(1, "\n", 1);
	*nb += 1;
	elf = buf;
	if (elf->e_shoff != 0) {
		sec = buf + (size_t)elf->e_shoff + (elf->e_shstrndx * sizeof(Elf64_Shdr));
		tmp = buf + sec->sh_offset;
		sec = buf + (size_t)elf->e_shoff;
		followSec(buf, tmp, sec, elf->e_shnum);
	} else
		followHed(buf, buf +elf->e_phoff, elf->e_phnum);
}

void printError(int nb, ...)
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

void startPrint(char *name, void *buf, int *nb)
{
	Elf64_Ehdr *elf;

	startReading(buf, nb);
}

int main(int ac, char **av)
{
	int fd;
	void *buf;
	struct stat s;
	int nb = 0;

	for (size_t i = 1; i  && i < ac; i++) {
		fd = getFd(ac, av, i);
		if (fd != -1) {
			fstat(fd, &s);
			buf = mmap(NULL, s.st_size, PROT_READ,
				MAP_PRIVATE, fd, 0);
			if (buf != (void *)-1) {
				startPrint(av[i], buf, &nb);
				munmap(buf, s.st_size);
			}
		close(fd);
		} else
			printError(3, "objdump: << ", av[i],
				" >>: no such file\n");
	}
}