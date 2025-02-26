## **Prerequisites:**

- **Xcode Command Line Tools**:  
  Install by running:
  ```bash
  xcode-select --install
  ```
- **Homebrew**:  
  If not installed, follow [Homebrew’s instructions](https://brew.sh).

- **CMake & GoogleTest**:
  ```bash
  brew install cmake googletest
  ```

## **Directory Structure:**

```
.
├── LICENSE
├── Makefile
├── README.md
├── include
│   └── printer.hpp
└── problems
    ├── arrays
    │   ├── 0001_Two_Sum
    │   │   ├── Solution.cpp
    │   │   ├── Solution.h
    │   │   ├── notes.md
    │   │   └── test.cpp
    │   └── 0026_Remove_Duplicates_from_Sorted_Array
    │       ├── Solution.cpp
    │       ├── Solution.h
    │       ├── notes.md
    │       └── test.cpp
    └── strings
        └── 0003_Longest_Substring_Without_Repeating_Characters
            ├── Solution.cpp
            ├── Solution.h
            ├── notes.md
            └── test.cpp
```

## **Building and Running:**

### How to Use

1. **Build All**

   ```bash
   make
   ```

   or

   ```bash
   make all
   ```

2. **Run All**

   ```bash
   make run_all
   ```

3. **Build Single**

   ```bash
   make build_0001
   ```

   This will find exactly one directory, e.g. `problems/arrays/0001_Two_Sum`, and build `test_0001_Two_Sum`.

4. **Run Single**

   ```bash
   make run_0001
   ```

   This will build (if not already built) and then run `test_0001_Two_Sum`.

5. **Clean Single**

   ```bash
   make clean_0001
   ```

   This removes only the test executable for the matching `0001_*` directory—useful if you want to quickly rebuild just one.

6. **Clean All**
   ```bash
   make clean
   ```
   Removes _all_ the test executables for _all_ problems.

## **VSCode Setup:**

- Install Extensions:
  - _C/C++ (Microsoft)_
- Open the project folder in VSCode.
- Reload VSCode if needed (`Cmd+Shift+P` → `Developer: Reload Window`).

## **Trouble shooting:**

Use Apple’s Clang (the default on macOS) rather than Homebrew’s GCC. Clang is well-integrated with macOS system headers and avoids many compatibility issues.

If no compilers are specified, CMake will typically pick Clang by default on macOS. Make sure not to set `CC` or `CXX` environment variables to GCC.
