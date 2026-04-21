#include "vcs.h"

vcs_t vcs;

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