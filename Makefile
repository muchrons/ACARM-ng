.PHONY: all
all: build

NCPUS:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
LOCAL_MAKE_FLAGS:=-C src --no-print-directory -j "$(NCPUS)"

F_PATH_STRIP=$(shell echo "$1" | sed 's://\+:/:g')

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
	@echo ' $(call F_PATH_STRIP,$(INSTALL_DIR)/$(DOCDIR))     '
	@echo
	@echo ' data base preparation scripts are placed under:   '
	@echo ' $(call F_PATH_STRIP,$(INSTALL_DIR)/$(MODELDIR))   '
	@echo
	@echo '---------------------------------------------------'
	@echo

.PHONY: install-libs
install-libs: ensure-configure
	@echo "installing libraries: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(LIBDIR))"
	@install -d '$(INSTALL_DIR)/$(LIBDIR)'
	@install $(INSTSTRIP) -m 644 '$(BUILD_DIR)/libs'/*.so '$(INSTALL_DIR)/$(LIBDIR)'

.PHONY: install-bin
install-bin: ensure-configure
	@echo "installing binaries: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(BINDIR))"
	@install -d '$(INSTALL_DIR)/$(BINDIR)'
	@# generic script for running applications
	@echo '#!/bin/sh' > '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@echo 'cd "$(SYSCONFDIR)/acarm-ng/"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@echo 'export LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:$(EXTRA_LIB_DIRS)"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@echo 'APP="$$1"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@echo 'shift' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@echo 'exec "$(BINDIR)/$$APP" "$$@"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@chmod 755 '$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner'
	@# ACARM-ng
	@echo '#!/bin/sh' > '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@echo 'exec "$(INSTALL_DIR)/$(BINDIR)/acarm-app-runner" "acarm-ng.bin" "$$@"' >> '$(INSTALL_DIR)/$(BINDIR)/acarm-ng'
	@install $(INSTSTRIP) -m 755 '$(BUILD_DIR)/acarmng/acarmng.out' '$(INSTALL_DIR)/$(BINDIR)/acarm-ng.bin'
	@# log splitter application
	@install $(INSTSTRIP) -m 755 '$(BUILD_DIR)/logsplitter/logsplitter.out' '$(INSTALL_DIR)/$(BINDIR)/acarm-logsplitter'

.PHONY: install-includes
install-includes: ensure-configure
	@echo "installing headers: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(INCLUDEDIR))"
	@install -d '$(INSTALL_DIR)/$(INCLUDEDIR)/acarm-ng'
	@cp -rL $(BUILD_DIR)/includes/* '$(INSTALL_DIR)/$(INCLUDEDIR)/acarm-ng/'

.PHONY: install-config
install-config: $(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml ensure-configure
	@echo "installing conf-dir: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(SYSCONFDIR))"
	@install -d '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng'

$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml: configure-output.mk doc/example_configs/minimal.xml
	@echo "installing config file: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml)"
	@install -d '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng'
	@install -d '$(INSTALL_DIR)/$(LOCALSTATEDIR)/log/acarm-ng'
	@install -m 600 -b 'doc/example_configs/minimal.xml' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml'
	@sed -i 's:\(<pluginsDir>\).*\(</pluginsDir>\):\1$(PLUGINSDIR)\2:g' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml'
	@sed -i 's:\(<output>\).*\(</output>\):\1$(LOCALSTATEDIR)/log/acarm-ng/daemon.log\2:g' '$(INSTALL_DIR)/$(SYSCONFDIR)/acarm-ng/main_config.xml'

.PHONY: install-doc
install-doc: ensure-configure
	@echo "installing documentation: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(DOCDIR))"
	@install -d '$(INSTALL_DIR)/$(DOCDIR)'
	@cp -rL doc/* '$(INSTALL_DIR)/$(DOCDIR)'
	@# install example init.d script
	@install -d '$(INSTALL_DIR)/$(DOCDIR)/init.d'
	@install -m 755 -b 'src/.misc/init.d/acarm-ng' '$(INSTALL_DIR)/$(DOCDIR)/init.d/acarm-ng'
	@sed -i -e 's:^RUNLOG=".*"$$:RUNLOG="$(LOCALSTATEDIR)/log/acarm-ng/run.log":' \
	        -e 's:^DAEMON=".*"$$:DAEMON="$(BINDIR)/acarm-ng":' \
	        '$(INSTALL_DIR)/$(DOCDIR)/init.d/acarm-ng'

.PHONY: install-db-schemas
install-db-schemas: ensure-configure
	@echo "installing data base schemas: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(MODELDIR))"
	@install -d '$(INSTALL_DIR)/$(MODELDIR)'
	@cp -rL data_model/* '$(INSTALL_DIR)/$(MODELDIR)'

.PHONY: install-plugins
install-plugins: install-libs ensure-configure
	@echo "installing plugins: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(PLUGINSDIR))"
	@install -d '$(INSTALL_DIR)/$(PLUGINSDIR)'
	@for f in '$(INSTALL_DIR)/$(LIBDIR)/'libinput?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libfilter?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libtrigger?*.so \
	          '$(INSTALL_DIR)/$(LIBDIR)/'libpersistency?*.so ; \
	do \
	  install $(INSTSTRIP) -m 644 "$$f" "$(INSTALL_DIR)/$(PLUGINSDIR)/`basename "$$f" | sed 's:\.so$$:.acmp:'`" ; \
	  rm "$$f" ; \
	done
	@# moving back two unfortunate names, that must be placed in the libraries directory
	@install $(INSTSTRIP) -m 644 "$(INSTALL_DIR)/$(PLUGINSDIR)/libpersistencystubs.acmp" "$(INSTALL_DIR)/$(LIBDIR)/libpersistencystubs.so"
	@install $(INSTSTRIP) -m 644 "$(INSTALL_DIR)/$(PLUGINSDIR)/libfilterhostcommon.acmp" "$(INSTALL_DIR)/$(LIBDIR)/libfilterhostcommon.so"

.PHONY: install-wui
install-wui: ensure-configure
	@echo "installing Web User Interface: $(call F_PATH_STRIP,$(INSTALL_DIR)/$(LOCALSTATEDIR)/www/acarm-ng)"
	@install -d '$(INSTALL_DIR)/$(LOCALSTATEDIR)/www/acarm-ng'
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
