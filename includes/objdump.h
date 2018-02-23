/*
** EPITECH PROJECT, 2018
** nmobjdump
** File description:
** objdump
*/

#ifndef OBJDUMP_H_
# define OBJDUMP_H_

# include <fcntl.h>
# include <stdio.h>
# include <elf.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>

void my_revstr(char *str);
void print_nbr_end(char *str);
void print_nbr(size_t nb);
void print_nbr_addr(char *str);
void print_nbr_fill(size_t nb, size_t until);
void print_hex(unsigned char nb);
void print_data_str(unsigned char *str, size_t size);
void print_data_hex(unsigned char *str, size_t size);
void print_data(size_t size, unsigned char *data, size_t addr);
int is_pritable(char *str);
void follow_sec32(void *buf, char *tab, Elf32_Shdr *sec, size_t nb);
void print_first32(Elf32_Ehdr *elf, char *name);
void start_reading32(void *buf, int *nb, char *name);
void follow_sec64(void *buf, char *tab, Elf64_Shdr *sec, size_t nb);
void print_first64(Elf64_Ehdr *elf, char *name);
void start_reading64(void *buf, int *nb, char *name);
void follow_hed(void *buff, Elf64_Phdr *hed, size_t nb);
int get_fd(int ac, char **av, int i);
void print_archi(size_t mach);
void print_error(size_t nb, ...);
int is_elf(Elf64_Ehdr *elf);

#endif /* !OBJDUMP_H_ */
