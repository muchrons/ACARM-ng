## Process this file with automake to produce Makefile.in

libidmefincdir = $(includedir)/libidmef

libidmefinc_HEADERS = idmef_create.h \
		      idmef_strings.h \
		      idmef_global.h \
		      idmef_types.h \
		      idmef_parser.h \
		      idmef_parser_helpers.h \
		      idmef_list.h \
		      idmef_message.h

EXTRA_DIST = idmef_create.h.in \
	     idmef_global.h.in

CLEANFILES = idmef_create.h \
	     idmef_global.h

SUFFIXES = .in

idmef_global.h: idmef_global.h.in
	@$(SED) -e "s|@inst_dtd_dir@|$(datadir)|" $< > $@

idmef_create.h: idmef_create.h.in ../libidmef-functions.map
#	@list="`$(CAT) ../libidmef-functions.map | $(SED) -e \"s|#.*||\"`"; \
#	set -- $$list; \
#	while test -n "$$1"; do \
#		_func="$$1"; shift; \
#		_element="$$1"; shift; \
#		_type="$$1"; shift; \
#		case "$${_type}" in \
#		DOC|NODE) \
#		$(ECHO) "IDMEFPtr $${_func}(IDMEFPtr first, ...);"; ;; \
#		CONTENT|ATTRIBUTE) \
#		$(ECHO) "IDMEFPtr $${_func}(const char *first, ...);"; ;; \
#		esac; \
#	done | \
#	$(AWK) "/@input@/ {while(getline <\"/dev/stdin\") print \$$0} {print}" \
#		$< > $@;
	perl ./gen.pl
