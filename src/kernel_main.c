#include <stdint.h>
#include "terminal.h"
#include "ramfs.h"

#define MULTIBOOT2_HEADER_MAGIC 0xe85250d6

/* This header lets GRUB recognize the kernel as multiboot2. */
const unsigned int multiboot_header[] __attribute__((section(".multiboot"))) = {
    MULTIBOOT2_HEADER_MAGIC,
    0,
    24,
    -(MULTIBOOT2_HEADER_MAGIC + 24),
    0,
    8
};

void main(void)
{
    int first_result;
    int second_result;
    int write_result;

    puts("COMP 310 project booted successfully.\n");
    puts("Terminal output layer is working.\n");

    ramfs_init();
    puts("RAMFS initialized.\n");

    first_result = ramfs_create("notes.txt");

    if (first_result == 0)
    {
        puts("First create succeeded: notes.txt\n");
    }
    else
    {
        puts("First create failed: notes.txt\n");
    }

    second_result = ramfs_create("notes.txt");

    if (second_result == -1)
    {
        puts("Second create correctly failed because file already exists.\n");
    }
    else
    {
        puts("Second create did not fail as expected.\n");
    }

    write_result = ramfs_write("notes.txt", "hello from ramfs");

    if (write_result == 0)
    {
        puts("Write succeeded for notes.txt\n");
    }
    else
    {
        puts("Write failed for notes.txt\n");
    }

    while (1)
    {
    }
}