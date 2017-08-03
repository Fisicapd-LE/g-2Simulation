DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

CFLAGS = -std=c++11 -O3
INCLUDE = src
DIRS = $(sort $(dir $(wildcard src/*/.)))
SRCS = $(foreach DIR,$(DIRS),$(wildcard $(DIR)*.cpp))  src/Main.cpp src/Configuration.cpp
OBJECTS = $(addsuffix .o,$(basename $(SRCS)))

src/Simulation: $(DIRS) src/Main.o src/Configuration.o
	g++ $(CFLAGS) -o Simulation $(OBJECTS)
	
.PHONY: $(DIRS) clean

clean: 
	rm -f $(OBJECTS)
	rm -f Simulation
	rm -f $(DEPDIR)/**/*.d

.SECONDEXPANSION:

$(DIRS): src/%: $$(addsuffix .o,$$(basename $$(wildcard src/%*.cpp)))

$(OBJECTS): src/%.o: src/%.cpp $(DEPDIR)/%.d
	g++ -c $(CFLAGS) $(DEPFLAGS) -I $(INCLUDE) -o $@ $<
	$(POSTCOMPILE)
	
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
