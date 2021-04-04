import Menu from './Menu'
import SliderModule from './SliderModule'
import ButtonModule from './ButtonModule'
import DummyModule from './DummyModule'

import React, { Component } from 'react';

import {
  View,
} from 'react-juce';

class App extends Component {
  constructor(props) {
    super(props);

    this._onEnvForAmpToggled = this._onEnvForAmpToggled.bind(this);
    this._onSyncToggled = this._onSyncToggled.bind(this);
    this._onChorusToggled = this._onChorusToggled.bind(this);

    this.state = {
      isEnvForAmpOn: true,
      isSyncOn: false,
      isChorusOn: false
    }
  }

  _onEnvForAmpToggled(toggled) {
    this.setState({
      isEnvForAmpOn: toggled
    });
  }

  _onSyncToggled(toggled) {
    this.setState({
      isSyncOn: toggled
    })
  }

  _onChorusToggled(toggled) {
    this.setState({
      isChorusOn: toggled
    });
  }

  render() {

    return (
      <View {...styles.container}>
        <Menu />
        <View {...styles.content}>
          <View {...styles.param_row}>
            <SliderModule
              paramId="sinGain"
              paramLabel="Sin"
            />
            <SliderModule
              paramId="squareGain"
              paramLabel="Square"
            />
            <SliderModule
              paramId="sawGain"
              paramLabel="Saw"
            />
            <SliderModule
              paramId="subSquareGain"
              paramLabel="Sub Square"
            />
            <SliderModule
              paramId="noiseGain"
              paramLabel="Noise"
            />
            <SliderModule
              paramId="shape"
              paramLabel="Osc shape"
            />
            <SliderModule
              paramId="lfoShape"
              paramLabel="LFO -> Shape"
            />
            <SliderModule
              paramId="numVoices"
              paramLabel="Num Voices"
            />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="attack"
              paramLabel="Attack"
            />
            <SliderModule
              paramId="decay"
              paramLabel="Decay"
            />
            <SliderModule
              paramId="sustain"
              paramLabel="Sustain"
            />
            <SliderModule
              paramId="release"
              paramLabel="Release"
            />
            <ButtonModule
              paramId="envForAmpOn"
              paramLabel="Env -> Amp"
              onToggled={this._onEnvForAmpToggled}
              isOn={this.state.isEnvForAmpOn}
            />
            <DummyModule />
            <DummyModule />
            <DummyModule />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="frequency"
              paramLabel="Frequency"
            />
            <SliderModule
              paramId="resonance"
              paramLabel="Resonance"
            />
            <SliderModule
              paramId="filterEnv"
              paramLabel="Env -> Filter"
            />
            <SliderModule
              paramId="lfoFilterFreq"
              paramLabel="LFO -> Freq"
            />
            <SliderModule
              paramId="hpfFreq"
              paramLabel="HPF Freq"
            />
            <DummyModule />
            <DummyModule />
            <DummyModule />
          </View>
          <View {...styles.param_row}>
            <ButtonModule
              paramId="syncOn"
              paramLabel="LFO Sync"
              onToggled={this._onSyncToggled}
              isOn={this.state.isSyncOn}
            />
            <SliderModule
              paramId="rate"
              paramLabel="Rate"
            />
            <SliderModule
              paramId="rateSync"
              paramLabel="Synced Rate"
            />
            <SliderModule
              paramId="lfoPhase"
              paramLabel="LFO Phase"
            />
            <SliderModule
              paramId="lfoDelay"
              paramLabel="LFO Delay"
            />
            <DummyModule />
            <DummyModule />
            <ButtonModule
              paramId="chorusOn"
              paramLabel="Chorus"
              onToggled={this._onChorusToggled}
              isOn={this.state.isChorusOn}
            />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="lfoPitch"
              paramLabel="LFO -> Pitch"
            />
            <SliderModule
              paramId="pitchBendWidth"
              paramLabel="Pitch Bend"
            />
            <SliderModule
              paramId="portamento"
              paramLabel="Portamento"
            />
            <SliderModule
              paramId="masterOctaveTune"
              paramLabel="Octave"
            />
            <SliderModule
              paramId="masterSemitoneTune"
              paramLabel="Semi"
            />
            <SliderModule
              paramId="masterFineTune"
              paramLabel="Fine Tune"
            />
            <DummyModule />
            <SliderModule
              paramId="masterVolume"
              paramLabel="Master Vol"
            />
          </View>
        </View>
      </View>
    );
  }
}

const styles = {
  container: {
    width: '100%',
    height: '100%',
    flexDirection: 'column',
    backgroundColor: 'ff17191f',
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: 12,
    borderStyle: 'solid',
    borderColor: 'ff626262',
    borderWidth: 10,
  },
  content: {
    height: '95%',
    width: '100%',
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    alignItems: 'flex-start',
    padding: 12.0,
  },
  param_row: {
    flex: 1.0,
    width: '100%',
    flexDirection: 'row',
    borderRadius: 12,
    borderStyle: 'solid',
    borderColor: 'ff626262',
    borderWidth: 3,
    marginBottom: 4,
  },
};

export default App;
