import React, { Component } from 'react';
import {
  EventBridge,
  Text,
  View,
} from 'react-juce';

import type { ParamValue } from './ParamValueType';

interface IProps {
  paramId?: string;
}

interface IState {
  label?: string;
}

class Label extends Component<IProps, IState> {
  constructor(props: IProps) {
    super(props);

    this._onParameterValueChange = this._onParameterValueChange.bind(this);

    this.state = {
      label: '',
    };
  }

  componentDidMount() {
    EventBridge.addListener('parameterValueChange', this._onParameterValueChange);
  }

  componentWillUnmount() {
    EventBridge.removeListener('parameterValueChange', this._onParameterValueChange);
  }

  _onParameterValueChange(
    _index: number,
    paramId: string,
    _defaultValue: ParamValue,
    _currentValue: ParamValue,
    stringValue: string) {
    if (paramId === this.props.paramId) {
      this.setState({
        label: stringValue,
      });
    }
  }

  render() {
    return (
      <View {...this.props}>
        <Text {...styles.labelText}>
          {this.state.label}
        </Text>
      </View>
    );
  }
}

const styles = {
  labelText: {
    color: 'ffcbcbcb',
    fontSize: 15.0,
    lineSpacing: 1.6,
  },
};

export default Label;
