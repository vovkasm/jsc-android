## Differences with jsc-android-buildscripts

- ICU and Webkit sources forked and embedded in repo
  - ICU 71.1 official distr
  - WebKit 7615.1.7.1 (tag: WebKit-7615.1.7.1, or canonical: 254623.2@safari-7615.1.7-branch)
- All sources built with -Oz and LTO
- DFG JIT enabled

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
