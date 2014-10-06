echo "THIS SCRIPT WILL HAVE NO EFFECT UNLESS IS IS SOURCED."
set -x

unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE
MACHINE=avr

export ARCH="avr"
export CROSS_COMPILE="avr-"
export CC="avr-gcc"
export LD="avr-ld"
export STRIP="avr-strip"
export AS="avr-as"

export KERNEL_CROSS_BUILD_ENVIRONMENT_SOURCED="true"
set +x
echo "Altered environment for cross building with avr tool chain."
