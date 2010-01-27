PUBLIC_HEADERS:=public_headers.$(PROFILE)

.PHONY: all
all:: $(LIBRARY_NAME)

include $(MAKEFILES_COMMON_BASE_DIR)/lib_app_common.mk
