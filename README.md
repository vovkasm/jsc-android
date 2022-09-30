## How to build

Define ANDROID_NDK_HOME env var.

```
cmake --preset android-arm64
cmake --build build/arm64-v8a
cmake --install build/arm64-v8a
```