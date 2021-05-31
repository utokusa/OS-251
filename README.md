# OS-251
OS-251 is a synthesizer plugin (vst3, au) made with C++ , JUCE and react-juce.

This plugin is currently under development.

<div align="center"><img src="screenshot.png" width="600px"></div>

## Lint
Lint checking for both C++ and Node.js is available.
See details by running `./lint.sh -h`.

## TODO

### Quality

- [ ] Improve sound
- [ ] Improve UI/UX

### Functionality

- [ ] Add "About" to UI
- [x] One knob HPF
- [x] LFO -> Shape parameter
- [x] Master pitch setting (3 knobs)
- [x] Pitchbend width setting
- [ ] Presets
- [x] (Optional) Make LFO sync with the host's tempo.
  Maybe "Sync ON" button and "Phase" knob will be added.
- [ ] ~~(Optional) Tempo LFO re-triggered by MIDI notes ("retring").~~
  ~~Maybe "Tempo ON" button and "Phase" knob will be added.~~
- [x] Allow to change number of voices
- [ ] Unison
- [x] Portamento
- [x] "ENV or GATE" switch for VCA

### Tools

- [x] Add lint tool for javascript
- [x] Move to Typescript
- [x] Lint check in CI
- [x] CI/CD for macOS, Windows and Linux
- [ ] Create installer in CD
