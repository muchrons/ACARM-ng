PUBLIC_HEADERS:=public_headers.$(PROFILE)

.PHONY: all
all:: $(LIBRARY_NAME) $(PUBLIC_HEADERS)

include $(MAKEFILES_COMMON_BASE_DIR)/lib_app_common.mk
