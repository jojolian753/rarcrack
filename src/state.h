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
#ifndef RARCRACK_STATE_H
#define RARCRACK_STATE_H 1

#include "rarcrack.h"


gboolean state_load(gchar*, params*);
void state_save(gchar*, params*);


#endif // RARCRACK_STATE_H
