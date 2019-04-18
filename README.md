RTGE - game engine
===

* Install externan packages:

```console
sudo pip install conan
```

* Build

```console
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO ../
cmake --build .
cd ..
```
