#!/bin/sh

PACKAGE="libidmef 1.0.3"

(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have automake installed to compile $PACKAGE";
	echo;
	exit;
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have autoconf installed to compile $PACKAGE";
	echo;
	exit;
}

echo "Generating configuration files for $PACKAGE, please wait...."
echo;

aclocal $ACLOCAL_FLAGS;
autoheader;
automake --add-missing -c;
autoconf;
libtoolize --force -c;

# this configure is adjusted for FC3, taken from /usr/lib/rpm/macros
#  ./configure CFLAGS="$CFLAGS -g -O0 -Wall" \
  ./configure \
	--host=i686-pc-linux-gnu --build=i686-pc-linux-gnu \
        --target=i686-redhat-linux-gnu \
        --prefix=/usr \
        --exec-prefix=/usr \
        --bindir=/usr/bin \
        --sbindir=/usr/sbin \
        --sysconfdir=/usr/etc \
        --datadir=/usr/share \
        --includedir=/usr/include \
        --libdir=/usr/lib \
        --libexecdir=/usr/libexec \
        --localstatedir=/usr/var \
        --sharedstatedir=/usr/com \
        --mandir=/usr/man \
        --infodir=/usr/info \
        $@

