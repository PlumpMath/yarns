cc=clang
cxx=llvm-g++
lib_cflags=-O4 -DNDEBUG -pipe -Wall -arch i386
bin_cflags=-O0 -pipe -Wall -arch i386
ldflags=-arch i386 -L.
lto=1
asflags=
context=i386
context_asm=1
as=as
strip=1
