/*
 *  Copyright (C) 2009 by David Kedves <kedazo@gmail.com>
 *
 *  This file is part of rarcrack.
 *
 *  Rarcrack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Rarcrack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with rarcrack.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "state.h"

/**
 * Loads state file, and fills params struct
 * 
 * @param f_status The status file name
 * @param p Params struct to fill, existing options 
 *          will be replaced, except threads (if it's nonzero).
 * @return TRUE when status succesfully restored from f_status
 */
gboolean state_load(gchar* filename, params* p) {
    GKeyFile* file;
    gchar* f_status = g_strdup_printf("%s.rcs", filename);
    gchar* str_val = 0;   
    if (!g_file_test(f_status, G_FILE_TEST_EXISTS | 
                               G_FILE_TEST_IS_REGULAR)) {
        g_free(f_status);
        return FALSE; /* resume file isn't exists... */
    }
    if (!g_key_file_load_from_file(file, f_status, G_KEY_FILE_NONE |
                                  G_KEY_FILE_KEEP_COMMENTS, NULL)) {
        gchar* backup = g_strdup_printf("%s.bak", f_status);
        (void) g_rename(f_status, backup);
        g_printf("Status file loading error:\n"
                 "The old status file renamed to %s, "
                 "and i will create a new one.\n", backup);
        g_usleep(1000);
        g_free(backup);
        g_free(f_status);
        return FALSE; /* status file error... */
    }
    /* [files] */
    /* archive=archive.ext */
    str_val = g_key_file_get_string(file, "files", "archive", NULL);      
    if (str_val) {
        if (p->file_ext) g_free(p->file_ext);
        p->file_ext = str_val;
    }
    /* dictionary=dictfile.txt */
    if (g_key_file_has_key(file, "files", "dictionary", NULL)) {
        if (p->file_dict) g_free(p->file_dict);
        p->file_dict = g_key_file_get_string(file, "files",
                                             "dictionary", NULL);
    }
    /* [cracking] */
    /* charset=Escaped character set... */
    if (g_key_file_has_key(file, "cracking", "charset", NULL)) {
        if (p->charset) g_free(p->charset);
        p->charset = g_key_file_get_string(file, "cracking",
                                           "charset", NULL);
    }
    /* current=current password... */
    if (g_key_file_has_key(file, "cracking", "current", NULL)) {
        if (p->current) g_free(p->current);
        p->current = g_key_file_get_string(file, "cracking",
                                           "current", NULL);
    }
    /* threads=n */
    if ((p->threads < 1) &&  (g_key_file_has_key(file, "cracking",
                                                 "threads", NULL)))
        p->threads = g_key_file_get_integer(file, "cracking", 
                                            "threads", NULL);
    if (p->threads < 1) p->threads = 1;

    g_free(f_status); /* freeing state filename */
    g_key_file_free(file);
    return TRUE;
}

void state_save(char* fn, params* p) {
    GKeyFile* file;
    gboolean success = FALSE;
    gchar* fn_rcs = g_strdup_printf("%s.rcs", fn);
    gchar* contents = 0; 
    file = g_key_file_new();
    /* [files]
     * archive=archive.ext */
    g_key_file_set_string(file, "files", "archive", p->file_ext);
    /* dictionary=file_dict.txt */
    if (p->file_dict)  /* optional */
        g_key_file_set_string(file, "files", "dictionary", p->file_dict);
    /* [cracking]     
     * charset=Escaped character set */
    if (p->charset) /* optional */
        g_key_file_set_string(file, "cracking", "charset", p->charset);

    /* current=kfdFD43
     * threads=4 */
    g_key_file_set_string(file, "cracking", "current", p->current);
    g_key_file_set_integer(file, "cracking", "threads", p->threads);

    contents = g_key_file_to_data(file, NULL, NULL); /* i don't care about size / errors */
    success = g_file_set_contents(fn_rcs, contents, -1, NULL);
    if (!success)
        g_critical("CRITICAL: Failure on writing '%s'!"
                   "Please check permissions!\n", fn_rcs);

    g_free(contents); /* freeing state file contents */
    g_free(fn_rcs); /* freeing state filename */
    g_key_file_free(file);
    return;
}

