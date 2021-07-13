# OS-251
OS-251 is a synthesizer plugin (vst3, au) made with C++ , JUCE and react-juce.

This plugin is currently under development.

<div align="center"><img src="screenshot.png" width="600px"></div>

## Download

You can down load pre-built binaries from [release](https://github.com/utokusa/OS-251/releases).

## Development

### Build

You can build OS-251 using CMake

### Lint

Lint checking for both C++ and Node.js is available.
See details by running `./lint.sh -h`.

### Code completion

Use clangd for code completion or go-to-definition. 
`compile_commands.json` (it's necessary for clangd) is automatically generated when you build OS-251.
Use `./run_compdb.sh -h`  if you want to add header information to `compile_commands.json`.
