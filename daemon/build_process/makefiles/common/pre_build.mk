# include all required features
include $(MAKEFILES_COMMON_BASE_DIR)/version_check.mk

# force doing some extra stuff before building starts
.PHONY: pre_build
pre_build: $(BUILD_PROCESS_VERSION_TIMESTAMP) $(GEN_WORK_ROOT)/features_build_list.txt

# write down list of features buing build in given directory
$(GEN_WORK_ROOT)/features_build_list.txt:
	@echo "$(FEATURES)" | xargs -n 1 echo | sort | uniq > "$@"
