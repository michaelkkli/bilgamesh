# Bilgamesh
Monte Carlo strategy for Checkers on Hacker Rank in C++11.

## Developer Notes

First time use:
```bash
bilgamesh$ autoreconf --install && mkdir build && cd build
```

Subsequent builds:
```bash
build$ (cd .. && autoreconf) && ../configure CXXFLAGS="-g" && make check
```
