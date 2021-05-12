import Menu from './Menu'
import SliderModule from './SliderModule'
import ButtonModule from './ButtonModule'
import DummyModule from './DummyModule'
import { darkBlueModuleColor } from './ModuleColors'

import React, { Component, ReactNode } from 'react'

import {
  View
} from 'react-juce'

interface IState {
  isEnvForAmpOn?: boolean
  isSyncOn?: boolean
  isChorusOn?: boolean
}

class App extends Component<{}, IState> {
  constructor (props: {}) {
    super(props)

    this._onEnvForAmpToggled = this._onEnvForAmpToggled.bind(this)
    this._onSyncToggled = this._onSyncToggled.bind(this)
    this._onChorusToggled = this._onChorusToggled.bind(this)

    this.state = {
      isEnvForAmpOn: true,
      isSyncOn: false,
      isChorusOn: false
    }
  }

  _onEnvForAmpToggled (toggled: boolean): void {
    this.setState({
      isEnvForAmpOn: toggled
    })
  }

  _onSyncToggled (toggled: boolean): void {
    this.setState({
      isSyncOn: toggled
    })
  }

  _onChorusToggled (toggled: boolean): void {
    this.setState({
      isChorusOn: toggled
    })
  }

  render (): ReactNode {
    return (
      <View {...styles.container}>
        <Menu />
        <View {...styles.content}>
          <View {...styles.param_row}>
            <SliderModule
              paramId="sinGain"
              paramLabel="Sin"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="squareGain"
              paramLabel="Square"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="sawGain"
              paramLabel="Saw"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="subSquareGain"
              paramLabel="Sub Square"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="noiseGain"
              paramLabel="Noise"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="shape"
              paramLabel="Osc shape"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="lfoShape"
              paramLabel="LFO -> Shape"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="numVoices"
              paramLabel="Num Voices"
              color={darkBlueModuleColor}
            />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="attack"
              paramLabel="Attack"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="decay"
              paramLabel="Decay"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="sustain"
              paramLabel="Sustain"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="release"
              paramLabel="Release"
              color={darkBlueModuleColor}
            />
            <ButtonModule
              paramId="envForAmpOn"
              paramLabel="Env -> Amp"
              onToggled={this._onEnvForAmpToggled}
              color={darkBlueModuleColor}
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
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="resonance"
              paramLabel="Resonance"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="filterEnv"
              paramLabel="Env -> Filter"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="lfoFilterFreq"
              paramLabel="LFO -> Freq"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="hpfFreq"
              paramLabel="HPF Freq"
              color={darkBlueModuleColor}
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
              color={darkBlueModuleColor}
              isOn={this.state.isSyncOn}
            />
            <SliderModule
              paramId="rate"
              paramLabel="Rate"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="rateSync"
              paramLabel="Synced Rate"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="lfoPhase"
              paramLabel="LFO Phase"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="lfoDelay"
              paramLabel="LFO Delay"
              color={darkBlueModuleColor}
            />
            <DummyModule />
            <DummyModule />
            <ButtonModule
              paramId="chorusOn"
              paramLabel="Chorus"
              onToggled={this._onChorusToggled}
              color={darkBlueModuleColor}
              isOn={this.state.isChorusOn}
            />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="lfoPitch"
              paramLabel="LFO -> Pitch"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="pitchBendWidth"
              paramLabel="Pitch Bend"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="portamento"
              paramLabel="Portamento"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="masterOctaveTune"
              paramLabel="Octave"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="masterSemitoneTune"
              paramLabel="Semi"
              color={darkBlueModuleColor}
            />
            <SliderModule
              paramId="masterFineTune"
              paramLabel="Fine Tune"
              color={darkBlueModuleColor}
            />
            <DummyModule />
            <SliderModule
              paramId="masterVolume"
              paramLabel="Master Vol"
              color={darkBlueModuleColor}
            />
          </View>
        </View>
      </View>
    )
  }
}

const styles = {
  container: {
    width: '100%',
    height: '100%',
    flexDirection: 'column',
    backgroundColor: 'ff262123',
    justifyContent: 'center',
    alignItems: 'center'
  },
  content: {
    height: '95%',
    width: '100%',
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    alignItems: 'flex-start',
    padding: 12.0
  },
  param_row: {
    flex: 1.0,
    width: '100%',
    flexDirection: 'row',
    backgroundColor: 'ff282423',
    borderRadius: 12,
    borderStyle: 'solid',
    borderColor: 'ffA99988',
    borderWidth: 3,
    marginBottom: 4
  }
}

export default App
