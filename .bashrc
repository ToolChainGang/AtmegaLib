# .bashrc
#
# Config file for the bash shell
#
######################################################################

export WORKDIR="$PROJECTDIR/src"
export EDITFILES="../Notes.txt test/WS2812Test.c"
export NEMODIR="$WORKDIR"
export BUILDDIR="$WORKDIR/../build"

export BINDIR="$BUILDDIR/bin"
export TESTEXEC="LED5050Test"
export TESTARGS=""

alias tst="cd $BUILDDIR/src/test"

alias pe="avrdude -p m328p -c dragon_isp -P usb -e -U flash:w:class.hex"
