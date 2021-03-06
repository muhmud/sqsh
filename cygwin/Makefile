#  $Id: Makefile,v 1.3 2013/02/25 10:54:25 mwesdorp Exp $
#
#  Makefile for sqsh on Cygwin
#

#  Fix the SYBASE_OCOS entry below, copy "config.h" to the
#  src directory, then cd to the src directory and run
#       make -f ../cygwin/Makefile
#  and everything **should** work.


#  The following list of sqshrc files will be executed by sqsh upon
#  startup.  This allows for system-wide defaults to be set in a 
#  common directory. If you don't want a global sqshrc just comment 
#  out SQSHRC_GLOBAL, below.

SQSHRC_GLOBAL = /usr/local/etc/sqshrc
SQSHRC_PATH   = "$(SQSHRC_GLOBAL):$${HOME}/.sqshrc"

#  The following variables are configurable by the user.  Typically
#  these include things that GNU autoconf has a little trouble
#  figuring out for itself.

SYBASE_OCOS   = c:/sybase/OCS-12_5
SYBASE_INCDIR = -I$(SYBASE_OCOS)/include
#SYBASE_LIBDIR = -L$(SYBASE_OCOS)/lib

#
#  The following set of CT-LIB libraries were determined automatically
#  by 'configure'.  For most systems configure looks up the required
#  libraries by looking at the name of the OS (although this doesn't
#  mean it got them right), however if the line below ends with the
#  word "Guess", then 'configure' didn't have an entry for your operating
#  system and it took a best guess to figure out which libraries you
#  need. In either case, there may be problems, so look this line over
#  and if it doesn't work, compare it to the libraries located in 
#  $SYBASE/samples/ctlibrary.
#
#  The listings below show suggested libraries for Operating Systems
#  that frequently fail to be recognized by 'configure':
#
#  SCO:   -lblk -lct -lcs -lcomn -ltcl -ltli -lnsl_s -lintl -m -lsocket 
#  Dynix: -lblk -lct -lcs -lcomn -ltcl -ltli -lnsl -lintl -lm -lseq
#
#SYBASE_LIBS   = -L$(SYBASE)/OCS-12_5/dll -lblk -lcs -lct  
#SYBASE_LIBS = libblk.lib libcs.lib libct.lib
SYBASE_LIBS = ../cygwin/libblk.a ../cygwin/libcs.a ../cygwin/libct.a

#
#  If you have the GNU readline library available, uncomment the
#  following definitions and make sure the lib directory and
#  include directories are correct.
#
DEF_READLINE    = -DUSE_READLINE -DOLD_READLINE
READLINE_LIBDIR = # -L/lib
READLINE_INCDIR = # -I/usr/include

#
#  On most systems, -lreadline and -lcurses is enough to link with
#  the readline library. However on a few systems you may need to
#  link with termcap rather than curses, and on other systems (such
#  as certain Solaris installations), you may need to link in -lucb
#  to get around some BSD specifics dealing with the termcap library.
#
READLINE_LIBS   = -lreadline -ltermcap

#
#  Motif Support - The `configure' motif support logic for sqsh 
#  isn't exactly perfected yet. If --with-motif was run, the
#  following what `configure' guessed at for the location
#  for your Motif includes and libs.  Note that if MOTIF_INCDIR
#  and MOTIF_LIBDIR are commented out, then the X_INCDIR and
#  X_LIBDIR will be used, below.
#
#  Note that on some systems -lXpm may need to be added to
#  MOTIF_LIBS.
#
DEF_MOTIF    = # -DUSE_MOTIF
MOTIF_INCDIR = 
MOTIF_LIBDIR = 
MOTIF_LIBS   = 

