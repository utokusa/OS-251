# OS-251
OS-251 is a synthesizer plugin (vst3, au) made with C++ , [JUCE](https://github.com/juce-framework/JUCE) and [react-juce](https://github.com/nick-thompson/react-juce).

<div align="center"><img src="screenshot.png" width="600px"></div>

## Download

You can down load pre-built binaries from [release](https://github.com/utokusa/OS-251/releases).

## Development

### Build

- Download the codes

  ```bash
  git clone --recursive https://github.com/utokusa/OS-251
  ```

- Build the UI (Node.js and [react-juce](https://github.com/nick-thompson/react-juce))
  Use Node.js version 14.

  ```bash
  cd ./src/jsui
  npm ci
  npm run build
  ```

- Build the body of the plugin (C++)

  You can build it using CMake.

### Lint

Lint checking for both C++ and Node.js is available.
For C++, we use clang-format 11.
See details by running `./lint.sh -h`.

### Code completion

Use clangd for code completion or go-to-definition. 
`compile_commands.json` (it's necessary for clangd) is automatically generated when you build OS-251.
Use `./run_compdb.sh -h`  if you want to add header information to `compile_commands.json`.
