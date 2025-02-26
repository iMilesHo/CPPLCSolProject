CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Iinclude -I/opt/homebrew/include
LD_FLAGS  := -L/opt/homebrew/lib -lgtest -lgtest_main -pthread

# Collect all problem directories
PROBLEM_DIRS := $(wildcard problems/*/*)

# For each directory, produce an executable:
#   <dir>/test_<basename-of-dir>
# e.g.:
#   problems/arrays/0001_Two_Sum/test_0001_Two_Sum
TEST_TARGETS := $(foreach d,$(PROBLEM_DIRS),$(d)/test_$(notdir $(d)))

.PHONY: all run_all clean

# --- Default target: build all test executables ---
all: $(TEST_TARGETS)

# Template for building each problem’s test
define build_test
$1/test_$(notdir $1): $1/Solution.cpp $1/test.cpp
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
# Pattern rules for building and running one problem by ID
# Example usage:
#   make build_0001   -> builds the matching "0001_*" test
#   make run_0001     -> builds + runs the matching "0001_*" test
#   make clean_0001   -> removes the matching "0001_*" test executable
#

# --- Pattern rule: "make build_XXXX" ---
build_%:
	@PROBLEM_DIRS="$(wildcard problems/*/$*_*)"; \
	if [ -z "$$PROBLEM_DIRS" ]; then \
	  echo "ERROR: No problem directory found matching '*$*_*'."; \
	  exit 1; \
	fi; \
	COUNT=$$(echo "$$PROBLEM_DIRS" | wc -w); \
	if [ $$COUNT -gt 1 ]; then \
	  echo "ERROR: Multiple directories match '*$*_*':"; \
	  echo "$$PROBLEM_DIRS"; \
	  exit 1; \
	fi; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	$(MAKE) $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Pattern rule: "make run_XXXX" ---
run_%: build_%
	@PROBLEM_DIRS="$(wildcard problems/*/$*_*)"; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	echo "Running $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)"; \
	./$$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Pattern rule: "make clean_XXXX" ---
# Removes just the test executable for "XXXX_*".
clean_%:
	@PROBLEM_DIRS="$(wildcard problems/*/$*_*)"; \
	if [ -z "$$PROBLEM_DIRS" ]; then \
	  echo "ERROR: No problem directory found matching '*$*_*'."; \
	  exit 1; \
	fi; \
	COUNT=$$(echo "$$PROBLEM_DIRS" | wc -w); \
	if [ $$COUNT -gt 1 ]; then \
	  echo "ERROR: Multiple directories match '*$*_*':"; \
	  echo "$$PROBLEM_DIRS"; \
	  exit 1; \
	fi; \
	PROBLEM_DIR=$$(echo "$$PROBLEM_DIRS"); \
	echo "Removing $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)"; \
	rm -f $$PROBLEM_DIR/test_$$(basename $$PROBLEM_DIR)

# --- Clean all test executables ---
clean:
	rm -f $(TEST_TARGETS)