#
#  X Windows Support
#
#  The following variables configure whether or not X windows
#  support is compiled into sqsh (this allows a result set to
#  be displayed in a separate window).  If '--with-x' was sup-
#  plied to 'configure' then most of the variables should be
#  filled in for you (read INSTALL).  If '--with-x' was not
#  supplied or '--without-x' was supplied then The following
#  lines should be commented out.
#
DEF_X11      = # -DUSE_X11
X_INCDIR     = # -I/usr/X11R6/include
X_LIBDIR     = # -L/usr/X11R6/lib
X_LIBS       =  $(MOTIF_LIBS) # -lXaw -lXt -lXext -lXmu -lX11

#
#  Undefine the following line to turn off bcp support in sqsh. This
#  feature allows a result set to be redirected (bcp'ed) to another
#  server.  Since this is a new feature, if you get compile errors on
#  cmd_bcp.c, mail me a copy of the error message(s) (gray@voicenet.com)
#  and uncomment this line.
#
#DEF_NOBCP       = -DNO_BCP

#
#  As of sqsh-1.4, the $password value will never expand to a clear-text
#  copy of the users password.  If you wish to keep the old behavior
#  (which did show the clear-text password), uncomment the following
#  define.  Note that this applies to the new $lock password as well.
#
#DEF_INSECURE   = -DINSECURE

#
#  The following define turns on a work-around for a rather pernicious
#  bug in CT-Lib having to do with the way that async i/o signals are
#  delivered.  If, while running sqsh, pipes tend to lose data or don't
#  display anything at all, like:
#
#        1> sp_who
#        2> go | more
#        1>
#
#  you probably have the bug.  First, PLEASE e-mail me (gray@voicenet.com)
#  and I'll give you a description of what is going on so you can call
#  tech support: I am trying to get them to fix the problem, but they 
#  are moving at a snails pace.  Then, when you have done that, un-
#  comment the following line, which will attempt to install a work-
#  around for the problem.
#
#  Please, do not uncomment this line unless you have reproduced the
#  behavior described above.
#
#DEF_BUGFIX   = -DCTLIB_SIGPOLL_BUG

#
#  Uncommenting the following line turns on debugging support. Turning
#  this feature on will increase the size of sqsh by about 12K, and
#  will probably slow it down nominally. When enabled, various debugging
#  messages may be turned on and off using the -l flag or the $debug
#  environment variable.  There is probably no reason to enable this
#  unless you are debugging a problem or are interested in what is
#  going on under the hood.
#
DEF_DEBUG       = -DDEBUG

#
#  End of user configuration section.
#

prefix         = /usr/local
src_dir        = .
exec_prefix    = ${prefix}
bin_dir        = $(exec_prefix)/bin
inc_dir        = $(prefix)/include
lib_dir        = $(exec_prefix)/lib
man_src        = $(src_dir)/doc
man_dir        = $(prefix)/man

INSTALL        = $(src_dir)/autoconf/install-sh
INSTALL_PROG   = $(INSTALL) -c
INSTALL_DIR    = $(INSTALL) -d
INSTALL_DATA   = $(INSTALL) -c -m 644
INSTALL_MAN    = $(src_dir)/autoconf/install-man

CC             = gcc
DEBUG          = 
DEFINES        = -DSQSH_RC='${SQSHRC_PATH}' $(DEF_READLINE) \
                 $(DEF_NOBCP) $(DEF_DEBUG) $(DEF_INSECURE) $(DEF_X11) \
                 $(DEF_MOTIF) $(DEF_BUGFIX)  
INCLUDE_DIRS   = $(X_INCDIR) $(MOTIF_INCDIR) $(SYBASE_INCDIR) $(READLINE_INCDIR)
LIB_DIRS       = $(X_LIBDIR) $(MOTIF_INCDIR) $(SYBASE_LIBDIR) $(READLINE_LIBDIR)
CFLAGS         = -g -O2 $(DEBUG) $(DEFINES) $(INCLUDE_DIRS) 
CPPFLAGS       = -D_MSC_VER=800 -D_CYGWIN32_ 
LDFLAGS        = $(DEBUG) $(LIB_DIRS)
LIBS           = $(SYBASE_LIBS) $(X_LIBS) $(READLINE_LIBS) 

