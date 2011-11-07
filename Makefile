.PHONY: all
all: build

NCPUS:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)

LOCAL_MAKE_FLAGS:=-C src --no-print-directory -j "$(NCPUS)"

-include configure-output.mk

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
install: install-plugins install-libs install-bin install-includes install-config install-doc install-wui install-db-schemas
	@echo
	@echo '---------------------------------------------------'
	@echo
	@echo '           INSTALLATION SUCCESSFUL                 '
	@echo
	@echo ' to proceed with configuration read documentation: '
	@echo ' $(INSTALL_DIR)/share/doc                          '
	@echo
	@echo ' data base preparation scripts are placed under:   '
	@echo ' $(INSTALL_DIR)/share/ACARM-ng/data_model          '
	@echo
	@echo '---------------------------------------------------'
	@echo

.PHONY: install-libs
install-libs: ensure-configure
	@install -v -d '$(INSTALL_DIR)/lib/acarm-ng'
	install -v -m 755 '$(BUILD_DIR)/libs'/*.so '$(INSTALL_DIR)/lib/acarm-ng/'

.PHONY: install-bin
install-bin: ensure-configure
	@install -v -d '$(INSTALL_DIR)/bin'
	@echo '#!/bin/sh' > '$(INSTALL_DIR)/bin/acarm-ng'
	@echo 'cd "$(INSTALL_DIR)/etc/acarm-ng/"' >> '$(INSTALL_DIR)/bin/acarm-ng'
	@echo 'export LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:$(EXTRA_LIB_DIRS)"' >> '$(INSTALL_DIR)/bin/acarm-ng'
	@echo 'exec "$(INSTALL_DIR)/bin/acarm-ng.bin"' >> '$(INSTALL_DIR)/bin/acarm-ng'
	@chmod 755 '$(INSTALL_DIR)/bin/acarm-ng'
	@install -v -m 755 '$(BUILD_DIR)/acarmng/acarmng.out' '$(INSTALL_DIR)/bin/acarm-ng.bin'
	@install -v -m 755 '$(BUILD_DIR)/logsplitter/logsplitter.out' '$(INSTALL_DIR)/bin/acarm-logsplitter'

.PHONY: install-includes
install-includes: ensure-configure
	@install -v -d '$(INSTALL_DIR)/include'
	@cp -rL '$(BUILD_DIR)/includes' '$(INSTALL_DIR)/include/acarm-ng'

.PHONY: install-config
install-config: $(INSTALL_DIR)/etc/acarm-ng/acarm_ng_config.xml ensure-configure
	@install -v -d '$(INSTALL_DIR)/etc/acarm-ng'


$(INSTALL_DIR)/etc/acarm-ng/acarm_ng_config.xml: configure-output.mk doc/example_configs/minimal.xml
	@install -v -d '$(INSTALL_DIR)/etc/acarm-ng'
	@install -v -d '$(INSTALL_DIR)/var/log/acarm-ng'
	@install -v -m 600 -b 'doc/example_configs/minimal.xml' '$(INSTALL_DIR)/etc/acarm-ng/acarm_ng_config.xml'
	@sed -i 's:\(<pluginsDir>\).*\(</pluginsDir>\):\1$(INSTALL_DIR)/lib/acarm-ng/plugins\2:g' '$(INSTALL_DIR)/etc/acarm-ng/acarm_ng_config.xml'
	@sed -i 's:\(<output>\).*\(</output>\):\1$(INSTALL_DIR)/var/log/acarm-ng/daemon.log\2:g' '$(INSTALL_DIR)/etc/acarm-ng/acarm_ng_config.xml'

.PHONY: install-doc
install-doc: ensure-configure
	@install -v -d '$(INSTALL_DIR)/share/doc'
	@cp -rL 'doc' '$(INSTALL_DIR)/share/'

.PHONY: install-db-schemas
install-db-schemas: ensure-configure
	@install -v -d '$(INSTALL_DIR)/share/ACARM-ng'
	@cp -rL 'data_model' '$(INSTALL_DIR)/share/ACARM-ng'

.PHONY: install-plugins
install-plugins: install-libs ensure-configure
	@install -v -d '$(INSTALL_DIR)/lib/acarm-ng/plugins'
	@for f in '$(INSTALL_DIR)/lib/acarm-ng/'libinput?*.so \
	          '$(INSTALL_DIR)/lib/acarm-ng/'libfilter?*.so \
	          '$(INSTALL_DIR)/lib/acarm-ng/'libtrigger?*.so \
	          '$(INSTALL_DIR)/lib/acarm-ng/'libpersistency?*.so ; \
	do \
	  install -v -m 755 "$$f" "$(INSTALL_DIR)/lib/acarm-ng/plugins/`basename "$$f" | sed 's:\.so$$:.acmp:'`" ; \
	done

.PHONY: install-wui
install-wui: ensure-configure
	@install -v -d '$(INSTALL_DIR)/var/www'
	@cp -r 'wui/site' '$(INSTALL_DIR)/var/www/acarm-ng'


.PHONY: enusre-configure
ensure-configure: configure-output.mk
configure-output.mk: configure Makefile
	@echo "run ./confiugure first" >&2
	@false

.PHONY: clean
clean:
	@rm -fv  configure-output.mk configure.log
	@rm -fvr src/gen
