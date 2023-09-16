构建一个你自己的 `tff-Lang` 编译和运行器, 你可以参考以下的编译过程:

- 第1步, 构建 `./dist/base.dll`. `./src/base/` 是 `tff-Lang` 的核心实现; `./src/std/` 是标准库实现.

```bash
g++ -std=c++17
    -fconcepts -Wno-return-type
    -shared -fpic
    ./src/base/*.cpp ./src/std/*.cpp
    -I ./include
    -o ./dist/base.dll
```

- 第2步, 并行构建`./dist/analyzer.dll`, `./dist/compiler.dll` 和 `./dist/interpreter.dll`.

```bash
g++ -std=c++17
    -fconcepts -Wno-return-type
    -shared -fpic
    ./src/analyzer.cpp ./dist/base.dll
    -I ./include
    -o ./dist/analyzer.dll

g++ -std=c++17
    -fconcepts -Wno-return-type
    -shared -fpic
    ./src/compiler.cpp ./dist/base.dll
    -I ./include
    -o ./dist/compiler.dll

g++ -std=c++17
    -fconcepts -Wno-return-type
    -shared -fpic
    ./src/interpreter.cpp ./dist/base.dll
    -I ./include
    -o ./dist/interpreter.dll
```

- 第3步, 构建 `./dist/tff.exe`.

```bash
g++ -std=c++17
    -fconcepts -Wno-return-type
    ./src/main.cpp ./dist/*.dll
    -I ./include
    -o ./dist/tff.exe
```