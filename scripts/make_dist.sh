#!/bin/sh

if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null
then
	if (echo -n testing; echo 1,2,3) | sed s/-n/xn/ | grep xn >/dev/null
	then
		ac_n= ac_c='
' ac_t=' '
	else
		ac_n=-n ac_c= ac_t=
	fi
else
	ac_n= ac_c='\c' ac_t=
fi         

#
# Determine the version we are compiling.
#
version=`grep SQSH_VERSION src/sqsh_config.h | sed -e 's,[^0-9.],,g'`

echo
echo " ===================== BINARY DISTRIBUTION ====================="
echo
echo "Please enter a relatively short name for the operating system"
echo "you are compiling for, and any other indicators of build type"
echo "that you would like to include the resulting tar file. The"
echo "description you enter will become part of the name of the tar file"
echo "generated by this script.  For example, the name 'solaris-8-static'"
echo "will create a file called sqsh-$version-solaris-8-static.tar.gz"
echo
echo $ac_n "Description string: $ac_c"
read desc

tar_file="sqsh-$version-$desc.tar.gz"

#
# Creating a working directory.
#
tmp_dir=sqsh-$version
mkdir $tmp_dir
mkdir $tmp_dir/doc
mkdir $tmp_dir/scripts

#
# Place binary in root directory
#
cp src/sqsh $tmp_dir
chmod gou+rx $tmp_dir/sqsh

#
# Place install script in root directory
#
cp scripts/install.sh $tmp_dir
chmod gou+rx $tmp_dir/install.sh

#
# Basic readme files.
#
cp COPYING doc/README.bin $tmp_dir

#
# Documentation files.
#
cp ChangeLog doc/FAQ doc/global.sqshrc doc/sample.sqsh_m4 \
   doc/sqsh.1 doc/sqsh.html doc/sqshrc-2.4 doc/CHANGES-2.* \
	$tmp_dir/doc

#
# Scripts
#
cp scripts/wrapper.sh.in $tmp_dir/scripts

echo " ==== GENERATING $tar_file ===="
tar cvf - $tmp_dir | gzip -9 -c > "$tar_file"
rm -fr $tmp_dir
