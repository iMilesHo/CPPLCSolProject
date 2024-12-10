Below is a concise guide to setting up a local C++ and GoogleTest project on an M-series MacBook Pro using VSCode and CMake:

---

**Prerequisites:**
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

**Directory Structure:**
```
project/
├─ CMakeLists.txt
├─ include/
│  └─ Solution.h
├─ src/
│  └─ Solution.cpp
└─ tests/
   └─ test_solution.cpp
```

- `CMakeLists.txt`:
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(MySolution CXX)

  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS} include)

  add_library(SolutionLib src/Solution.cpp)

  add_executable(runTests tests/test_solution.cpp)
  target_link_libraries(runTests PRIVATE SolutionLib GTest::GTest GTest::Main)

  enable_testing()
  add_test(NAME runTests COMMAND runTests)
  ```

**Building and Running:**
1. Create a build directory:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
   
2. Run tests:
   ```bash
   ./runTests
   ```

**VSCode Setup:**
- Install Extensions:  
  - *C/C++ (Microsoft)*  
  - *CMake Tools (Microsoft)*  

- Open the project folder in VSCode.
- Configure CMake:  
  Press `Cmd+Shift+P` → `CMake: Configure`.
- Build from VSCode:  
  `Cmd+Shift+P` → `CMake: Build`.
- Reload VSCode if needed (`Cmd+Shift+P` → `Developer: Reload Window`).


**Compiler Choice:**

Use Apple’s Clang (the default on macOS) rather than Homebrew’s GCC. Clang is well-integrated with macOS system headers and avoids many compatibility issues.

If you previously installed GCC via Homebrew, ensure you do not force your project to use it. Instead, let CMake detect the default compiler:

```bash
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

If no compilers are specified, CMake will typically pick Clang by default on macOS. Make sure not to set `CC` or `CXX` environment variables to GCC.

You should now have proper IntelliSense, can run and debug your code, and have GoogleTest integrated locally on your M-series MacBook Pro.