# has C sources?
ifneq (,$(CDEPS))
-include $(CDEPS)
endif

# has C++ sources?
ifneq (,$(CXXDEPS))
-include $(CXXDEPS)
endif

