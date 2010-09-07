## Process this file with automake to produce Makefile.in

INCLUDES = -I../include

lib_LTLIBRARIES = libidmef.la

libidmef_la_LDFLAGS = -version-info @LIBIDMEF_VERSION_INFO@

libidmef_la_SOURCES = create.c \
		      global.c \
		      strings.c \
		      helpers.c \
		      parser.c \
		      ilist.c \
		      message.c

EXTRA_DIST = create.c.in \
	     create.c.tmpl

CLEANFILES = create.c

%.c: %.c.in %.c.tmpl ../libidmef-functions.map
	@list="`$(CAT) ../libidmef-functions.map | $(SED) -e \"s|#.*||\"`"; \
	set -- $$list; \
	while test -n "$$1"; do \
		_func="$$1"; shift; \
		_element="$$1"; shift; \
		_type="$$1"; shift; \
		case "$${_type}" in \
		DOC|NODE) \
		$(SED)  -e "s|@param@|IDMEFPtr|" \
			-e "s|@function@|$${_func}|" \
			-e "s|@element@|$${_element}|" \
			-e "s|@type@|$${_type}|" \
			${@}.tmpl; ;; \
		CONTENT|ATTRIBUTE) \
		$(SED)  -e "s|@param@|const char\*|" \
			-e "s|@function@|$${_func}|" \
			-e "s|@element@|$${_element}|" \
			-e "s|@type@|$${_type}|" \
			${@}.tmpl; ;; \
		esac; \
	done | \
	$(AWK) "/@input@/ {while(getline <\"/dev/stdin\") print \$$0} {print}" \
		$< > $@;
