RTGE - game engine
===

* Install externan packages:

```console
yay -S conan
```

* Build

```console
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ../
cmake --build .
cd ..
```
