# my-c-cpp-playground

This is my C/C++ playgounrd repogitory.

## C language + GoogleTest (CMake)

This project contains a minimal C17 executable and GoogleTest-based tests.
GoogleTest is fetched automatically by CMake using FetchContent.

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run app

```bash
./build/app
```

On Windows with multi-config generators (Visual Studio), run:

```bash
./build/Debug/app.exe
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

On multi-config generators:

```bash
ctest --test-dir build -C Debug --output-on-failure
```
