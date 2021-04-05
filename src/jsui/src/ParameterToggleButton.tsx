import ParameterValueStore from './ParameterValueStore';
import React, { Component } from 'react';
import { Button } from "react-juce";

interface IProps {
  paramId?: string;
  onToggled?: Function;
  borderColor?: string;
}

interface IState {
  defaultValue?: any;
  value?: any;
}

class ParameterToggleButton extends Component<IProps, IState> {
  _handleEnter: any;
  _handleLeave: any;

  constructor(props: IProps) {
    super(props);

    this._handleClick = this._handleClick.bind(this);
    this._onParameterValueChange = this._onParameterValueChange.bind(this);

    const paramState = ParameterValueStore.getParameterState(this.props.paramId);

    const initialDefaultValue = typeof paramState.defaultValue === 'number' ?
      paramState.defaultValue : 0.0;

    const initialValue = typeof paramState.currentValue === 'number' ?
      paramState.currentValue : 0.0;

    this.state = {
      defaultValue: initialDefaultValue,
      value: initialValue,
    };

    if (typeof this.props.onToggled === 'function') {
      this.props.onToggled(initialValue !== 0.0);
    }
  }

  componentDidMount() {
    ParameterValueStore.addListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    );
  }

  componentWillUnmount() {
    ParameterValueStore.removeListener(
      ParameterValueStore.CHANGE_EVENT,
      this._onParameterValueChange
    );
  }

  _handleClick(e: any) {
    const newValue = this.state.value === 0.0 ? 1.0 : 0.0

    this.setState({
      value: newValue
    });

    if (typeof this.props.paramId === 'string' && this.props.paramId.length > 0) {
      // @ts-ignore
      global.beginParameterChangeGesture(this.props.paramId);
      // @ts-ignore
      global.setParameterValueNotifyingHost(this.props.paramId, newValue);
      // @ts-ignore
      global.endParameterChangeGesture(this.props.paramId);
    }

    if (typeof this.props.onToggled === 'function') {
      this.props.onToggled(newValue !== 0.0);
    }
  }

  _onParameterValueChange(paramId: string) {
    const shouldUpdate = typeof this.props.paramId === 'string' &&
      this.props.paramId.length > 0 &&
      this.props.paramId === paramId;

    if (shouldUpdate) {
      const state = ParameterValueStore.getParameterState(paramId);

      const newDefaultValue = state.defaultValue;
      const newValue = state.currentValue;

      this.setState({
        defaultValue: newDefaultValue,
        value: newValue,
      });

      if (typeof this.props.onToggled === 'function') {
        this.props.onToggled(newValue !== 0.0);
      }
    }
  }

  render() {
    const { paramId, onToggled, ...other } = this.props;

    return (
      <Button {...other} onClick={this._handleClick} onMouseEnter={this._handleEnter} onMouseLeave={this._handleLeave}>
        {this.props.children}
      </Button>
    )
  }
}

export default ParameterToggleButton;

