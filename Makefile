.PHONY: all
all: build

NCPUS:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
LOCAL_MAKE_FLAGS:=-C src --no-print-directory -j "$(NCPUS)"

-include configure-output.mk

PLUGINSDIR:=$(DATADIR)/acarm-ng/plugins
MODELDIR:=$(DATADIR)/acarm-ng/data_model

.PHONY: build
build: build-core build-plugins

.PHONY: build-core
build-core: ensure-configure
	@echo "BUILDING ACARM-ng"
	@$(MAKE) $(LOCAL_MAKE_FLAGS) acarmng logsplitter

.PHONY: build-plugins
build-plugins: ensure-configure
	@for pl in $(PLUGINS_LIST) ; \
	do \
	  echo "BUILDING PLUGIN $$pl" ; \
	  $(MAKE) $(LOCAL_MAKE_FLAGS) "$$pl" || exit $$? ;\
	done

.PHONY: install
install: install-plugins install-libs install-bin install-includes install-config install-doc install-wui install-db-schemas install-wui
	@echo
	@echo '---------------------------------------------------'
	@echo
	@echo '           INSTALLATION SUCCESSFUL                 '
	@echo
	@echo ' to proceed with configuration read documentation: '
	@echo ' $(INSTALL_DIR)/$(DOCDIR)                          '
	@echo
	@echo ' data base preparation scripts are placed under:   '
	@echo ' $(INSTALL_DIR)/$(MODELDIR)                        '
	@echo
	@echo '---------------------------------------------------'
	@echo

.PHONY: install-libs
install-libs: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(LIBDIR)'
	install -v -m 755 '$(BUILD_DIR)/libs'/*.so '$(INSTALL_DIR)/$(LIBDIR)'

.PHONY: install-bin
install-bin: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(BINDIR)'
	@echo '#!/bin/sh' > '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@echo 'cd "$(SYSCONFDIR)/acarm-ng/"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@echo 'export LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:$(EXTRA_LIB_DIRS)"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@echo 'exec "$(BINDIR)/acarm-ng.bin" "$$@"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@chmod 755 '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@install -v -m 755 '$(BUILD_DIR)/acarmng/acarmng.out' '$(INSTALL_DIR)/$(BINDIR)/acarm-ng.bin'
	@install -v -m 755 '$(BUILD_DIR)/logsplitter/logsplitter.out' '$(INSTALL_DIR)/$(BINDIR)/acarm-logsplitter'

.PHONY: install-includes
install-includes: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(INCLUDEDIR)/acarm-ng'
	@cp -rL $(BUILD_DIR)/includes/* '$(INSTALL_DIR)/$(INCLUDEDIR)/acarm-ng/'

.PHONY: install-config
install-config: $(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/acarm_ng_config.xml ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng'
	@install -v -d '$(INSTALL_DIR)/$(SYSCONFDIR)/init.d'
	@install -v -m 755 -b 'src/.misc/init.d/acarm_ng' '$(INSTALL_DIR)/$(SYSCONFDIR)/init.d/acarm_ng'
	@sed -i -e 's:^PREFIX=".*"$$:PREFIX="$(PREFIX)":' '$(INSTALL_DIR)/$(SYSCONFDIR)/init.d/acarm_ng'


$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/acarm_ng_config.xml: configure-output.mk doc/example_configs/minimal.xml
	@install -v -d '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng'
	@install -v -d '$(INSTALL_DIR)/$(LOCALSTATEDIR)/log/acarm-ng'
	@install -v -m 600 -b 'doc/example_configs/minimal.xml' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/acarm_ng_config.xml'
	@sed -i 's:\(<pluginsDir>\).*\(</pluginsDir>\):\1$(PLUGINSDIR)\2:g' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/acarm_ng_config.xml'
	@sed -i 's:\(<output>\).*\(</output>\):\1$(LOCALSTATEDIR)/log/acarm-ng/daemon.log\2:g' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/acarm_ng_config.xml'

.PHONY: install-doc
install-doc: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(DOCDIR)'
	@cp -rL doc/* '$(INSTALL_DIR)/$(DOCDIR)'

.PHONY: install-db-schemas
install-db-schemas: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(MODELDIR)'
	@cp -rL data_model/* '$(INSTALL_DIR)/$(MODELDIR)'

.PHONY: install-plugins
install-plugins: install-libs ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(PLUGINSDIR)'
	@for f in '$(INSTALL_DIR)/$(LIBDIR)/'libinput?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libfilter?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libtrigger?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libpersistency?*.so ; \
	do \
	  install -v -m 755 "$$f" "$(INSTALL_DIR)/$(PLUGINSDIR)/`basename "$$f" | sed 's:\.so$$:.acmp:'`" ; \
	  rm "$$f" ; \
	done
	@install -v -m 755 "$(INSTALL_DIR)/$(PLUGINSDIR)/libpersistencystubs.acmp" "$(INSTALL_DIR)/$(LIBDIR)/libpersistencystubs.so"
	@install -v -m 755 "$(INSTALL_DIR)/$(PLUGINSDIR)/libfilterhostcommon.acmp" "$(INSTALL_DIR)/$(LIBDIR)/libfilterhostcommon.so"

.PHONY: install-wui
install-wui: ensure-configure
	@install -v -d '$(INSTALL_DIR)/$(LOCALSTATEDIR)/www/acarm-ng'
	@cp -r wui/site/* '$(INSTALL_DIR)/$(LOCALSTATEDIR)/www/acarm-ng/'


.PHONY: enusre-configure
ensure-configure: configure-output.mk
configure-output.mk: configure Makefile
	@echo "run ./confiugure first" >&2
	@false

.PHONY: clean
clean:
	@rm -fv  configure-output.mk configure.log
	@rm -fvr src/gen
