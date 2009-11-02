/*****
*
* Copyright (C) 2008 PreludeIDS Technologies. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@prelude-ids.com>
*
* This file is part of the Prelude library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****/

#include <stdio.h>

#ifndef _LIBPRELUDE_PRELUDE_HXX
#define _LIBPRELUDE_PRELUDE_HXX

#include "LibPrelude/CppBind/prelude-client.hpp"
#include "LibPrelude/CppBind/prelude-client-easy.hpp"
#include "LibPrelude/CppBind/prelude-connection.hpp"
#include "LibPrelude/CppBind/prelude-connection-pool.hpp"

#include "LibPrelude/CppBind/idmef.hpp"
#include "LibPrelude/CppBind/idmef-path.hpp"
#include "LibPrelude/CppBind/idmef-value.hpp"
#include "LibPrelude/CppBind/idmef-criteria.hpp"

const char *CheckVersion(const char *version = NULL);

#endif
/* public header */
