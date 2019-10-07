#!/bin/bash

g++ -fPIC -c -I../vstsdk2.4 -D__cdecl="" svf-vst.cpp
g++ -fPIC -c -I../vstsdk2.4 -D__cdecl="" svf.cpp
g++ -fPIC -c -I../vstsdk2.4 -D__cdecl="" vstplugmain.cpp
g++ -fPIC -c -I../vstsdk2.4 -D__cdecl="" audioeffectx.cpp
g++ -fPIC -c -I../vstsdk2.4 -D__cdecl="" audioeffect.cpp
g++ -shared -Wl,-soname,again.so -o svf.so *.o
