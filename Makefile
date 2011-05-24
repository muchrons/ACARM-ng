.PHONY: all
all: build-system

-include configure-output.mk

configure: configure-output.mk
configure-output.mk:
	@echo "run ./confiugure first" >&2
	@false

.PHONY: clean
clean:
	rm -fv configure-output.mk
	#rm -fv src/gen
