#ifndef _FILE_H
#define _FILE_H

#include <glib.h>
#include <glib/gstdio.h> /* for g_fopen */

enum {
    FILE_ERROR = 100, /* file not found / permission error */
    FILE_NOPACKER,    /* file identified but packer program is missing
                         eg.: rar, but there is no unrar */
    FILE_NOTYPE       /* we can't examine the file type */
};

/* forcing means skip test for unpacker prog */
guint file_identify(const gchar* filename, gboolean force);
guint file_packer_get_id(const gchar* type_ext, gboolean force);
gchar* file_packer_get_command(guint id, gboolean force); /* return unpacker with needed arguments */
gchar* file_packer_get_type(guint id);
void file_packer_list_supported(void);

typedef struct _types {
    char* name; /* type name: rar */
    char* cmd;  /* command to process: unrar */
    char* arg;  /* arguments to cmd: "z -y -p%s %s" for unrar */
    char magic[11]; /* file header magic bytes to identify type */
} types;

                           
#endif
