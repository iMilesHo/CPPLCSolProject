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
- Reload VSCode if needed (`Cmd+Shift+P` → `Developer: Reload Window`).
- Configure CMake:  
  Press `Cmd+Shift+P` → `CMake: Configure`.
- Build from VSCode:  
  `Cmd+Shift+P` → `CMake: Build`.
- Run from VSCode:  
  `Cmd+Shift+P` → `CMake: Run tests`.



**Trouble shooting:**

Use Apple’s Clang (the default on macOS) rather than Homebrew’s GCC. Clang is well-integrated with macOS system headers and avoids many compatibility issues.

If we previously installed GCC via Homebrew, ensure we do not force wer project to use it. Instead, let CMake detect the default compiler:

```bash
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

If no compilers are specified, CMake will typically pick Clang by default on macOS. Make sure not to set `CC` or `CXX` environment variables to GCC.