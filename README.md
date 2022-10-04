## Differences with jsc-android-buildscripts

Current version is 0.0.1, it is unstable and work-in-progress

- ICU and Webkit sources forked and embedded in repo
  - ICU 71.1 official distr
  - WebKit 7615.1.7.1 (tag: WebKit-7615.1.7.1, or canonical: 254623.2@safari-7615.1.7-branch)
- All sources built with -Oz and LTO
- DFG JIT enabled
- It use static libc++ and expose only C API, it makes used NDK maximally independed from other components. So please use
  latest NDK, if possible (tested with NDK 25.1.8937393).

## Install

android/build.gradle
```
  maven {
      // Android JSC is installed from npm
      // Replace this:
      // url("$rootDir/../node_modules/jsc-android/dist")
      // to:
      url("$rootDir/../node_modules/@vovkasm/jsc-android/dist")
  }

  ...

  mavenCentral {
      // We don't want to fetch react-native from Maven Central as there are
      // older versions over there.
      content {
          excludeGroup "com.facebook.react"
          excludeGroup "org.webkit" // <--- add this line
      }
  }
```

android/app/build.gradle
```
    if (enableHermes) {
        //noinspection GradleDynamicVersion
        implementation("com.facebook.react:hermes-engine:+") { // From node_modules
            exclude group:'com.facebook.fbjni'
        }
    } else {
      //noinspection GradleDynamicVersion
        implementation 'org.webkit:android-jsc:+' // <--- use this!
    }
```

## Building from source

Define ANDROID_NDK_HOME env var.

```
npm run clean
npm run native
npm run dist
```

## Update Webkit

1. Clone [webkit repo](https://github.com/WebKit/webkit.git)
2. Switch to required branch
3. From jsc-android root directory: `./import-webkit.sh <path-to-webkit>`
4. (may be hard) Reapply all patches
  - git format-patch, then git am, or maybe rebase/cherry-pick
