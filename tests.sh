#!/bin/sh

ARCH=`uname -m`

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

echo "yes       : no test."

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

[ "`${ARCH}/arch`" = "`arch`" ] || { echo "arch: fail!"; exit 1; }
echo "arch      : pass."

# hostname

[ "`${ARCH}/hostname`" = "`hostname`" ] || { echo "hostname: fail!"; exit 1; }
echo "hostname  : pass."

# sleep

#${ARCH}/sleep 1s || { echo "sleep failed!"; exit 1; }
#echo "sleep passed."
echo "sleep     : skipped."

# chvt

echo "chvt      : no test."

# reset

${ARCH}/reset >/dev/null || { echo "reset: fail!"; exit 1; }
echo "reset     : pass."

# basename

echo "basename  : no test."

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

echo "nice      : no test."

# renice

echo "renice    : no test."

# dmesg

[ "`${ARCH}/dmesg|tail -n10`" = "`dmesg|tail -n10`" ] || \
	{ echo "dmesg: fail!"; exit 1; }
echo "dmesg     : pass."

# rmdir

echo "rmdir     : no test."

# cat

echo "cat       : no test."

# cmp

echo "cmp       : no test."

# nohup

echo "nohup     : no test."

# tee

echo "tee       : no test."

# env

[ "`${ARCH}/env -i FOO=test printenv`" = "FOO=test" ] || \
	{ echo "env: fail!"; exit 1; }
echo "env       : pass."

echo
echo "... All tests passed!"
exit 0
