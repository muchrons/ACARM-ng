#CONFIGURE_OPTIONS:=--enable-shared --disable-static
CONFIGURE_OPTIONS:=
MAKE_OPTIONS:=
MAKE_INSTALL_OPTIONS:=

pre_configure.dep:
	touch "$@.log"
	@echo "$(COMPONENT_NAME): fixing includes... (log in `readlink -f $@.log`)"
	( find ext_sources -type f -name '*.c' -o -name '*.h' | xargs -r sed -i 's:#include \(.\)idmef_:#include \1libidmef/idmef_:' ) > $@.log 2>&1
	date > "$@"
