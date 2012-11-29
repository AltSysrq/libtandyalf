#! /bin/sh
PATHS="/lib /usr/lib /usr/local/lib"
LIBRARY="libtandyalf.so"
TC="$1"
shift

for x in $PATHS; do
    if test -f $x/$LIBRARY; then
        LD_PRELOAD="$x/$LIBRARY $LD_PRELOAD"
        break
    fi
done

if test "x$LD_PRELOAD" = "x"; then
    echo "Could not find $LIBRARY, aborting"
    exit 1
fi

export LD_PRELOAD

for comp in `cat <<EOF
addr2line
ar
as
c++
c++filt
cpp
dlltool
dllwrap
elfedit
g++
gcc
gcov
gprof
ld
ld.bfd
nm
objcopy
objdump
ranlib
readelf
size
strings
strip
windmc
windres
EOF`
do
    echo "$TC$comp"
    tccomp=`which "$TC$comp"`
    test "x$tccomp" = "x" && continue
    comp=`echo "$comp" | sed 's/\+/x/g;s/\./_/g'`
    eval "TANDYALF_bin_$comp=$tccomp"
    export TANDYALF_bin_$comp
    eval "TANDYALF_usr_bin_$comp=$tccomp"
    export TANDYALF_usr_bin_$comp
    eval "TANDYALF_usr_local_bin_$comp=$tccomp"
    export TANDYALF_usr_local_bin_$comp
done

exec $*
