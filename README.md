# OS-251

OS-251 is a synthesizer plugin (vst3, au) made with C++ , [JUCE](https://github.com/juce-framework/JUCE).

<div align="center"><img src="screenshot.png" width="600px"></div>

## Download

You can down load pre-built binaries from [release](https://github.com/utokusa/OS-251/releases).

## Development

### Build

- Download the codes

  ```bash
  git clone --recursive https://github.com/utokusa/OS-251
  ```

- Build the body of the plugin (C++)

  You can build it using CMake.

### Lint

Lint checking with clang-format 11 for C++ is available.
See details by running `./lint.sh -h`.

### Code completion

Use clangd for code completion or go-to-definition. 
`compile_commands.json` (it's necessary for clangd) is automatically generated when you build OS-251.
Use `./run_compdb.sh -h`  if you want to add header information to `compile_commands.json`.
