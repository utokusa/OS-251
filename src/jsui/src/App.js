import Label from './Label';
import ParameterSlider from './ParameterSlider'

import React, { Component } from 'react';

import {
  Text,
  View,
  Slider,
} from 'react-juce';

class Param extends Component {
  constructor(props) {
    super(props);
  }
  render() {
    const sliderFillColor  = 'ff66fdcf';
    const sliderTrackColor = 'ff626262';

    return (
      <View {...styles.param}>
        <ParameterSlider
            paramId={this.props.paramId}
            onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.rotaryGestureMap}
            {...styles.knob}
        >
          <Label paramId={this.props.paramId} {...styles.label} />
        </ParameterSlider>
        <Text {...styles.param_text}>{this.props.paramLabel}</Text>
      </View>
    )
  }
}

class DummyParam extends Component {
  render() {
    return (<View {...styles.param}/>)
  }
}

class App extends Component {
  constructor(props) {
    super(props);
  }

  render() {

    return (
      <View {...styles.container}>
        <View {...styles.content}>
          <View {...styles.param_row}>
            <Param
                paramId="sinGain"
                paramLabel="Sin"
            />
            <Param
                paramId="squareGain"
                paramLabel="Square"
            />
            <Param
                paramId="sawGain"
                paramLabel="Saw"
            />
            <Param
                paramId="subSquareGain"
                paramLabel="Sub Square"
            />
            <Param
                paramId="noiseGain"
                paramLabel="Noise"
            />
          </View>
          <View {...styles.param_row}>
            <Param
                paramId="attack"
                paramLabel="Attack"
            />
            <Param
                paramId="decay"
                paramLabel="Decay"
            />
            <Param
                paramId="sustain"
                paramLabel="Sustain"
            />
            <Param
                paramId="release"
                paramLabel="Release"
            />
            <DummyParam/>
          </View>
          <View {...styles.param_row}>
            <Param
                paramId="frequency"
                paramLabel="Frequency"
            />
            <Param
                paramId="resonance"
                paramLabel="Resonance"
            />
            <Param
                paramId="filterEnv"
                paramLabel="Env -> Filter"
            />
            <Param
                paramId="lfoFilterFreq"
                paramLabel="LFO -> Freq"
            />
            <DummyParam/>
          </View>
          <View {...styles.param_row}>
            <Param
                paramId="rate"
                paramLabel="Rate"
            />
            <Param
                paramId="lfoDelay"
                paramLabel="LFO Delay"
            />
            <Param
                paramId="lfoPitch"
                paramLabel="LFO -> Pitch"
            />
            <DummyParam/>
            <DummyParam/>
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
    backgroundColor: 'ff17191f',
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: 12,
    borderStyle: 'solid',
    borderColor: 'ff626262',
    borderWidth: 10,
  },
  content: {
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    alignItems: 'flex-start',
    padding: 12.0,
    maxWidth: 800,
    aspectRatio: 400.0 / 240.0,
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
  param: {
    minWidth: 100,
    minHeight: 100,
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    alignItems: 'center',
    marginTop: 4,
    marginBottom: 4,
    marginLeft: 12,
    marginRight: 12,
  },
  knob: {
    minWidth: 20.0,
    minHeight: 20.0,
    width: '100%',
    height: '100%',
    marginTop: 5,
    marginBottom: 0,
    justifyContent: 'center',
    alignItems: 'center',
  },
  label: {
    flex: 1.0,
    justifyContent: 'center',
    alignItems: 'center',
    interceptClickEvents: false,
  },
  param_text: {
    fontSize: 15.0,
    minHeight: 18,
    color: 'ffcbcbcb',
  },
};

export default App;
