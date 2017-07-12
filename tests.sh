#!/bin/sh

ARCH=`uname -m`

# check for Arch Linux
if [ -e /etc/arch-release ]; then
	CMD_ARCH="uname -m"
else
	CMD_ARCH="arch"
fi

echo "Executing tests ..."
echo

# true

${ARCH}/true || { echo "true: fail!"; exit 1; }
echo "true      : pass."

# false

${ARCH}/false
[ $? -eq 1 ] || { echo "false: fail!"; exit 1; }
echo "false     : pass."

# yes

[ "`${ARCH}/yes|head -n1`" = "y" -a "`${ARCH}/yes a b|head -n1`" = "a b" ] || \
	{ echo "yes: fail!"; exit 1; }
echo "yes       : pass."

# clear

${ARCH}/clear >/dev/null || { echo "clear: fail!"; exit 1; }
echo "clear     : pass."

# echo

ECHO1=`${ARCH}/echo -n -e "a\\\y\xyy\x20a\ca"`
ECHO2=`/bin/echo -n -e    "a\\\y\xyy\x20a\ca"`
[ "$ECHO1" = "$ECHO2" ] || { echo "echo: fail!"; exit 1; }
echo "echo      : pass."

# printenv

[ "`${ARCH}/printenv PATH`" = "`printenv PATH`" ] || \
	{ echo "printenv: fail!"; exit 1; }
echo "printenv  : pass."

# pwd

[ "`${ARCH}/pwd`" = "`pwd`" ] || { echo "pwd: fail!"; exit 1; }
echo "pwd       : pass."

# uname

[ "`${ARCH}/uname`" = "`uname`" ] || { echo "uname: fail!"; exit 1; }
echo "uname     : pass."

# arch

[ "`${ARCH}/arch`" = "`${CMD_ARCH}`" ] || { echo "arch: fail!"; exit 1; }
echo "arch      : pass."

# hostname

[ "`${ARCH}/hostname`" = "`hostname`" ] || { echo "hostname: fail!"; exit 1; }
echo "hostname  : pass."

# sleep

${ARCH}/sleep 0 0s || { echo "sleep: fail!"; exit 1; }
echo "sleep     : pass."

# chvt

${ARCH}/chvt 1
if [ $? -eq 0 ]; then
	echo "chvt: fail!"
	exit 1
fi
echo "chvt      : pass."

# reset

${ARCH}/reset >/dev/null || { echo "reset: fail!"; exit 1; }
echo "reset     : pass."

# basename

[ "`${ARCH}/basename inc/stdio.h .h`" = "`basename inc/stdio.h .h`" ] || \
	{ echo "basename: fail!"; exit 1; }
echo "basename  : pass."

# chroot

echo "chroot    : no test."

# domainname

[ "`${ARCH}/domainname`" = "`domainname`" ] || \
	{ echo "domainname: fail!"; exit 1; }
echo "domainname: pass."

# dirname

echo "dirname   : no test."

# mesg

echo "mesg      : no test."

# sync

#${ARCH}/sync || { echo "sync failed!"; exit 1; }
#echo "sync passed."
echo "sync      : skipped."

# nice

${ARCH}/nice -n 10 sleep 5 &
PID=$!
[ `ps -p $PID -o ni|tail -n1|tr -d " "` -eq 10 ] || \
	{ echo "nice: fail!"; exit 1; }
kill $PID
echo "nice      : pass."

# renice

sleep 5 &
PID=$!
${ARCH}/renice -n 10 $PID
[ `ps -p $PID -o ni|tail -n1|tr -d " "` -eq 10 ] || \
	{ echo "renice: fail!"; exit 1; }
kill $PID
echo "renice    : pass."

# dmesg

[ "`${ARCH}/dmesg|tail -n10`" = "`dmesg|tail -n10`" ] || \
	{ echo "dmesg: fail!"; exit 1; }
echo "dmesg     : pass."

# rmdir

DIR1=`mktemp -d`
DIR2=`mktemp -d`/
mkdir -p $DIR1
mkdir -p $DIR2
${ARCH}/rmdir $DIR1 $DIR2
[ ! -d $DIR1 -a ! -d $DIR2 ] || { echo "rmdir: fail!"; exit 1; }
[ -d $DIR1 ] && rmdir $DIR1
[ -d $DIR2 ] && rmdir $DIR2
echo "rmdir     : pass."

# cat

FILE1=`mktemp`
FILE2=`mktemp`
${ARCH}/cat -v /bin/true >$FILE1
cat -v /bin/true >$FILE2
cmp -s $FILE1 $FILE2
[ $? -eq 0 -a "`echo foo|${ARCH}/cat`" = "foo" ] || { echo "cat: fail!"; exit 1; }
rm -f $FILE1 $FILE2
echo "cat       : pass."

# cmp

FILE1=`mktemp`
FILE2=`mktemp`
</bin/true ${ARCH}/cmp -l - /bin/false|tr -d " ">$FILE1
</bin/true cmp -l - /bin/false|tr -d " ">$FILE2
cmp -s $FILE1 $FILE2 || { echo "cmp: fail!"; exit 1; }
rm -f $FILE1 $FILE2
echo "cmp       : pass."

# nohup

echo "nohup     : no test."

# tee

FILE=`mktemp`
TEE=`echo -n foo|${ARCH}/tee -i $FILE`
[ "`cat $FILE`" = "foo" -a "$TEE" = "foo" ] || \
	{ echo "tee: fail!"; exit 1; }
rm -f $FILE
echo "tee       : pass."

# env

[ "`${ARCH}/env -i FOO=test printenv`" = "FOO=test" ] || \
	{ echo "env: fail!"; exit 1; }
echo "env       : pass."

echo
echo "... All tests passed!"
exit 0
