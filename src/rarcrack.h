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
 */
#ifndef _RARCRACK_H
#define _RARCRACK_H

#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>


typedef struct _params
{
    gchar* file_ext;    /* the archive filename */
    gchar* file_type;   /* archive type */
    guint  file_type_id; /* archive TYPES id */
    gchar* file_status; /* the status filename */
    gchar* current;     /* current password (from we are resuming) */
    gchar* charset;     /* character-set specification */
    gchar* file_dict;   /* dictionary file */

    /* TODO: if (!charset && !file_dict) we read passwords from stdin */
    int threads;        /* number of concurrent cracking threads */
} params;


#endif
