/*
 * sqsh_history.c - Rolling queue of command histories
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
#include <sys/stat.h>
#include "sqsh_config.h"
#include "sqsh_error.h"
#include "sqsh_expand.h"
#include "sqsh_global.h"
#include "sqsh_history.h"
#include "sqsh_varbuf.h"

/*-- Current Version --*/
#if !defined(lint) && !defined(__LINT__)
static char RCS_Id[] = "$Id: sqsh_history.c,v 1.10 2013/05/03 11:19:38 mwesdorp Exp $" ;
USE(RCS_Id)
#endif /* !defined(lint) */

/*-- Local Prototypes --*/
static hisbuf_t* hisbuf_create  _ANSI_ARGS(( history_t*, char*, int )) ;
static hisbuf_t* hisbuf_get     _ANSI_ARGS(( history_t*, int )) ;
static int       hisbuf_destroy _ANSI_ARGS(( hisbuf_t* )) ;
static unsigned long adler32    _ANSI_ARGS(( char*, int )) ;   /* sqsh-2.1.6 feature */
static int       chk_buf_ifs    _ANSI_ARGS(( char*, int )) ;   /* sqsh-2.1.7 feature */
static void      hist_auto_save _ANSI_ARGS(( history_t* )) ;   /* sqsh-2.1.7 feature */
static int       history_merge  _ANSI_ARGS(( history_t*, history_t*)) ; /* sqsh-2.2.0 feature */


/*
 * history_create():
 *
 * Allocates a history structure large enough to hold up to size history
 * entries.
 */
history_t* history_create( size )
    int   size ;
{
    history_t  *h ;

    /*-- Always check your parameters --*/
    if( size < 0 ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return NULL ;
    }

    /*-- Allocate a new history structure --*/
    if( (h = (history_t*)malloc(sizeof(history_t))) == NULL ) {
        sqsh_set_error( SQSH_E_NOMEM, NULL ) ;
        return NULL ;
    }

    /*-- Initialize elements of structure --*/
    h->h_size     = size ;
    h->h_nitems   = 0 ;
    h->h_change   = HISTSAVE_INIT ;
    h->h_start    = NULL ;
    h->h_next_nbr = 1 ;
    h->h_end      = NULL ;

    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    return h ;
}

/*
 * history_get_size():
 *
 * Returns the total size of the current history.
 */
int history_get_size( h )
    history_t  *h ;
{
    /*-- Check parameters --*/
    if( h == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return -1 ;
    }

    return h->h_size ;
}

/*
 * history_get_nitems():
 *
 * Returns the total number of entries in history.
 */
int history_get_nitems( h )
    history_t  *h ;
{
    /*-- Check parameters --*/
    if( h == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return -1 ;
    }

    return h->h_nitems ;
}

/*
 * history_get_nbr():
 *
 * Returns the next available history entry number.
 */
int history_get_nbr( h )
    history_t  *h ;
{
    return h->h_next_nbr ;
}