# The follow define information about the components that make up
# the actual program.

TARGET         = sqsh

CMDS           =  \
	cmd_alias.o cmd_bcp.o cmd_buf.o cmd_connect.o cmd_do.o \
	cmd_echo.o cmd_exit.o cmd_for.o cmd_func.o cmd_go.o \
	cmd_help.o cmd_history.o cmd_if.o cmd_input.o cmd_jobs.o \
	cmd_kill.o cmd_lock.o cmd_loop.o cmd_misc.o cmd_read.o \
	cmd_reconnect.o cmd_redraw.o cmd_reset.o cmd_return.o cmd_rpc.o \
	cmd_set.o cmd_shell.o cmd_show.o cmd_sleep.o cmd_wait.o \
	cmd_warranty.o cmd_while.o

DISPLAYS       = \
	dsp.o dsp_bcp.o dsp_conv.o dsp_desc.o dsp_horiz.o \
	dsp_html.o dsp_meta.o dsp_none.o dsp_out.o dsp_pretty.o \
	dsp_vert.o dsp_x.o

VARS           = \
	var_ctlib.o var_date.o var_debug.o var_dsp.o var_hist.o \
	var_misc.o var_passwd.o var_readline.o var_thresh.o

CORE           = \
	sqsh_alias.o sqsh_args.o sqsh_avl.o sqsh_buf.o sqsh_cmd.o \
	sqsh_compat.o sqsh_debug.o sqsh_env.o sqsh_error.o \
	sqsh_expand.o sqsh_fd.o sqsh_filter.o sqsh_fork.o sqsh_func.o \
	sqsh_getopt.o sqsh_global.o sqsh_history.o sqsh_init.o \
	sqsh_job.o sqsh_readline.o sqsh_sig.o sqsh_sigcld.o sqsh_stdin.o \
	sqsh_strchr.o sqsh_tok.o sqsh_varbuf.o

OBJS = $(CMDS) $(VARS) $(DISPLAYS) $(CORE)

MAN_PAGES      = sqsh.1
SRCS           = $(OBJS:.o=.c)
HEADERS        = 

$(TARGET) : $(OBJS) sqsh_main.o
	$(CC) $(LDFLAGS) $(OBJS) sqsh_main.o $(LIBS) -o $@

sqsh_test : $(OBJS) sqsh_test.o
	$(CC) $(LDFLAGS) $(OBJS) sqsh_test.o $(LIBS) -o $@

sig_test : sqsh_debug.o sqsh_error.o sqsh_sig.c
	$(CC) $(LDFLAGS) $(DEF_DEBUG) -DSIG_TEST sqsh_debug.o sqsh_error.o \
	                                         sqsh_sig.c -o sig_test

clean :
	rm -f *.o $(TARGET) sqsh_test sig_test

realclean : clean
	rm -f config.cache config.log config.status

distclean : realclean
	rm -f Makefile config.h core

# 
# The following absolutely disgusting list of dependancies was
# automatically generated via 'gcc -MM'
#
cmd_alias.o: cmd_alias.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_bcp.o: cmd_bcp.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_getopt.h sqsh_sig.h cmd.h
cmd_buf.o: cmd_buf.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_error.h sqsh_getopt.h sqsh_buf.h sqsh_stdin.h cmd.h
cmd_connect.o: cmd_connect.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_getopt.h \
 sqsh_init.h sqsh_stdin.h cmd.h
cmd_do.o: cmd_do.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h sqsh_buf.h \
 sqsh_readline.h sqsh_getopt.h sqsh_stdin.h cmd.h cmd_misc.h \
 cmd_input.h
cmd_echo.o: cmd_echo.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_exit.o: cmd_exit.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_cmd.h sqsh_avl.h cmd.h sqsh_varbuf.h
cmd_for.o: cmd_for.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h sqsh_buf.h \
 sqsh_readline.h sqsh_getopt.h sqsh_stdin.h cmd.h cmd_misc.h \
 cmd_input.h
