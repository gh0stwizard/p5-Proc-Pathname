#!/bin/sh

APPNAME=$1
STRIP="none" #ppi"
LINKTYPE="static" #allow-dynamic"
SP_FILE=${HOME}/staticperl
BOOT_FILE="${APPNAME}.pl"

# "eat" $1
shift

${SP_FILE} mkapp ${APPNAME} --boot ${BOOT_FILE} \
-Mstrict \
-Mwarnings \
-MEnv \
-MConfig \
-MConfig_heavy.pl \
-MProc::Pathname \
--strip ${STRIP} \
--${LINKTYPE} \
--usepacklists \
$@
