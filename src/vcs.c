#include "vcs.h"

vcs_t vcs;

/* Copy one string into another buffer safely. */
static void vcs_copy_string(char *destination, const char *source, int max_length)
{
    int i = 0;

    while (i < max_length - 1 && source[i] != '\0')
    {
        destination[i] = source[i];
        i = i + 1;
    }

    destination[i] = '\0';
}

/* Copy a file buffer into a blob buffer. */
static void vcs_copy_data(char *destination, const char *source, int max_length)
{
    int i = 0;

    while (i < max_length - 1 && source[i] != '\0')
    {
        destination[i] = source[i];
        i = i + 1;
    }

    destination[i] = '\0';
}

/* Return the length of a string. */
static int vcs_string_length(const char *str)
{
    int len = 0;

    while (str[len] != '\0')
    {
        len = len + 1;
    }

    return len;
}

/* Initialize all VCS structures as empty. */
void vcs_init(void)
{
    int i;
    int j;

    vcs.initialized = 1;
    vcs.head = 0;

    for (i = 0; i < VCS_MAX_BLOBS; i = i + 1)
    {
        vcs.blobs[i].size = 0;
        vcs.blobs[i].hash = 0;
        vcs.blobs[i].used = 0;

        for (j = 0; j < RAMFS_MAX_FILENAME; j = j + 1)
        {
            vcs.blobs[i].filename[j] = '\0';
        }

        for (j = 0; j < RAMFS_MAX_FILESIZE; j = j + 1)
        {
            vcs.blobs[i].data[j] = '\0';
        }
    }

    for (i = 0; i < VCS_MAX_TREE_ENTRIES; i = i + 1)
    {
        int entry_index;

        vcs.trees[i].entry_count = 0;
        vcs.trees[i].used = 0;

        for (entry_index = 0; entry_index < VCS_MAX_TREE_ENTRIES; entry_index = entry_index + 1)
        {
            int k;

            vcs.trees[i].entries[entry_index].blob_index = -1;
            vcs.trees[i].entries[entry_index].used = 0;

            for (k = 0; k < RAMFS_MAX_FILENAME; k = k + 1)
            {
                vcs.trees[i].entries[entry_index].filename[k] = '\0';
            }
        }
    }

    for (i = 0; i < VCS_MAX_COMMITS; i = i + 1)
    {
        vcs.commits[i].tree_index = -1;
        vcs.commits[i].hash = 0;
        vcs.commits[i].used = 0;
        vcs.commits[i].parent = 0;

        for (j = 0; j < VCS_MAX_COMMIT_MESSAGE; j = j + 1)
        {
            vcs.commits[i].message[j] = '\0';
        }
    }
}

/* Hash a string using the 32-bit FNV-1a algorithm. */
unsigned int fnv1a_hash(const char *data)
{
    unsigned int hash = 2166136261u;
    int i = 0;

    while (data[i] != '\0')
    {
        hash = hash ^ (unsigned int)(unsigned char)data[i];
        hash = hash * 16777619u;
        i = i + 1;
    }

    return hash;
}

/* Create a VCS blob from an existing RAMFS file. */
int vcs_create_blob_from_file(const char *filename)
{
    const char *content;
    int i;
    int free_index;

    content = ramfs_read(filename);

    if (content == 0)
    {
        return -1;
    }

    free_index = -1;

    for (i = 0; i < VCS_MAX_BLOBS; i = i + 1)
    {
        if (vcs.blobs[i].used == 0)
        {
            free_index = i;
            i = VCS_MAX_BLOBS;
        }
    }

    if (free_index == -1)
    {
        return -1;
    }

    vcs.blobs[free_index].used = 1;
    vcs_copy_string(vcs.blobs[free_index].filename, filename, RAMFS_MAX_FILENAME);
    vcs_copy_data(vcs.blobs[free_index].data, content, RAMFS_MAX_FILESIZE);
    vcs.blobs[free_index].size = vcs_string_length(content);
    vcs.blobs[free_index].hash = fnv1a_hash(vcs.blobs[free_index].data);

    return free_index;
}

/* Create one empty tree in the VCS. */
int vcs_create_tree(void)
{
    int i;

    for (i = 0; i < VCS_MAX_TREE_ENTRIES; i = i + 1)
    {
        if (vcs.trees[i].used == 0)
        {
            vcs.trees[i].used = 1;
            vcs.trees[i].entry_count = 0;
            return i;
        }
    }

    return -1;
}

/* Add one blob into one tree. */
int vcs_tree_add_blob(int tree_index, int blob_index)
{
    int entry_index;

    if (tree_index < 0 || tree_index >= VCS_MAX_TREE_ENTRIES)
    {
        return -1;
    }

    if (blob_index < 0 || blob_index >= VCS_MAX_BLOBS)
    {
        return -1;
    }

    if (vcs.trees[tree_index].used == 0)
    {
        return -1;
    }

    if (vcs.blobs[blob_index].used == 0)
    {
        return -1;
    }

    if (vcs.trees[tree_index].entry_count >= VCS_MAX_TREE_ENTRIES)
    {
        return -1;
    }

    entry_index = vcs.trees[tree_index].entry_count;

    vcs.trees[tree_index].entries[entry_index].used = 1;
    vcs.trees[tree_index].entries[entry_index].blob_index = blob_index;
    vcs_copy_string(
        vcs.trees[tree_index].entries[entry_index].filename,
        vcs.blobs[blob_index].filename,
        RAMFS_MAX_FILENAME);

    vcs.trees[tree_index].entry_count = vcs.trees[tree_index].entry_count + 1;

    return 0;
}