int history_set_size( h, size )
    history_t *h ;
    int        size ;
{
    hisbuf_t  *hb ;

    /*-- Check parameters --*/
    if( h == NULL || size < 0 ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    /*
     * If the size was smaller than the current number of items
     * in the history then we need to roll older items off of
     * the history until we can fit the new size.
     */
    while( size < h->h_nitems ) {

        /*
         * Unlink the oldest entry from the list and destroy
         * it.
         */
        hb = h->h_end ;
        if( hb->hb_prv != NULL ) {
            h->h_end         = hb->hb_prv ;
            h->h_end->hb_nxt = NULL ;
        } else {
            h->h_end = h->h_start = NULL ;
        }
        hisbuf_destroy( hb ) ;

        --h->h_nitems ;
    }
    hist_auto_save ( h ) ;

    h->h_size = size ;

    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    return True ;
}

/*
 * history_append():
 *
 * Adds buffer buf to history h.
 */
int history_append( h, buf )
    history_t   *h ;
    char        *buf ;
{
    hisbuf_t  *hisbuf, *hb_tmp ;
    int        len ;
    /* sqsh-2.1.6 - New variables */
    register int   i;
    unsigned long  chksum;
    char          *histunique;


    /*-- Check parameters --*/
    if( h == NULL || buf == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    /*-- Get length of buffer --*/
    len = strlen( buf ) ;

    /*
     * If the buffer is set up to hold no entries, or the buffer is
     * empty, then we don't need to go any further.
     */
    if( h->h_size == 0 || len == 0 ) {
        sqsh_set_error( SQSH_E_NONE, NULL ) ;
        return True ;
    }

    /*
     * sqsh-2.1.7 feature - Ignore 'empty' buffers with only
     * IFS characters.
     */
    if (chk_buf_ifs (buf, len))
        return True ;

    /*
     * sqsh-2.1.6 feature - Calculate a checksum on the buffer.
     * If we do not want to store duplicate history entries, then
     * we search the linked list for an identical entry. If found,
     * do not store the buffer in the list, but maintain MRU-LRU
     * order; else continue as usual.
    */
     chksum = adler32 (buf, len);
     DBG(sqsh_debug(DEBUG_HISTORY, "sqsh_history: checksum of buffer: %u\n", chksum);)
     env_get (g_env, "histunique", &histunique);
     if (histunique != NULL && *histunique != '0')
     {
       for (hb_tmp = h->h_start; hb_tmp != NULL && hb_tmp->hb_chksum != chksum;
            hb_tmp = hb_tmp->hb_nxt);

       /*
        * If hb_tmp is not NULL we just found an identical history buffer entry.
       */
       if (hb_tmp != NULL)
       {
         /*
          * sqsh-2.1.7 - Adjust buffer access datetime and increase buffer usage count.
         */
         time( &hb_tmp->hb_dttm );
         hb_tmp->hb_count++;

         /*
          * - Nothing to do when it is already the first entry
          *   (or only one).
         */
         if (hb_tmp == h->h_start)
         {
           sqsh_set_error( SQSH_E_NONE, NULL ) ;
           return True;
         }
         else
         {
           /*
            * Is it currently the last entry then we have to adjust h->h_end,
            * else we have to pull the current entry out of the list.
           */
           if (hb_tmp == h->h_end)
           {
             h->h_end               = hb_tmp->hb_prv;
             h->h_end->hb_nxt       = NULL;
           }
           else
           {
             hb_tmp->hb_prv->hb_nxt = hb_tmp->hb_nxt;
             hb_tmp->hb_nxt->hb_prv = hb_tmp->hb_prv;
           }
         }
         /*
          * Now put the entry to the start of the list.
          * (Also save the recent buffer number for renumbering the list).
         */
         i                        = h->h_start->hb_nbr;
         h->h_start->hb_prv       = hb_tmp;
         hb_tmp->hb_nxt           = h->h_start;
         h->h_start               = hb_tmp;
         hb_tmp->hb_prv           = NULL;
         /*
          * Adjust the buffer numbers in the list.
         */
         for (hb_tmp = h->h_start; hb_tmp != NULL && hb_tmp->hb_nbr != i;
              hb_tmp->hb_nbr = i--, hb_tmp = hb_tmp->hb_nxt);
         hist_auto_save ( h ) ;

         sqsh_set_error( SQSH_E_NONE, NULL ) ;
         return True;
       }
     }

    /*
     * Go ahead an try to allocate a buffer before we try to
     * stick it into the history.
     */
    if( (hisbuf = hisbuf_create( h, buf, len )) == NULL ) {
        sqsh_set_error( SQSH_E_NOMEM, NULL ) ;
        return False ;
    }

    /*
     * sqsh-2.1.6 feature - Store the checksum of the new buffer in the structure.
    */
    hisbuf->hb_chksum = chksum;

    /*
     * sqsh-2.1.7 - Set current date and time in hb_dttm and set the usage count
     * of the buffer to 1 in hb_count.
    */
    time( &hisbuf->hb_dttm );
    hisbuf->hb_count = 1;

    /*
     * If the buffer is full, then we need to roll and entry off
     * of the beginning of the list.
     */
    if( h->h_nitems == h->h_size ) {

        hb_tmp = h->h_end->hb_prv ;    /* Temp pointer to next-to-last */
        hisbuf_destroy( h->h_end ) ;   /* Destroy last entry */
        h->h_end = hb_tmp ;            /* Make end be the next-to-last */

        if( h->h_end != NULL )         /* If there is an end */
            h->h_end->hb_nxt = NULL ;   /* Then it doesn't point anywhere */
        else
            h->h_start = NULL ;         /* Otherwise list is empty */

        --h->h_nitems ;                /* One fewer... */
    }

    /*
     * Now that we have the space, stick the new hisbuf_t into
     * our history list.
     */
    if( h->h_start == NULL ) {
        h->h_start = hisbuf ;
        h->h_end   = hisbuf ;
    } else {
        h->h_start->hb_prv = hisbuf ;
        hisbuf->hb_nxt     = h->h_start ;
        h->h_start         = hisbuf ;
    }
    ++h->h_nitems ;
    hist_auto_save ( h ) ;

    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    return True ;
}

int history_find( h, idx, buf )
    history_t  *h ;
    int         idx ;
    char      **buf ;
{
    hisbuf_t  *hb ;

    /*-- Check parameters not checked by hisbuf_get() --*/
    if( buf == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    /*-- Retrieve the appropriate history entry */
    if( (hb = hisbuf_get( h, idx )) == NULL )
        return False ;

    *buf = hb->hb_buf ;

    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    return True ;
}

/*
 * history_del():
 *
 * Deletes entry idx from history h.
 */
int history_del( h, idx )
    history_t   *h ;
    int          idx ;
{
    hisbuf_t   *hb ;
    register int i ; /* sqsh-2.1.6 - New variable */


    /*-- Retrieve the appropriate history entry */
    if( (hb = hisbuf_get( h, idx )) == NULL )
        return False ;

    /*-- Unlink the node --*/
    if( hb->hb_prv != NULL )
        hb->hb_prv->hb_nxt = hb->hb_nxt ;
    else
        h->h_start = hb->hb_nxt ;

    if( hb->hb_nxt != NULL )
        hb->hb_nxt->hb_prv = hb->hb_prv ;
    else
        h->h_end = hb->hb_prv ;

    hisbuf_destroy( hb ) ;

    /*
     * sqsh-2.1.6 feature - Adjust h_nitems, h_next_nbr and renumber the list.
    */
    switch ( --h->h_nitems )
    {
        case 0 :
            h->h_next_nbr = 1;
            break;

        case 1 :
            h->h_start->hb_nbr = 1;
            h->h_next_nbr = 2;
            break;

        default :
            for (hb = h->h_start, i = hb->hb_nbr, h->h_next_nbr = i + 1;
                 hb != NULL; hb->hb_nbr = i--, hb = hb->hb_nxt);
            break;
    }
    h->h_change = HISTSAVE_FORCE;
    hist_auto_save ( h );

    return True ;
}

/*
 * sqsh-2.1.7 - history_range_del():
 *
 * Deletes entries idx_start through idx_end from history h.
 */
int history_range_del( h, idx_start, idx_end )
    history_t   *h ;
    int          idx_start ;
    int          idx_end ;
{
    hisbuf_t   *hb ;
    register int i ;

    for ( i = idx_start; i <= idx_end; i++ ) {
        /*-- Retrieve the appropriate history entry */
        if( (hb = hisbuf_get( h, i )) == NULL )
            continue ;

        /*-- Unlink the node --*/
        if( hb->hb_prv != NULL )
            hb->hb_prv->hb_nxt = hb->hb_nxt ;
        else
            h->h_start = hb->hb_nxt ;

        if( hb->hb_nxt != NULL )
            hb->hb_nxt->hb_prv = hb->hb_prv ;
        else
            h->h_end = hb->hb_prv ;

        hisbuf_destroy( hb ) ;
        h->h_nitems--;
    }

    /*
     * Adjust h_next_nbr and renumber the list.
    */
    switch ( h->h_nitems )
    {
        case 0 :
            h->h_next_nbr = 1;
            break;

        case 1 :
            h->h_start->hb_nbr = 1;
            h->h_next_nbr = 2;
            break;

        default :
            for (hb = h->h_start, i = hb->hb_nbr, h->h_next_nbr = i + 1;
                 hb != NULL; hb->hb_nbr = i--, hb = hb->hb_nxt);
            break;
    }
    h->h_change = HISTSAVE_FORCE;
    hist_auto_save ( h );

    return True ;
}

int history_clear( h )
    history_t  *h ;
{
    hisbuf_t  *hb, *hb_nxt ;

    /*-- Check parameters --*/
    if( h == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    hb = h->h_start ;
    while( hb != NULL ) {
        hb_nxt = hb->hb_nxt ;
        hisbuf_destroy( hb ) ;
        hb = hb_nxt ;
    }

    h->h_start  = NULL ;
    h->h_end    = NULL ;
    h->h_nitems = 0 ;
    h->h_change = HISTSAVE_INIT ;

    return True ;
}

/*
 * history_save():
 *
 * Saves the current command history to file named save_file,
 * returns True upon success, False upon failure.
 */
int history_save( h, save_file )
    history_t   *h ;
    char        *save_file ;
{
    hisbuf_t   *hb ;
    FILE       *fptr ;
    mode_t      saved_mask;
    char       *histmerge;
    history_t  *x;


    /*-- Check the arguments --*/
    if( h == NULL || save_file == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    /*
     * sqsh-2.2.0 - Merge the history file with the buffers in memory.
     * If h_change is set to HISTSAVE_FORCE then we do not want to
     * merge, because we may have just removed a bunch of buffers
     * for example.
     */
    env_get (g_env, "histmerge", &histmerge);
    if ( histmerge != NULL && *histmerge == '1' &&
         h->h_change != HISTSAVE_FORCE)
    {
        x = history_create (history_get_size(h));
        if (history_load (x, save_file) == True)
            history_merge (h, x);
        history_destroy (x);
    }

    /*-- Open the file to save to --*/
    /* fix for 1105398 */
    saved_mask = umask( (mode_t) 0066);
    if( (fptr = fopen( save_file, "w" )) == NULL ) {
        sqsh_set_error( errno, "%s: %s", save_file, strerror( errno ) ) ;
        umask(saved_mask);
        return False ;
    }
    umask(saved_mask);

    /*
     * Traverse the list of history buffers from oldest to newest, this
     * way we can perform a regular history_append() on each buffer while
     * loading them in and they will be in the correct order.
     */
    for( hb = h->h_end; hb != NULL; hb = hb->hb_prv ) {
        /*
         * Since buffer can contain just about anything we need to
         * provide some sort of separator.
         * sqsh-2.1.7 - Also store time_t of last buffer access and usage count.
         */
        fprintf( fptr, "--> History Entry <--\n" ) ;
        fprintf( fptr, "--> History Info (%d:%d) <--\n", (int) hb->hb_dttm, hb->hb_count ) ;
        fputs( hb->hb_buf, fptr ) ;

    }

    fclose( fptr ) ;
    h->h_change = HISTSAVE_INIT;

    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    return True ;
}

/*
 * history_load():
 *
 * Load history contained in load file into h.  If h is too small to
 * hold all of the entries in load_file, then the oldest entries are
 * rolled off as necessary.
 */
int history_load( h, load_file )
    history_t    *h ;
    char         *load_file ;
{
    FILE      *fptr ;
    char       str[1024] ;
    varbuf_t  *history_buf ;
    /*
     * sqsh-2.1.7 - Keep track of buffer usage count and last access datetime.
    */
    int        dttm  = 0;
    int        count = 0;

    /*-- Check the arguments --*/
    if( h == NULL || load_file == NULL ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False ;
    }

    /*-- Attempt to open the load file --*/
    if( (fptr = fopen( load_file, "r" )) == NULL ) {
        sqsh_set_error( errno, "%s: %s", load_file, strerror( errno ) ) ;
        return False ;
    }

    /*
     * Create a varbuf structure to hold the current buffer being
     * loaded, this buffer will be destroyed before we return
     * to the caller.
     */
    if( (history_buf = varbuf_create(1024)) == NULL ) {
        sqsh_set_error( sqsh_get_error(), "varbuf_create: %s",
                        sqsh_get_errstr() ) ;
        return False ;
    }

    /*
     * sqsh-2.1.7 - Value HISTSAVE_LOAD indicates we are performing
     * a load and we do not want to save history in hist_auto_save().
     */
    h->h_change = HISTSAVE_LOAD;
    while( fgets( str, sizeof(str), fptr ) != NULL ) {
        /*
         * If we hit the separator for history entries then we need
         * to save the current history buffer that we are building.
         */
        if( strcmp( str, "--> History Entry <--\n" ) == 0 ) {

            /*
             * However, since the first line of the load file contains
             * the separator we don't wan't to build and empty buffer.
             */
            if( varbuf_getlen( history_buf ) > 0 ) {
                /*
                 * Create a new entry.
                 */
                if( history_append( h, varbuf_getstr( history_buf ) ) == False ) {
                    fclose( fptr ) ;
                    varbuf_destroy( history_buf ) ;
                    sqsh_set_error( SQSH_E_NOMEM, NULL ) ;
                    h->h_change = HISTSAVE_INIT;
                    return False ;
                }
                /*
                 * sqsh-2.1.7 - Adjust dttm and usage count for the current buffer.
                */
                h->h_start->hb_dttm   = (time_t) dttm;
                h->h_start->hb_count += count;
                dttm  = 0;
                count = 0;
            }

            varbuf_clear( history_buf ) ;
        } else if ( strncmp( str, "--> History Info (", 18 ) == 0) {
            /*
             * sqsh-2.1.7 - The history file may contain a new entry to store
             * last access date/time and buffer usage count.
             * During buffer allocation the initial count is set to 1. We do
             * not want to overwrite the value in the buffer, just add another
             * occurence to keep uniqueness count correct. Therefor substract
             * one from the value read from the history file.
             */
            char *p;

            p  = strchr (str, ')');
            *p = '\0';
            p  = strchr (str, ':');
            count = atoi (p+1)-1;
            *p = '\0';
            dttm  = atoi (str+18);
        } else {
            /*
             * Since this isn't an entry separator, it is a line that
             * needs to be added to the current buffer.
             */
            if( varbuf_strcat( history_buf, str ) == -1 ) {
                fclose( fptr ) ;
                varbuf_destroy( history_buf ) ;
                sqsh_set_error( SQSH_E_NOMEM, NULL ) ;
                h->h_change = HISTSAVE_INIT;
                return False ;
            }
        }
    }

    fclose( fptr ) ;

    /*
     * If there is anything left in the history buffer then we need
     * to add it to the history as well.
     */
    if( varbuf_getlen( history_buf ) > 0 ) {

        if( history_append( h, varbuf_getstr( history_buf ) ) == False ) {
            varbuf_destroy( history_buf ) ;
            sqsh_set_error( SQSH_E_NOMEM, NULL ) ;
            h->h_change = HISTSAVE_INIT;
            return False ;
        }
        /*
         * sqsh-2.1.7 - Adjust dttm and usage count for the current buffer.
        */
        h->h_start->hb_dttm   = (time_t) dttm;
        h->h_start->hb_count += count;
    }

    varbuf_destroy( history_buf ) ;
    sqsh_set_error( SQSH_E_NONE, NULL ) ;
    h->h_change = HISTSAVE_INIT;
    return True ;
}

int history_destroy( h )
    history_t *h ;
{
    if( history_clear( h ) == False )
        return False ;

    free( h ) ;
    return True ;
}

/*
 * hisbuf_get():
 *
 * Retrieves history entry idx from h If idx is HISTORY_HEAD then the
 * first (most recent) entry into the history is returned and if idx
 * is HISTORY_TAIL the last (oldest) entry is returned.  Upon success
 * True is returned, otherwise False.
 */
static hisbuf_t* hisbuf_get( h, idx )
    history_t  *h ;
    int         idx ;
{
    hisbuf_t  *hb ;

    /*-- Check parameters --*/
    if( h == NULL || (idx < 0 && idx != HISTORY_HEAD && idx != HISTORY_TAIL) ) {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return NULL ;
    }

    /*-- The most recent entry? --*/
    if( idx == HISTORY_HEAD ) {
        if( h->h_start == NULL ) {
            sqsh_set_error( SQSH_E_EXIST, "History is empty" ) ;
            return NULL ;
        }

        return h->h_start ;
    }

    /*-- The most oldest entry? --*/
    if( idx == HISTORY_TAIL ) {
        if( h->h_end == NULL ) {
            sqsh_set_error( SQSH_E_EXIST, "History is empty" ) ;
            return NULL ;
        }

        return h->h_end ;
    }

    /*
     * Perform a rather inefficient, linear, search through
     * our history list looking for idx.
     */
    for( hb = h->h_start; hb != NULL && hb->hb_nbr != idx;
          hb = hb->hb_nxt ) ;

    if( hb == NULL ) {
        sqsh_set_error( SQSH_E_EXIST, "Invalid history number %d", idx ) ;
        return NULL ;
    }

    return hb ;
}


static hisbuf_t* hisbuf_create( h, buf, len )
    history_t *h ;
    char      *buf ;
    int        len ;
{
    hisbuf_t  *hisbuf ;

    /*-- Allocate --*/
    if( (hisbuf = (hisbuf_t*)malloc(sizeof(hisbuf_t))) == NULL )
        return NULL ;

    /*-- Initialize --*/
    hisbuf->hb_len = len ;
    hisbuf->hb_nbr = h->h_next_nbr++ ;
    hisbuf->hb_chksum = 0 ; /* sqsh-2.1.6 feature */
    hisbuf->hb_dttm   = 0 ; /* sqsh-2.1.7 feature */
    hisbuf->hb_count  = 0 ; /* sqsh-2.1.7 feature */
    hisbuf->hb_nxt = NULL ;
    hisbuf->hb_prv = NULL ;

    /*-- Copy the buffer --*/
    if( (hisbuf->hb_buf = sqsh_strdup( buf )) == NULL ) {
        free( hisbuf ) ;
        return NULL ;
    }

    return hisbuf ;
}

static int hisbuf_destroy( hb )
    hisbuf_t  *hb ;
{
    if( hb != NULL ) {
        if( hb->hb_buf != NULL )
            free( hb->hb_buf ) ;
        free( hb ) ;
    }
    return True ;
}


/*
 * sqsh-2.1.6 feature - Function adler32
 *
 * Calculate a simple checksum for the buffer pointed to by data over
 * a length of len bytes, using the Adler algorithm.
*/
#define MOD_ADLER 65521

static unsigned long adler32 (data, len)
    char *data;
    int   len;
{
    unsigned long a = 1, b = 0;

    while (len-- > 0)
    {
        a = (a + *data++) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}


/*
 * sqsh-2.1.7 feature - Function chk_buf_ifs
 *
 * Check if the buffer only contains IFS characters.
 * Return True if this is the case, else return False.
*/
static int chk_buf_ifs (data, len)
    char *data;
    int   len;
{
    char  *ifs = "\f\n\r\t\v ";
    int    i;

    for (i = 0; i < len && strchr(ifs, data[i]) != NULL; ++i);
    if ( i == len )
      return True;
    else
      return False;
}


/*
 * sqsh-2.1.7 feature - Autosave history after more than x
 * configured changes (Variable hist_auto_save).
*/
static void hist_auto_save ( h )
    history_t *h;
{
    varbuf_t *exp_buf;
    char     *history;
    char     *histsave;
    char     *hist_auto_save;


    /*
     * If we are currently loading the history file, then we do
     * not want to start autosaving the history, so back off.
     */
    if (h->h_change == HISTSAVE_LOAD)
        return;

    /*
     * Only try to autosave the history if we have histsave on
     * and a history file and a hist_auto_save value configured.
     */
    env_get (g_env, "history",        &history);
    env_get (g_env, "hist_auto_save", &hist_auto_save);
    env_get (g_env, "histsave",       &histsave);
    if ( history        == NULL || *history        == '\0' ||
         hist_auto_save == NULL || *hist_auto_save == '0'  ||
         histsave       == NULL || *histsave       == '0')
        return;

    if ( h->h_change != HISTSAVE_FORCE &&
         ++h->h_change < atoi(hist_auto_save) )
        return;

    /*
     * Expand the history file and save the buffers to the history.
     * history_save() will also reset the h_change counter.
     */
    exp_buf = varbuf_create( 256 );
    if (exp_buf != NULL)
    {
        if ( sqsh_expand( history, exp_buf, 0 ) == True )
        {
            history_save( h, varbuf_getstr(exp_buf) );
            DBG(sqsh_debug(DEBUG_HISTORY, "sqsh_history: History automatically saved to %s\n",
                varbuf_getstr(exp_buf));)
        }
        varbuf_destroy( exp_buf );
    }

    return;
}


/*
 * history_merge():
 *
 * sqsh-2.2.0 - Merge the history list of x into the history list of h.
 */
static int history_merge (h, x)
    history_t   *h;
    history_t   *x;
{
    hisbuf_t    *hbh;
    hisbuf_t    *hbx;
    hisbuf_t    *hbt;
    int          i;
    char        *histunique;
#if defined (DEBUG)
    char         hdrinfo[64];
    char        *line, *nl;
#endif


    if (h == NULL || x == NULL)
    {
        sqsh_set_error( SQSH_E_BADPARAM, NULL ) ;
        return False;
    }

    /*
     * if histunique=On then remove the oldest double entries
     * (based on hb_chksum) from h or x, otherwise only remove
     * double entries from x, regardless the values of hb_dttm.
     * Kind of nested loop join, h is outer table, x is inner table.
    */
    env_get (g_env, "histunique", &histunique);
    hbh = h->h_start;
    hbx = x->h_start;
    while (hbh != NULL && hbx != NULL)
    {
        if (hbh->hb_chksum == hbx->hb_chksum)
        {
            if (hbx->hb_dttm > hbh->hb_dttm && *histunique == '1')
            {
                hbt = hbh->hb_nxt;
                if (hbh->hb_prv != NULL)
                    hbh->hb_prv->hb_nxt = hbh->hb_nxt;
                else
                    h->h_start          = hbh->hb_nxt;
                if (hbh->hb_nxt != NULL)
                    hbh->hb_nxt->hb_prv = hbh->hb_prv;
                else
                    h->h_end            = hbh->hb_prv;
                hisbuf_destroy (hbh);
                h->h_nitems--;
                hbh = hbt;
                hbx = x->h_start;
            }
            else
            {
                hbt = hbx->hb_nxt;
                if (hbx->hb_prv != NULL)
                    hbx->hb_prv->hb_nxt = hbx->hb_nxt;
                else
                    x->h_start          = hbx->hb_nxt;
                if (hbx->hb_nxt != NULL)
                    hbx->hb_nxt->hb_prv = hbx->hb_prv;
                else
                    x->h_end            = hbx->hb_prv;
                hisbuf_destroy (hbx);
                x->h_nitems--;
                if ((hbx = hbt) == NULL)
                {
                    hbh = hbh->hb_nxt;
                    hbx = x->h_start;
                }
            }
        }
        else
        {
            if ((hbx = hbx->hb_nxt) == NULL)
            {
                hbh = hbh->hb_nxt;
                hbx = x->h_start;
            }
        }
    }

#if defined (DEBUG)
    if ( sqsh_debug_show (DEBUG_HISTORY) )
    {
        fprintf (stdout, "history_merge: Available entries in original list %d\n", h->h_nitems);
        fprintf (stdout, "history_merge: Available entries in merge list %d\n", x->h_nitems);
        for (hbx = x->h_end; hbx != NULL; hbx = hbx->hb_prv)
        {
            line = hbx->hb_buf;
            while ((nl = strchr (line, '\n' )) != NULL)
            {
                if (line == hbx->hb_buf) {
                    sprintf (hdrinfo, "(%d) ", hbx->hb_nbr);
                    fprintf (stdout, "%s%*.*s\n", hdrinfo, (int) (nl - line), (int) (nl - line), line);
                } else {
                    fprintf (stdout, "%*s%*.*s\n", (int) strlen (hdrinfo), " ", (int) (nl - line), (int) (nl - line), line);
                }
                line = nl + 1;
            }
            if ( *line != '\0' )
                fprintf (stdout, "     %s\n", line);
        }
    }
#endif

    /*
     * The remaining entries of x should be relinked into h
     * order by hb_dttm.
     */
    for (hbx = x->h_start; hbx != NULL; hbx = x->h_start)
    {
        /*
         * Unlink the first node from x.
        */
        x->h_start = hbx->hb_nxt;
        if (hbx->hb_nxt != NULL)
            hbx->hb_nxt->hb_prv = NULL;
        else
           x->h_end = NULL;
        x->h_nitems--;

        /*
         * Link this node into h based on value of dttm in MRU-LRU order.
        */
        for (hbh = h->h_start; hbh != NULL && hbh->hb_dttm > hbx->hb_dttm; hbh = hbh->hb_nxt);

        if (hbh != NULL)
        {
            if (hbh->hb_prv != NULL)
                hbh->hb_prv->hb_nxt = hbx;
            else
                h->h_start          = hbx;
            hbx->hb_nxt = hbh;
            hbx->hb_prv = hbh->hb_prv;
            hbh->hb_prv = hbx;
        }
        else
        {
            if (h->h_end != NULL)
                h->h_end->hb_nxt = hbx;
            else
                h->h_start       = hbx;
            hbx->hb_prv = h->h_end;
            h->h_end    = hbx;
            hbx->hb_nxt = NULL;
        }
        ++h->h_nitems;
    }

    /*
     * Renumber the list.
    */
    switch (h->h_nitems)
    {
        case 0 :
            h->h_next_nbr = 1;
            break;
        case 1 :
            h->h_start->hb_nbr = 1;
            h->h_next_nbr      = 2;
            break;
        default :
            for (hbh = h->h_start, i = h->h_nitems, h->h_next_nbr = i + 1;
                 hbh != NULL; hbh->hb_nbr = i--, hbh = hbh->hb_nxt);
            break;
    }

    /*
     * If there are more entries than the allowed size of the list then
     * unlink the oldest entries from the list and destroy them.
    */
    while (h->h_size < h->h_nitems)
    {
        hbh = h->h_end;
        if (hbh->hb_prv != NULL)
        {
            h->h_end         = hbh->hb_prv;
            h->h_end->hb_nxt = NULL;
        }
        else
            h->h_end = h->h_start = NULL;
        hisbuf_destroy (hbh);
        --h->h_nitems;
    }

    return True;
}

