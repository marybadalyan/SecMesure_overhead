
## 🧪 Stack Protector Benchmark (CMake-Based)

This project benchmarks the performance overhead of **stack protection (`-fstack-protector`)** using CMake to build two versions of the same executable:

* One **with** stack protection enabled.
* One **without** stack protection.

This helps quantify the trade-off between **security** and **performance** in performance-critical code.

---

### 📄 Benchmark Description

The benchmark runs two tests:

1. **`tightLoop()`**

   * Uses a stack-allocated buffer and `strcpy()`, making it a target for stack protection.
2. **`callDummy()`**

   * A CPU-bound loop that simply increments a value.

```cpp
void tightLoop() {
    for (int i = 0; i < 100000; ++i) {
        char buffer[100];                        // 🧠 Stack allocation
        strcpy(buffer, "This can overflow");     // 💥 Risky operation (triggers stack protection)
    }
}
```

```cpp
void callDummy() {
    for (int i = 0; i < 100000000; ++i) {
        dummy(); // Simple CPU-bound logic
    }
}
```

---

### 🏗️ Building with CMake

#### ✅ Project structure:

```
/project-root
├── CMakeLists.txt
└── benchmark.cpp
```

#### ✅ Example `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(StackProtectionBenchmark CXX)

add_executable(benchmark_protected benchmark.cpp)
add_executable(benchmark_unprotected benchmark.cpp)

if (MSVC)
    target_compile_options(benchmark_protected PRIVATE /GS)
    target_compile_options(benchmark_unprotected PRIVATE /GS-)  # Disable stack protection
else()
    target_compile_options(benchmark_protected PRIVATE -fstack-protector-strong)
    target_compile_options(benchmark_unprotected PRIVATE -fno-stack-protector)
endif()
```

#### 🔧 Build commands:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

This will produce:

* `benchmark_protected` — with stack protection
* `benchmark_unprotected` — without it

---

### 🚀 Running the Benchmark

```bash
./main
./main_unprotected
```

Sample output:

```
Tight loop executed.
Execution time: 23.41 ms
Execution time: 132.14 ms
```

You should see:

* `tightLoop()` is **slightly slower** with stack protection.
* `callDummy()` stays **almost unchanged** across both versions.

---

### 📊 Takeaway Table

| Function      | Uses Stack? | Triggers Stack Protector       | Slower With Protection? |
| ------------- | ----------- | ------------------------------ | ----------------------- |
| `tightLoop()` | ✅ Yes       | ✅ Yes (due to buffer + strcpy) | ✅ Yes                   |
| `callDummy()` | ❌ No        | ❌ No                           | ❌ No (negligible diff)  |

---

### ⚠️ Security vs Performance

Stack protection is a **crucial defense** against buffer overflows. Disabling it **should only be done in trusted environments** for benchmarking, embedded systems, or extremely performance-sensitive routines.

---

### ✅ Summary

This project demonstrates how to:

* Use **CMake** to build both protected and unprotected binaries.
* Benchmark performance overhead of stack protection mechanisms.
* Evaluate when the security trade-off may be justifiable.
