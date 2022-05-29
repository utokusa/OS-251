import Menu from './Menu'
import SliderModule from './SliderModule'
import ButtonModule from './ButtonModule'
import { ClippingIndicator } from './ClippingIndicator'

import React, { Component, ReactNode } from 'react'

import {
  View,
  Text
} from 'react-juce'
import { backgroundColor, backgroundColorDark, testColorSecondary, textColorDark } from './Colors'

interface IState {
  isEnvForAmpOn?: boolean
  isSyncOn?: boolean
  isUnisonOn?: boolean
  isChorusOn?: boolean
}

class App extends Component<{}, IState> {
  constructor (props: {}) {
    super(props)

    this._onEnvForAmpToggled = this._onEnvForAmpToggled.bind(this)
    this._onSyncToggled = this._onSyncToggled.bind(this)
    this._onUnisonToggled = this._onUnisonToggled.bind(this)
    this._onChorusToggled = this._onChorusToggled.bind(this)

    this.state = {
      isEnvForAmpOn: true,
      isSyncOn: false,
      isUnisonOn: false,
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

  _onUnisonToggled (toggled: boolean): void {
    this.setState({
      isUnisonOn: toggled
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
            <View {...styles.param_row_header}>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>OSC</Text></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
            </View>
            <View {...styles.param_row_body}>
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
                paramLabel="OSC shape"
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
          </View>
          <View {...styles.param_row}>
            <View {...styles.param_row_header}>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>ENV</Text></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>LPF</Text></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
            </View>
            <View {...styles.param_row_body}>
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
            </View>
          </View>
          <View {...styles.param_row}>
            <View {...styles.param_row_header}>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>LFO</Text></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>UNISON</Text></View>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>HPF</Text></View>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>CHORUS</Text></View>
            </View>
            <View {...styles.param_row_body}>
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
              <ButtonModule
                paramId="unisonOn"
                paramLabel="Unison"
                onToggled={this._onUnisonToggled}
                isOn={this.state.isUnisonOn}
              />
              <SliderModule
                paramId="hpfFreq"
                paramLabel="HPF Freq"
              />
              <ButtonModule
                paramId="chorusOn"
                paramLabel="Chorus"
                onToggled={this._onChorusToggled}
                isOn={this.state.isChorusOn}
              />
            </View>
          </View>
          <View {...styles.param_row}>
            <View {...styles.param_row_header}>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>PITCH</Text></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}></View>
              <View {...styles.param_row_header_element}><Text {...styles.groupText}>AMP</Text></View>
              <ClippingIndicator {...styles.param_row_header_element} />
            </View>
            <View {...styles.param_row_body}>
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
              <ButtonModule
                paramId="envForAmpOn"
                paramLabel="Env -> Amp"
                onToggled={this._onEnvForAmpToggled}
                isOn={this.state.isEnvForAmpOn}
              />
              <SliderModule
                paramId="masterVolume"
                paramLabel="Master Vol"
              />
            </View>
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
    backgroundColor: backgroundColorDark,
    justifyContent: 'center',
    alignItems: 'center'
  },
  content: {
    height: '93%',
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
    flexDirection: 'column',
    backgroundColor: backgroundColor,
    borderRadius: 12,
    borderStyle: 'solid',
    borderColor: textColorDark,
    borderWidth: 3,
    marginBottom: 4
  },
  param_row_header: {
    flex: 1.0,
    width: '100%',
    height: '25%',
    flexDirection: 'row'
  },
  groupText: {
    color: testColorSecondary,
    fontSize: 16.0
  },
  param_row_header_element: {
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    alignItems: 'flex-start',
    width: 100,
    paddingTop: 6,
    paddingLeft: 15
  },
  param_row_body: {
    flex: 1.0,
    width: '100%',
    height: '85%',
    flexDirection: 'row'
  }
}

export default App
