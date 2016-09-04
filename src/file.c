#include "file.h"


types TYPES[] = {
    {"rar", "unrar", "t -y -p%s %s", {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0}},
    {"zip", "unzip", "-P%s -t %s",  {0x50, 0x4B, 0x03, 0x04, 0}},
    {"7zip", "7z", "t -y -p%s %s", {0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0}},
    {NULL, NULL, NULL, {0}} /* end of types */
};


/*
 * Try to run unpacker command,
 * if there is no such command/program
 * this function returns FALSE
 */
static gboolean test_for_packer(gchar* packer) {
    gboolean ret = FALSE;
#ifdef WIN32
    packer = g_strdup_printf("%s > NUL", packer);
#else
    packer = g_strdup_printf("%s 2> /dev/null > /dev/null", packer);
#endif
    gint totest = system(packer);

    g_free(packer);

    if ( (totest >= 0) &&    /* if result negative, error occured */
         (totest != 127) &&  /* result = 127 -> execve failed */
         (totest != 32512) ) { /* same from shell 127 << 8 WTF? */
         ret = TRUE; /* everything seems ok */
    }

    return ret;
}


/*
 * Open file, and try to identify archive type
 * returns id in TYPES array (so we know type/packer/...)
 */
guint file_identify(const gchar* filename, gboolean force)
{
    guint i = 0;
    gint j = 0;
    gchar totest[11] = {0};
    guint ret = FILE_NOTYPE; /* if we can't get packer for this type,
                                  we return FILE_NOTYPE */
    FILE* fp = (FILE*) g_fopen(filename, "r");
    if (!fp)
    {
        return FILE_ERROR; /* file opening error */
    }
    fread(totest, sizeof(gchar), 10, fp);
    fclose(fp);

    if (!totest[0])
    {
        return FILE_ERROR; /* file reading error */
    }

    for (i = 0; TYPES[i].name; ++i)
    {
        j = 0;  
        while ( (TYPES[i].magic[j] != 0) && (j < 10) ) {
            if (TYPES[i].magic[j] != totest[j]) {
                j = 100; /* no similarity */
                break;
            }
            j++;
        }
        if (j != 100) { /* yeah, we found the type/packer/... */
            ret = i; /* we return this TYPES id */
            break;
        }
    }

    /* on forcing we don't check unpacker... */
    if ((!force) && (ret != FILE_NOTYPE))
    {
        if (! test_for_packer(TYPES[ret].cmd))
        {
            ret = FILE_NOPACKER; /* RarCrack can't run unpacker ... :-( */
        }
    }

    return ret; /* we return the type or FILE_NOTYPE */
}


/*
 * Returns id in TYPES array for 'type_ext' archive type
 */
guint file_packer_get_id(const gchar* type_ext, gboolean force) {
    guint i = 0;
    guint ret = FILE_NOTYPE; /* if we can't get packer for this type,
                                  we return FILE_NOTYPE */
    if (!type_ext) /* if someone provide NULL parameter to this function */
    {
        return ret;
    }

    for (i = 0; TYPES[i].name; ++i)
    {
        if (strcmp(type_ext, TYPES[i].name) == 0)
        {
            ret = i;
            break;
        }
    }

    /* on forcing we don't check unpacker ... */
    if ((!force) && (ret != FILE_NOTYPE))
    {
        if (! test_for_packer(TYPES[ret].cmd))
        {
            ret = FILE_NOPACKER; /* RarCrack can't run unpacker ... :-( */
        }
    }

    return ret;
}


/*
 * Return unpacker command with needed arguments.
 * NonForcing: Returned string must be freed after use.
 * Forcing: Do not free returned string...
 *
 * // Yeah it is really st0pid c0de.... FIXME
 */
gchar* file_packer_get_command(guint id, gboolean force) {
    gchar* ret;
    if (!force) {
#ifdef WIN32
        ret = g_strdup_printf("%s %s > NUL", TYPES[id].cmd,
                                             TYPES[id].arg);
#else
        ret = g_strdup_printf("%s %s 2>&1 > /dev/null", TYPES[id].cmd,
                                                        TYPES[id].arg);
#endif
    } else {
        ret = TYPES[id].cmd; /* i just need the program name,  *
                              * without any junk.              */
    }

    return ret;
}


/*
 * Return archive type name (eg.: rar/zip/7zip),
 * You shall not free this string (if you don't want SIGSERV)
 */
gchar* file_packer_get_type(guint id)
{
    return TYPES[id].name;
}

void file_packer_list_supported()
{
    guint i = 0;

    g_printf("            Supported archive types:           \n");
    g_printf("-----------------------------------------------\n");
    g_printf("|  Type   |  Needed executable   | Installed? |\n");
    g_printf("-----------------------------------------------\n");
    for (i = 0; TYPES[i].name; ++i) {
        g_printf("|   %-6s|        %-14s|    %-8s|\n",
                 TYPES[i].name, TYPES[i].cmd,
                 ( test_for_packer(TYPES[i].cmd) ? "Yes" : "No" ));
    }
    g_printf("-----------------------------------------------\n\n");

    return;
}
