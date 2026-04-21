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
    int result;

    puts("COMP 310 project booted successfully.\n");
    puts("Terminal output layer is working.\n");

    ramfs_init();
    puts("RAMFS initialized.\n");

    result = ramfs_create("notes.txt");

    if (result == 0)
    {
        puts("Created file: notes.txt\n");
    }
    else
    {
        puts("Failed to create file: notes.txt\n");
    }

    while (1)
    {
    }
}