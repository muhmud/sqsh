/*
 * var_hist.c - Variables that affect the global history
 *
 * Copyright (C) 1995, 1996 by Scott C. Gray
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, write to the Free Software
 * Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * You may contact the author :
 *   e-mail:  gray@voicenet.com
 *            grays@xtend-tech.com
 *            gray@xenotropic.com
 */
#include <stdio.h>
#include "sqsh_config.h"
#include "sqsh_env.h"
#include "sqsh_error.h"
#include "sqsh_global.h"
#include "var.h"

/*-- Current Version --*/
#if !defined(lint) && !defined(__LINT__)
static char RCS_Id[] = "$Id: var_hist.c,v 1.1.1.1 2004/04/07 12:35:05 chunkm0nkey Exp $" ;
USE(RCS_Id)
#endif /* !defined(lint) */

/*
 * var_set_histsize()
 *
 * Validate that value is a number and automatically adjust the size
 * of the history.
 */
int var_set_histsize( env, var_name, var_value )
	env_t    *env ;
	char     *var_name ;
	char     **var_value ;
{
	int  histsize ;

	if( var_set_int( env, var_name, var_value ) == False )
		return False ;

	histsize = atoi(*var_value) ;

	if( history_set_size( g_history, histsize ) == False ) {
		sqsh_set_error( sqsh_get_error(), "history_set_size: %s",
		                sqsh_get_errstr() ) ;
	 	return False ;
	}

	return True ;
}
