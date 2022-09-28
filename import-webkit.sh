#!/bin/sh

SCRIPT=$(readlink -f "$0")
ROOT=$(dirname "$SCRIPT")

DEST_DIR=$ROOT/webkit

if [ -d $DEST_DIR ]; then
    echo "Please remove 'webkit' dir before import"
    exit 1
fi

WEBKIT_REPO=$1
if [ -z "$WEBKIT_REPO" -o ! -d "$WEBKIT_REPO/Source/JavaScriptCore" ]; then
    echo "Please provide path to WebKit directory from which it will import"
    exit 1
fi

mkdir "$DEST_DIR"
for d in .editorconfig .gitattributes .gitignore CMakeLists.txt ReadMe.md
do
    cp -R "$WEBKIT_REPO/$d" "$DEST_DIR/"
done

mkdir "$DEST_DIR/Source"
cp $WEBKIT_REPO/Source/*.cmake "$DEST_DIR/Source/"
for d in bmalloc cmake JavaScriptCore WTF .gitignore CMakeLists.txt
do
    cp -R "$WEBKIT_REPO/Source/$d" "$DEST_DIR/Source/"
done

mkdir "$DEST_DIR/Source/ThirdParty"
for d in capstone gtest
do
    cp -R "$WEBKIT_REPO/Source/ThirdParty/$d" "$DEST_DIR/Source/ThirdParty/"
done

echo "Done"