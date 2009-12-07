#
# this is a very ugly hack that prevents flood of messages from doxygen, since
# prelude C++ binding is not commented at all.
#
html/index.html:: Doxyfile
	@echo "suppressing doxygen for preludepp (code is NOT commented at all)"
	@cp 'Doxyfile' 'Doxyfile.prev'
	@sed 's:^\(EXCLUDE_PATTERNS \+=\).*:\1 *.mt.?pp *.t.?pp *.?pp:' \
        'Doxyfile.prev' > 'Doxyfile' || ( rm -f 'Dexyfile' ; false )
