#!/bin/sh

# This update compile_commands.json for clangd

cmake --preset=compile-commands
cp build/compile-commands/compile_commands.json ./ 