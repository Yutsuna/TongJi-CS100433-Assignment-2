CXX      := g++
CXXFLAGS := -std=gnu++26 -freflection -Wall -Wextra -Wpedantic -Werror -Wconversion -IInclude
DEPFLAGS := -MMD -MP
LDLIBS   := -lm -lglm
LDLIBS_Ex7 := -lglfw -lGL

SRC   := Source
BUILD := Build
LIB   := $(BUILD)/Lib/libLib.a

LIB_OBJS := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(wildcard $(SRC)/Lib/*.cpp))
EXES := $(filter-out Lib,$(notdir $(patsubst %/,%,$(dir $(wildcard $(SRC)/*/Main.cpp)))))

.DEFAULT_GOAL := all
.SUFFIXES:
.DELETE_ON_ERROR:
MAKEFLAGS += -r

all: $(EXES)

$(LIB): $(LIB_OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

define EX_RULE
$(1)_OBJS := $$(patsubst $$(SRC)/%.cpp,$$(BUILD)/%.o,$$(wildcard $$(SRC)/$(1)/*.cpp))
$(1): $$($(1)_OBJS) $$(LIB)
	$$(CXX) $$(CXXFLAGS) $$(LDFLAGS) -o $$@ $$($(1)_OBJS) $$(LIB) $$(LDLIBS) $$(LDLIBS_$(1))
endef
$(foreach ex,$(EXES),$(eval $(call EX_RULE,$(ex))))

-include $(shell find $(BUILD) -name '*.d' 2>/dev/null)

clean:
	$(RM) -r $(BUILD)

fclean: clean
	$(RM) $(EXES)

re: fclean all

format:
	clang-format -i $(shell find $(SRC) $(INC) -type f \( -name '*.cpp' -o -name '*.hpp' \))

.PHONY: all clean fclean re format
