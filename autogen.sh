#! /bin/sh
#
# (C) 2016 by Argonne National Laboratory.
#     See LICENSE.txt in top-level directory.
#

########################################################################
## Command line arguments
########################################################################

RELEASE_MODE=N

while [ $# -gt 0 ]; do
    case "$1" in
        -r) RELEASE_MODE=Y;;
        *)  break;;
    esac
    shift
done


########################################################################
## Utility functions
########################################################################

recreate_tmp() {
    rm -rf .tmp
    mkdir .tmp 2>&1 >/dev/null
}

warn() {
    echo "===> WARNING: $@"
}

error() {
    echo "===> ERROR:   $@"
}

echo_n() {
    # "echo -n" isn't portable, must portably implement with printf
    printf "%s" "$*"
}


########################################################################
## Version number
########################################################################

# Update the below when releasing a new version
BOLT_VERSION=1.0b1

# Produce a numeric version assuming the following format:
# Version: [MAJ].[MIN].[REV][EXT][EXT_NUMBER]
# Example: 1.0.7rc1 has
#          MAJ = 1
#          MIN = 0
#          REV = 7
#          EXT = rc
#          EXT_NUMBER = 1
#
# Converting to numeric version will convert EXT to a format number:
#          ALPHA (a) = 0
#          BETA (b)  = 1
#          RC (rc)   = 2
#          PATCH (p) = 3
# Regular releases are treated as patch 0
#
# Numeric version will have 1 digit for MAJ, 2 digits for MIN,
# 2 digits for REV, 1 digit for EXT and 2 digits for EXT_NUMBER.
V1=`expr $BOLT_VERSION : '\([0-9]*\)\.[0-9]*\.*[0-9]*[a-zA-Z]*[0-9]*'`
V2=`expr $BOLT_VERSION : '[0-9]*\.\([0-9]*\)\.*[0-9]*[a-zA-Z]*[0-9]*'`
V3=`expr $BOLT_VERSION : '[0-9]*\.[0-9]*\.*\([0-9]*\)[a-zA-Z]*[0-9]*'`
V4=`expr $BOLT_VERSION : '[0-9]*\.[0-9]*\.*[0-9]*\([a-zA-Z]*\)[0-9]*'`
V5=`expr $BOLT_VERSION : '[0-9]*\.[0-9]*\.*[0-9]*[a-zA-Z]*\([0-9]*\)'`

if test "$V2" -le 9 ; then V2="0$V2" ; fi
if test "$V3" = "" ; then V3="0"; fi
if test "$V3" -le 9 ; then V3="0$V3" ; fi
if test "$V4" = "a" ; then
    V4=0
elif test "$V4" = "b" ; then
    V4=1
elif test "$V4" = "rc" ; then
    V4=2
elif test "$V4" = "" ; then
    V4=3
    V5=0
elif test "$V4" = "p" ; then
    V4=3
fi
if test "$V5" -le 9 ; then V5="0$V5" ; fi

BOLT_NUMVERSION=`expr $V1$V2$V3$V4$V5 + 0`


########################################################################
## Release date
########################################################################

if test "$RELEASE_MODE" = "Y"; then
    BOLT_RELEASE_DATE=`date`
    BOLT_RELEASE_DATE="\"$BOLT_RELEASE_DATE\""
else
    BOLT_RELEASE_DATE="\"unreleased development copy\""
fi


########################################################################
## Building the CMakeLists.txt
########################################################################

echo_n "Updating the CMakeLists.txt ..."

INPUT_FILE=runtime/CMakeLists.txt.in
OUTPUT_FILE=runtime/CMakeLists.txt

if [ -f $INPUT_FILE ] ; then
    sed -e "s/%BOLT_VERSION%/${BOLT_VERSION}/g" $INPUT_FILE | \
    sed -e "s/%BOLT_NUMVERSION%/${BOLT_NUMVERSION}/g" | \
    sed -e "s/%BOLT_RELEASE_DATE%/${BOLT_RELEASE_DATE}/g" > $OUTPUT_FILE
    echo "done"
else
    echo "error"
    error "$INPUT_FILE file not present, unable to update version number (perhaps we are running in a release tarball source tree?)"
fi

