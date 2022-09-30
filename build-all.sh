#!/bin/sh

for preset in android-arm64 android-arm android-x86 android-x86_64
do
    echo "Configure ${preset}"
    cmake --preset $preset -DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME
done

for arch in arm64-v8a armeabi-v7a x86 x86_64
do
    echo "Build & Install ${arch}"
    cmake --build build/$arch
    cmake --install build/$arch
done

echo "Done"