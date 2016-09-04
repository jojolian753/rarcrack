/*
 *  Copyright (C) 2009 by David Kedves <kedazo@gmail.com>
 *
 *  rarcrack is a password cracker, it's supports lot of types of archives.
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
 */
#define VERSION "0.3"

#include "rarcrack.h"
#include "state.h"
#include "file.h"


/* main parameters */
params parameters;
gint minimum_length = 1;
gboolean just_temp = TRUE;
gboolean list_supported_archives = FALSE; 


static GOptionEntry entries[] = {
    { "bruteforce", 'b', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE, 
    &just_temp, "Bruteforce cracking [Default]", NULL },
    { "dictionary", 'd', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
    &parameters.file_dict, "Dictionary attack (- means from stdin)", "dict" },
    { "current", 'c', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, 
    &parameters.current, "Starting password", "firstpass" },
    { "type", 't', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
    &parameters.file_type, "Archive file type", "type" },
    { "list", 'l', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE,
    &list_supported_archives, "List of supported archive types", NULL}, 
    { "charset", 's', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
    &parameters.charset, "Character set (only on bruteforce)", "set" },
    { "minlen", 'm', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_INT,
    &minimum_length, "Minimum password length (only on bruteforce)", "X"},
    { "threads", 'p', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_INT,
    &parameters.threads, "Number of parallel cracking threads", "N" },
    { NULL }
};


int main(int argc, char** argv)
{
    GError *error = NULL;
    GOptionContext *context;

    /* some GLibrary initialization: */
    // if (!g_thread_supported()) g_thread_init(NULL);

    parameters.file_dict = NULL;
    parameters.current = NULL;
    parameters.file_type = NULL;
    parameters.charset = NULL;
    parameters.threads = 1;

    /* print program name and some version info */
    g_printf("RarCrack! %s\n\n", VERSION);

    context = g_option_context_new("filename - crack archive files");
    g_option_context_add_main_entries(context, entries, NULL);
    g_option_context_set_description(
        context,
        "Web:\n  http://rarcrack.sourceforge.net\n\n"
        "Author:\n  David Kedves <kedazo@gmail.com>\n"
    );
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("Error: %s\n", error->message);
        return 1; /* exiting with error code 1 */
    }

    if (list_supported_archives)
    {
        file_packer_list_supported();
        return 0;
    }

    if (argc == 2) { /* we got only one filename */
        if (! g_file_test(argv[1], G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS)) {
            g_printf("Error: No such file: '%s'\n\n", argv[1]);
            return 1; /* return with error */
        }
        parameters.file_ext = g_strdup(argv[1]);
        parameters.file_status = g_strdup_printf("%s.rcs", argv[1]);
        g_printf("Status filename: '%s'\n", parameters.file_status);
    }
    else
    {
        if (argc > 2)
        {
            g_printf("Error: Multiple files specified in parameter list.\n"
                     "       The program can crack only one file.\n");
        }
        else
        {
            g_printf("Error: Missing filename!\n");
        }
        g_printf("Info: See '%s --help'\n\n", argv[0]);

        return 1; /* returning with error */
    }

    if (parameters.file_type)
    {
        g_printf("Info: forced archive type: %s\n", parameters.file_type);
        parameters.file_type_id = file_packer_get_id(parameters.file_type, FALSE);
    }
    else
    {
        parameters.file_type_id = file_identify(parameters.file_ext, FALSE);
    }
    switch (parameters.file_type_id) {
        case FILE_ERROR:
            g_printf(
                "Error: I can't open archive file\n"
                "       Please check the permissions!\n\n"
            );
            break;
        case FILE_NOTYPE:
            if (parameters.file_type)
            {
                g_printf("Error: RarCrack! isn't support this file type!\n\n");
            }
            else
            {
                g_printf(
                    "Error: RarCrack! can't detect the archive type!\n"
                    "Info: If you sure about type, you may try forcing it...\n"
                    "      See help (%s --help) for more info!\n\n", argv[0]
                );
            }
            return 1;
            break; /* Control never reach this line ...*/
        case FILE_NOPACKER: {
            gchar* you_need_this;
            if (parameters.file_type)
            {
                you_need_this = file_packer_get_command(
                    file_packer_get_id(parameters.file_type, TRUE), TRUE);
            }
            else
            {
                you_need_this = file_packer_get_command(
                    file_identify(parameters.file_ext, TRUE), TRUE);
            }
            g_printf("Error: RarCrack! can't run '%s' command, please ensure of\n"
                     "it is installed and available in you PATH\n\n", you_need_this);
            return 1;
            }
            break;
        default:
            if (!parameters.file_type)
            {
                parameters.file_type = file_packer_get_type(
                    parameters.file_type_id
                );
                g_printf("Info: archive type: '%s'\n", parameters.file_type);
            }
            break;
    }

    return 0;
}
