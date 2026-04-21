#ifndef RAMFS_H
#define RAMFS_H

#define RAMFS_MAX_FILES 16
#define RAMFS_MAX_FILENAME 32
#define RAMFS_MAX_FILESIZE 256

/* One file stored in RAM. */
typedef struct
{
    char name[RAMFS_MAX_FILENAME];
    char data[RAMFS_MAX_FILESIZE];
    int size;
    int used;
} ramfs_file_t;

/* The full RAM file system. */
typedef struct
{
    ramfs_file_t files[RAMFS_MAX_FILES];
} ramfs_t;

/* Global RAM file system instance. */
extern ramfs_t ramfs;

/* Initialize all RAMFS entries as empty. */
void ramfs_init(void);

/* Create a file with the given name. */
int ramfs_create(const char *name);

/* Write text into an existing file. */
int ramfs_write(const char *name, const char *data);

/* Return the stored contents of a file, or 0 if not found. */
const char *ramfs_read(const char *name);

#endif