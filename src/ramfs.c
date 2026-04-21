#include "ramfs.h"

ramfs_t ramfs;

/* Compare two strings and return 1 if they match. */
static int strings_equal(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return 0;
        }

        i = i + 1;
    }

    if (a[i] == '\0' && b[i] == '\0')
    {
        return 1;
    }

    return 0;
}

/* Copy one string into another buffer. */
static void string_copy(char *dest, const char *src, int max_len)
{
    int i = 0;

    while (i < max_len - 1 && src[i] != '\0')
    {
        dest[i] = src[i];
        i = i + 1;
    }

    dest[i] = '\0';
}

/* Reset the RAM file system to an empty state. */
void ramfs_init(void)
{
    int i;
    int j;

    for (i = 0; i < RAMFS_MAX_FILES; i = i + 1)
    {
        ramfs.files[i].used = 0;
        ramfs.files[i].size = 0;

        for (j = 0; j < RAMFS_MAX_FILENAME; j = j + 1)
        {
            ramfs.files[i].name[j] = '\0';
        }

        for (j = 0; j < RAMFS_MAX_FILESIZE; j = j + 1)
        {
            ramfs.files[i].data[j] = '\0';
        }
    }
}

/* Create a file if the name is new and a free slot exists. */
int ramfs_create(const char *name)
{
    int i;

    for (i = 0; i < RAMFS_MAX_FILES; i = i + 1)
    {
        if (ramfs.files[i].used == 1 && strings_equal(ramfs.files[i].name, name) == 1)
        {
            return -1;
        }
    }

    for (i = 0; i < RAMFS_MAX_FILES; i = i + 1)
    {
        if (ramfs.files[i].used == 0)
        {
            ramfs.files[i].used = 1;
            ramfs.files[i].size = 0;
            string_copy(ramfs.files[i].name, name, RAMFS_MAX_FILENAME);
            return 0;
        }
    }

    return -2;
}