cmd_func.o: cmd_func.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h \
 sqsh_getopt.h sqsh_buf.h sqsh_stdin.h sqsh_readline.h cmd.h \
 cmd_misc.h cmd_input.h
cmd_go.o: cmd_go.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_getopt.h sqsh_buf.h \
 sqsh_filter.h sqsh_stdin.h cmd.h cmd_misc.h
cmd_help.o: cmd_help.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h cmd.h
cmd_history.o: cmd_history.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_if.o: cmd_if.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h sqsh_getopt.h \
 sqsh_buf.h sqsh_stdin.h sqsh_readline.h cmd.h cmd_misc.h cmd_input.h
cmd_input.o: cmd_input.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h \
 sqsh_readline.h sqsh_stdin.h cmd.h cmd_misc.h cmd_input.h
cmd_jobs.o: cmd_jobs.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_kill.o: cmd_kill.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_lock.o: cmd_lock.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_cmd.h sqsh_avl.h sqsh_global.h sqsh_env.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h cmd.h
cmd_loop.o: cmd_loop.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_getopt.h \
 sqsh_readline.h sqsh_stdin.h cmd.h cmd_misc.h cmd_input.h
cmd_misc.o: cmd_misc.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_expand.h sqsh_error.h cmd.h \
 cmd_misc.h
cmd_read.o: cmd_read.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_getopt.h \
 sqsh_readline.h sqsh_stdin.h cmd.h
cmd_reconnect.o: cmd_reconnect.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_redraw.o: cmd_redraw.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_cmd.h sqsh_avl.h cmd.h sqsh_varbuf.h
cmd_reset.o: cmd_reset.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_return.o: cmd_return.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_cmd.h sqsh_avl.h sqsh_env.h sqsh_global.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_rpc.o: cmd_rpc.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_getopt.h sqsh_sig.h \
 sqsh_stdin.h cmd.h
cmd_set.o: cmd_set.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_shell.o: cmd_shell.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_varbuf.h sqsh_global.h sqsh_env.h \
 sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_show.o: cmd_show.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_sleep.o: cmd_sleep.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_cmd.h sqsh_avl.h cmd.h sqsh_varbuf.h
cmd_wait.o: cmd_wait.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_stdin.h cmd.h
cmd_warranty.o: cmd_warranty.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h cmd.h
cmd_while.o: cmd_while.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_expand.h sqsh_error.h sqsh_sig.h \
 sqsh_getopt.h sqsh_buf.h sqsh_stdin.h sqsh_readline.h sqsh_tok.h \
 cmd.h cmd_misc.h cmd_input.h
dsp.o: dsp.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_error.h sqsh_sig.h
dsp_bcp.o: dsp_bcp.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h
dsp_conv.o: dsp_conv.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h dsp.h
dsp_desc.o: dsp_desc.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_horiz.o: dsp_horiz.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_html.o: dsp_html.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_meta.o: dsp_meta.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h dsp.h
dsp_none.o: dsp_none.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_out.o: dsp_out.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h
dsp_pretty.o: dsp_pretty.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_vert.o: dsp_vert.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h
dsp_x.o: dsp_x.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h
sqsh_alias.o: sqsh_alias.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_alias.h sqsh_varbuf.h sqsh_avl.h
sqsh_args.o: sqsh_args.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_args.h
sqsh_avl.o: sqsh_avl.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_avl.h
sqsh_buf.o: sqsh_buf.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_stdin.h sqsh_buf.h
sqsh_cmd.o: sqsh_cmd.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_avl.h sqsh_cmd.h
sqsh_compat.o: sqsh_compat.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_sig.h
sqsh_debug.o: sqsh_debug.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h
sqsh_env.o: sqsh_env.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_env.h
sqsh_error.o: sqsh_error.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h
sqsh_expand.o: sqsh_expand.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_varbuf.h sqsh_env.h sqsh_global.h \
 sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_fd.h sqsh_expand.h \
 sqsh_strchr.h sqsh_sig.h
