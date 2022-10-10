#!/bin/sh
if [ -d build ]; then
    rm build -rf
fi
mkdir build && cd ./build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/tool.cmake -DWITH_RKNN=ON ..
make -j32
