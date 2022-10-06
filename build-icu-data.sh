#!/bin/sh

SCRIPT=$(readlink -f "$0")
ROOT=$(dirname "$SCRIPT")

ICU_DATA_NAME=icudt71l
ICU_SOURCE_DIR=$ROOT/icu/source
ICU_BINARY_DIR=$ROOT/build/icu-host

mkdir -p $ICU_BINARY_DIR
pushd $ICU_BINARY_DIR
$ICU_SOURCE_DIR/configure --enable-release=yes --enable-rpath=yes --enable-tests=no --enable-samples=no --with-data-packaging=archive
make -j8

cp $ICU_BINARY_DIR/data/out/$ICU_DATA_NAME.dat $ROOT/android-library/android-jsc/src/main/assets/