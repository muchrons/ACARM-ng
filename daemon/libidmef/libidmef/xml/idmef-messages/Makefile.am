## Process this file with automake to produce Makefile.in

EXTRA_DIST = README \
             example1.xml.in \
             example2.xml.in \
             example3.xml.in \
             example4.xml.in \
             example5.xml.in \
             example6.xml.in \
             example7.xml.in \
             example8.xml.in \
             example9.xml.in \
             example10.xml.in \
             example11.xml.in \
	     example12.xml.in \
             example13.xml.in \
             example14.xml.in \
             example15.xml.in \
             example16.xml.in \
             example17.xml.in \
             example18.xml.in \
	     example20.xml.in

data_DATA = example1.xml \
             example2.xml \
             example3.xml \
             example4.xml \
             example5.xml \
             example6.xml \
             example7.xml \
             example8.xml \
             example9.xml \
             example10.xml \
             example11.xml \
	     example12.xml \
             example13.xml \
             example14.xml \
             example15.xml \
             example16.xml \
             example17.xml \
             example18.xml \
             example20.xml

CLEANFILES = $(data_DATA)

datadir = $(inst_idmef_messages_dir)

SUFFIXES = .xml .in

%.xml: %.xml.in
	@$(SED) -e "s|@inst_dtd_dir@|$(inst_idmef_dtd_dir)|" $< > $@
