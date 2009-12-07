#
# this is a very ugly hack that prevents flood of messages from doxygen, since
# prelude C++ binding is not commented at all.
#
html/index.html:: Doxyfile
	@echo "suppressing doxygen warnings (external code NOT commented at all)"
	@cp 'Doxyfile' 'Doxyfile.prev'
	@sed -e 's:^\(WARNINGS \+=\).*:\1 NO:' \
	     -e 's:^\(WARN_IF_UNDOCUMENTED \+=\).*:\1 NO:' \
	     -e 's:^\(WARN_IF_DOC_ERROR \+=\).*:\1 NO:' \
        'Doxyfile' > 'Doxyfile.no_warn' || ( rm -f 'Dexyfile' ; false )
	@mv 'Doxyfile.no_warn' 'Doxyfile'
