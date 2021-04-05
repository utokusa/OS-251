import moduleStyles from './ModuleStyles'
import Label from './Label'
import ParameterSlider from './ParameterSlider'

import React, { Component, ReactNode } from 'react'

import {
  Text,
  View,
  Slider
} from 'react-juce'

interface IProps {
  paramId: string
  paramLabel?: string
}

class SliderModule extends Component<IProps> {
  render (): ReactNode {
    const sliderFillColor = 'ff66fdcf'
    const sliderTrackColor = 'ff626262'

    return (
      <View {...moduleStyles.param_module}>
        <ParameterSlider
          paramId={this.props.paramId}
          onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
          mapDragGestureToValue={Slider.rotaryGestureMap}
          {...styles.knob}
        >
          <Label paramId={this.props.paramId} {...styles.value} />
        </ParameterSlider>
        <Text {...styles.param_name}>{this.props.paramLabel}</Text>
      </View>
    )
  }
}

const styles = {
  knob: {
    minWidth: 20.0,
    minHeight: 20.0,
    width: '100%',
    height: '100%',
    marginTop: 5,
    marginBottom: 0,
    justifyContent: 'center',
    alignItems: 'center'
  },
  value: {
    flex: 1.0,
    justifyContent: 'center',
    alignItems: 'center',
    interceptClickEvents: false
  },
  param_name: {
    fontSize: 15.0,
    minHeight: 18,
    color: 'ffcbcbcb'
  }
}

export default SliderModule
