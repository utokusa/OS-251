import React, { Component, ReactNode } from 'react'
import {
  Slider,
  SyntheticMouseEvent
} from 'react-juce'

import ParameterValueStore from './ParameterValueStore'
import type { ParamValue } from './ParamValueType'

interface IProps {
  paramId: string
  onDraw?: (ctx: any, width: any, height: any, value: any) => void
  mapDragGestureToValue?: (mouseDownX: number, mouseDownY: number, sensitivity: number,
    valueAtDragStart: number, dragEvent: SyntheticMouseEvent) => number
}

interface IState {
  currentValue?: ParamValue
  defaultValue?: ParamValue
  value?: ParamValue
}

// Extend NodeJS.Global
// https://stackoverflow.com/questions/40743131/how-to-prevent-property-does-not-exist-on-type-global-with-jsdom-and-t#answer-42304473
declare global {
  // eslint-disable-next-line @typescript-eslint/no-namespace
  namespace NodeJS {
    interface Global {
      setParameterValueNotifyingHost: (paramId: string, value: ParamValue) => void
      beginParameterChangeGesture: (paramId: string) => void
      endParameterChangeGesture: (paramId: string) => void
    }
  }
}

class ParameterSlider extends Component<IProps, IState> {
  constructor (props: IProps) {
    super(props)

    this._onMouseDown = this._onMouseDown.bind(this)
    this._onMouseUp = this._onMouseUp.bind(this)
    this._onSliderValueChange = this._onSliderValueChange.bind(this)
    this._onParameterValueChange = this._onParameterValueChange.bind(this)

    const paramState = ParameterValueStore.getParameterState(this.props.paramId)
    const initialValue = typeof paramState.currentValue === 'number'
      ? paramState.currentValue
      : 0.0

    this.state = {
      value: initialValue
    }
  }

  componentDidMount (): void {
    ParameterValueStore.addListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    )
  }

  componentWillUnmount (): void {
    ParameterValueStore.removeListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    )
  }

  _onMouseDown (_e: SyntheticMouseEvent): void {
    global.beginParameterChangeGesture(this.props.paramId)
  }

  _onMouseUp (_e: SyntheticMouseEvent): void {
    global.endParameterChangeGesture(this.props.paramId)
  }

  _onSliderValueChange (value: ParamValue): void {
    if (typeof this.props.paramId === 'string' &&
      this.props.paramId.length > 0) {
      global.setParameterValueNotifyingHost(this.props.paramId, value)
    }
  }

  _onParameterValueChange (paramId: string): void {
    const shouldUpdate = typeof this.props.paramId === 'string' &&
      this.props.paramId.length > 0 &&
      this.props.paramId === paramId

    if (shouldUpdate) {
      const state = ParameterValueStore.getParameterState(paramId)

      this.setState({
        defaultValue: state.defaultValue,
        value: state.currentValue
      })
    }
  }

  render (): ReactNode {
    return (
      <Slider
        {...this.props}
        value={this.state.value}
        onMouseDown={this._onMouseDown}
        onMouseUp={this._onMouseUp}
        onChange={this._onSliderValueChange}
      >
        {this.props.children}
      </Slider>
    )
  }
}

export default ParameterSlider
