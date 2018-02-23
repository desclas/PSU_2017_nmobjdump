/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** objdump
*/

#include "objdump.h"

void start_print(char *name, void *buf, int *nb)
{
	Elf64_Ehdr *elf = buf;

	if (strlen(name) > 2 && strncmp(&name[strlen(name) - 2], ".a", 2)
		!= 0) {
		if (is_elf(elf) == 0)
			start_reading32(buf, nb, name);
		else if (is_elf(elf) == 1)
			start_reading64(buf, nb, name);
		else
			print_error(3, "objdump: ", name,
				": File format not recognized\n");
	}
}

int main(int ac, char **av)
{
	int fd;
	void *buf;
	struct stat s;
	int nb = 0;

	for (int i = 1; i < 2 || i < ac; i++) {
		fd = get_fd(ac, av, i);
		if (fd != -1) {
			fstat(fd, &s);
			buf = mmap(NULL, s.st_size, PROT_READ,
				MAP_PRIVATE, fd, 0);
			if (buf != (void *)-1) {
				start_print(av[i], buf, &nb);
				munmap(buf, s.st_size);
			}
		close(fd);
		} else
			print_error(3, "objdump: << ", av[i],
				" >>: no such file\n");
	}
}