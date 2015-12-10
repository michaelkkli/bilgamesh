AC_INIT([bilgamesh], [0.1], [mikeliuk+bgm@gmail.com])
m4_include([ax_cxx_compile_stdcxx_11.m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign nostdinc subdir-objects])
AC_PROG_CXX([clang++])
AX_CXX_COMPILE_STDCXX_11
AC_CHECK_LIB([m],[log])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
