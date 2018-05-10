DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

CFLAGS = -std=c++11 -O3 -Wall -Wpedantic -Wextra `root-config --cflags` -g3
LDFLAGS = `root-config --libs`
INCLUDE = src
DIRS = $(sort $(dir $(wildcard src/*/.)))
SRCS = $(foreach DIR,$(DIRS),$(wildcard $(DIR)*.cpp))  src/Main.cpp
OBJECTS = $(addsuffix .o,$(basename $(SRCS)))

$(foreach folder,$(subst src,.d,$(DIRS)), $(shell mkdir -p $(folder) > /dev/null))

src/Simulation: $(OBJECTS)
	g++ $(CFLAGS) -o Simulation $(OBJECTS) $(LDFLAGS)
	
.PHONY: $(DIRS) clean deps

deps:
	@echo $(SRCS)	
	@echo
	@echo $(basename $(SRCS))
	@echo
	@echo $(patsubst src/%,$(DEPDIR)/%.d,$(basename $(SRCS)))
	@echo
	@echo $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

clean: 
	rm -f $(OBJECTS)
	rm -f Simulation
	rm -f $(DEPDIR)/**/*.d

.SECONDEXPANSION:

$(OBJECTS): src/%.o: src/%.cpp $(DEPDIR)/%.d
	g++ -c $(CFLAGS) $(DEPFLAGS) -I $(INCLUDE) -o $@ $<
	$(POSTCOMPILE)
	
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(patsubst src/%,$(DEPDIR)/%.d,$(basename $(SRCS)))
