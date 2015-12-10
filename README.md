# Bilgamesh
Toy problem to test C++11 features and performance.

## Developer Notes

First time use:
```bash
bilgamesh$ autoreconf --install && mkdir build && cd build
```

Subsequent builds:
```bash
build$ (cd .. && autoreconf) && ../configure CXXFLAGS="-g" && make check
```