sqsh_fd.o: sqsh_fd.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_sigcld.h sqsh_env.h sqsh_global.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h \
 dsp.h sqsh_func.h sqsh_error.h sqsh_fd.h
sqsh_filter.o: sqsh_filter.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_fd.h sqsh_sig.h sqsh_varbuf.h sqsh_error.h \
 sqsh_filter.h
sqsh_fork.o: sqsh_fork.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_fork.h
sqsh_func.o: sqsh_func.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_avl.h sqsh_func.h
sqsh_getopt.o: sqsh_getopt.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_varbuf.h sqsh_global.h sqsh_env.h \
 sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_getopt.h
sqsh_global.o: sqsh_global.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h
sqsh_history.o: sqsh_history.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_history.h sqsh_varbuf.h
sqsh_init.o: sqsh_init.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_expand.h \
 sqsh_readline.h sqsh_stdin.h sqsh_init.h cmd.h var.h alias.h
sqsh_job.o: sqsh_job.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_fd.h sqsh_tok.h sqsh_varbuf.h \
 sqsh_cmd.h sqsh_avl.h sqsh_global.h sqsh_env.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h \
 sqsh_fork.h sqsh_expand.h sqsh_strchr.h sqsh_getopt.h sqsh_sig.h
sqsh_main.o: sqsh_main.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_error.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_getopt.h \
 sqsh_init.h sqsh_fd.h sqsh_readline.h sqsh_expand.h sqsh_sig.h \
 sqsh_stdin.h cmd.h
sqsh_readline.o: sqsh_readline.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_global.h sqsh_env.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h sqsh_stdin.h \
 sqsh_readline.h
sqsh_sig.o: sqsh_sig.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_sig.h
sqsh_sigcld.o: sqsh_sigcld.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_sig.h sqsh_sigcld.h
sqsh_stdin.o: sqsh_stdin.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_sigcld.h sqsh_env.h sqsh_global.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_stdin.h
sqsh_strchr.o: sqsh_strchr.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_strchr.h sqsh_error.h
sqsh_test.o: sqsh_test.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_varbuf.h sqsh_filter.h
sqsh_tok.o: sqsh_tok.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_error.h sqsh_tok.h sqsh_varbuf.h
sqsh_varbuf.o: sqsh_varbuf.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_varbuf.h sqsh_error.h
var_ctlib.o: var_ctlib.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h sqsh_fd.h var.h
var_date.o: var_date.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h sqsh_global.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h var.h
var_debug.o: var_debug.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h sqsh_global.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h var.h
var_dsp.o: var_dsp.c sqsh_config.h config.h sqsh_compat.h sqsh_debug.h \
 sqsh_global.h sqsh_env.h sqsh_cmd.h sqsh_avl.h sqsh_job.h sqsh_args.h \
 sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h sqsh_alias.h dsp.h \
 sqsh_func.h sqsh_error.h sqsh_fd.h var.h
var_hist.o: var_hist.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h sqsh_global.h sqsh_cmd.h \
 sqsh_avl.h sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h \
 sqsh_history.h sqsh_alias.h dsp.h sqsh_func.h var.h
var_misc.o: var_misc.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h sqsh_stdin.h var.h
var_passwd.o: var_passwd.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_global.h sqsh_cmd.h sqsh_avl.h \
 sqsh_job.h sqsh_args.h sqsh_sigcld.h sqsh_varbuf.h sqsh_history.h \
 sqsh_alias.h dsp.h sqsh_func.h sqsh_error.h var.h
var_readline.o: var_readline.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h var.h
var_thresh.o: var_thresh.c sqsh_config.h config.h sqsh_compat.h \
 sqsh_debug.h sqsh_env.h sqsh_error.h var.h
