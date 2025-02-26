# My C++ Data Structures & Algorithms Practice

This repository contains **my personal C++ solutions** to various Data Structure and Algorithm problems (including some from LeetCode). I use it as a **learning playground**—focusing on refining my **C++** skills, exploring **memory management**, and testing algorithmic ideas. It includes a convenient build/test environment with **Google Test** to verify each solution.

## Why This Repository?

- **Personal Study & Practice**
  - Strengthen C++ proficiency, especially modern features like C++17
  - Dive deeper into memory management, containers, and STL usage
- **Quick Testing Environment**
  - Each solution is tested with Google Test (`gtest`), so I can quickly validate correctness for typical and edge cases
- **Reference & Growth**
  - Over time, I revisit solutions to optimize or explore alternate approaches

---

## Prerequisites

0. System Requirements:
   - **macOS**: 10.15 (Catalina) or later
1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```
2. **Homebrew**
   - If not installed, see [Homebrew’s instructions](https://brew.sh).
3. **CMake & GoogleTest**
   ```bash
   brew install cmake googletest
   ```

> **Note**: On macOS (Apple Silicon), I recommend using the system Clang rather than Homebrew’s GCC to avoid library path issues.

---

## Building and Running

### Using VSCode Tasks

If you open this project in Visual Studio Code:

1. Open the Command Palette (`F1` or `Cmd+Shift+P`).
2. Select **Tasks: Run Task**, then pick any of the following:
   - **All: Build**
   - **All: Run**
   - **All: Clean**
   - **Single: Build Problem**
   - **Single: Run Problem**
   - **Single: Clean Problem**

These tasks map directly to the `make` commands described below.

### Using `make` Commands

- **Build All**

  ```bash
  make
  ```

  or

  ```bash
  make all
  ```

- **Run All**

  ```bash
  make run_all
  ```

- **Build Single** (e.g. problem “0001” in `problems/arrays/0001_Two_Sum`):

  ```bash
  make build_0001
  ```

  This compiles `Solution.cpp` and `test.cpp`, producing an executable named `test_0001_Two_Sum` (stored at the top level).

- **Run Single**

  ```bash
  make run_0001
  ```

  Builds (if needed) and then runs `test_0001_Two_Sum`.

- **Clean Single**

  ```bash
  make clean_0001
  ```

  Removes the single test executable (useful if you want to do a fresh rebuild of that problem alone).

- **Clean All**
  ```bash
  make clean
  ```
  Deletes _all_ built test executables.

---

## VSCode Setup

- **Recommended Extension**: [C/C++ (Microsoft)](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- **Reload** if needed: `Cmd+Shift+P` → _Developer: Reload Window_

---

## Current Structure

```
.
├── Makefile
├── problems
│   ├── arrays
│   │   ├── 0001_Two_Sum
│   │   │   ├── Solution.cpp
│   │   │   ├── test.cpp
│   │   │   └── ...
│   │   └── ...
│   └── strings
│       └── 0003_Longest_Substring
│           ├── Solution.cpp
│           ├── test.cpp
│           └── ...
├── include
│   └── printer.hpp
└── ...
```

Each problem subfolder contains:

- **Solution.cpp** / **Solution.h**: My solution
- **test.cpp**: Google Test cases
- **notes.md** (if any): Implementation details, complexity analysis, or personal notes

---

## Contributing

I primarily use this for my own practice. However, if you spot something interesting (or incorrect!) and would like to fix or improve it, feel free to open a Pull Request. I’m always open to learning from other perspectives.

---

## Show Your Support

If you find these solutions or tests useful, please consider **starring** the repository. It helps others discover the project—and motivates me to keep improving and sharing more C++ explorations. Thank you for stopping by!
