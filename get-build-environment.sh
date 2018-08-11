#! /bin/bash

# This is a short reference of the commands to set up the build environment
# for compiling and building the project.

# Whether to assume an answer of yes to all questions.  (Used when installing
# packages.)  Set to a non-empty string to run without user interaction.
: ${ASSUME_YES:=""}

# Check for a known package manager:
if [ -f /etc/redhat-release ] && type yum &>/dev/null; then
    PACKAGE_MANAGER="yum"
elif [ -f /etc/debian_version ] && type apt-get &>/dev/null; then
    PACKAGE_MANAGER="apt-get"
else
    echo "No known package manager found (i.e., `yum` on Red Hat, `apt-get` on Debian)!" >&2
    echo "Please install the following manually:" >&2
    echo "  g++ cmake" >&2
    exit 1
fi

if [ $(id -u) != "0" ]; then
	SUDO=sudo
else
	# We're running as root, we don't need sudo:
	SUDO=""
fi

case "$PACKAGE_MANAGER" in
    yum)
        if [ -n "$ASSUME_YES" ]; then
            PACKAGE_MANAGER_ASSUME_YES=-y
        else
            PACKAGE_MANAGER_ASSUME_YES=""
        fi

        $SUDO yum install $PACKAGE_MANAGER_ASSUME_YES gcc-c++ cmake
        ;;
    apt-get)
        if [ -n "$ASSUME_YES" ]; then
            PACKAGE_MANAGER_ASSUME_YES=-y
        else
            PACKAGE_MANAGER_ASSUME_YES=""
        fi

        $SUDO apt-get $PACKAGE_MANAGER_ASSUME_YES install g++ cmake
        ;;
esac
