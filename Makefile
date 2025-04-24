CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Iinclude -I/opt/homebrew/include
LD_FLAGS  := -L/opt/homebrew/lib -lgtest -lgtest_main -pthread

# Collect all problem directories (new structure: problems/*/*)
PROBLEM_DIRS := $(wildcard problems/*/*)

# For each directory, produce an executable:
#   <dir>/test_<basename-of-dir>
TEST_TARGETS := $(foreach d,$(PROBLEM_DIRS),$(d)/test_$(notdir $(d)))

.PHONY: all run_all clean

# --- Default target: build all test executables ---
all: $(TEST_TARGETS)

# Template for building each problem's test
define build_test
$1/test_$(notdir $1): $1/test.cpp
	$(CXX) $(CXXFLAGS) -o $$@ $$^ $(LD_FLAGS)
endef

# Generate the individual build rules
$(foreach d,$(PROBLEM_DIRS),$(eval $(call build_test,$(d))))

# --- Run all tests after building them ---
run_all: all
	@for exe in $(TEST_TARGETS); do \
	  echo "Running $$exe"; \
	  ./$$exe; \
	  echo ""; \
	done

#
# Pattern rules for building and running one problem by ID (now considering the new directory naming)
# Example usage:
#   make build_0088   -> builds the matching "*_0088_*" test
#   make run_0088     -> builds + runs the matching "*_0088_*" test
#   make clean_0088   -> removes the matching "*_0088_*" test executable
#

# --- Pattern rule: "make build_XXXX" ---
build_%:
	@PROBLEM_DIRS="$$(find problems -type d -name "*_$*_*")"; \
	if [ -z "$$PROBLEM_DIRS" ]; then \
	  echo "ERROR: No problem directory found matching '*_$*_*'."; \
	  exit 1; \
	fi; \
	COUNT=$$(echo "$$PROBLEM_DIRS" | wc -w); \
	if [ $$COUNT -gt 1 ]; then \
	  echo "ERROR: Multiple directories match '*_$*_*':"; \
	  echo "$$PROBLEM_DIRS"; \
	  exit 1; \
	fi; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	$(MAKE) $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Pattern rule: "make run_XXXX" ---
run_%: build_%
	@PROBLEM_DIRS="$$(find problems -type d -name "*_$*_*")"; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	echo "Running $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)"; \
	./$$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Pattern rule: "make clean_XXXX" ---
clean_%:
	@PROBLEM_DIRS="$$(find problems -type d -name "*_$*_*")"; \
	if [ -z "$$PROBLEM_DIRS" ]; then \
	  echo "ERROR: No problem directory found matching '*_$*_*'."; \
	  exit 1; \
	fi; \
	COUNT=$$(echo "$$PROBLEM_DIRS" | wc -w); \
	if [ $$COUNT -gt 1 ]; then \
	  echo "ERROR: Multiple directories match '*_$*_*':"; \
	  echo "$$PROBLEM_DIRS"; \
	  exit 1; \
	fi; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	rm -f $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Clean all test executables ---
clean:
	 rm -f $(TEST_TARGETS)
