import Label from './Label';
import ParameterSlider from './ParameterSlider'
import ParameterToggleButton from './ParameterToggleButton'

import React, { Component } from 'react';

import {
  Text,
  View,
  Slider,
} from 'react-juce';

class SliderModule extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    const sliderFillColor = 'ff66fdcf';
    const sliderTrackColor = 'ff626262';

    return (
      <View {...styles.param_module}>
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

class ButtonModule extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    const buttonBorderColor = this.props.isOn ? 'ff66FDCF' : "ff626262";
    const buttonTextColor = this.props.isOn ? 'ff66FDCF' : 'ffCBCBCB';

    return (
      <View
        {...styles.param_module}
      >
        <ParameterToggleButton
          paramId={this.props.paramId}
          onToggled={this.props.onToggled}
          background-color="ff17191f"
          borderColor={buttonBorderColor}
          {...styles.button}
        >
          <Text color={buttonTextColor} {...styles.button_text}>
            {this.props.paramLabel}
          </Text>
        </ParameterToggleButton>
      </View>
    )
  }
}

class DummyModule extends Component {
  render() {
    return (<View {...styles.param_module} />)
  }
}

class App extends Component {
  constructor(props) {
    super(props);

    this._onChorusToggled = this._onChorusToggled.bind(this);

    this.state = {
      isChorusOn: false
    }
  }

  _onChorusToggled(toggled) {
    this.setState({
      isChorusOn: toggled
    });
  }

  render() {

    return (
      <View {...styles.container}>
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
            <ButtonModule
              paramId="chorusOn"
              paramLabel="Chorus"
              onToggled={this._onChorusToggled}
              isOn={this.state.isChorusOn}
            />
          </View>
          <View {...styles.param_row}>
            <SliderModule
              paramId="rate"
              paramLabel="Rate"
            />
            <SliderModule
              paramId="lfoDelay"
              paramLabel="LFO Delay"
            />
            <SliderModule
              paramId="lfoPitch"
              paramLabel="LFO -> Pitch"
            />
            <DummyModule />
            <DummyModule />
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
  param_module: {
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
  button: {
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: 5.0,
    borderWidth: 5.0,
    minWidth: 30.0,
    minHeight: 30.0,
    width: '80%',
    height: '17.5%',
  },
  button_text: {
    fontSize: 15.0,
    lineSpacing: 1.6,
  },
};

export default App;
