import moduleStyles from './ModuleStyles'
import ParameterToggleButton from './ParameterToggleButton'

import React, { Component } from 'react';

import {
  Text,
  View,
} from 'react-juce';

interface IProps {
  paramId?: string;
  paramLabel?: string;
  isOn?: boolean;
  onToggled?: (toggled: boolean) => void;
}

class ButtonModule extends Component<IProps> {
  constructor(props: IProps) {
    super(props);
  }

  render() {
    const buttonBorderColor = this.props.isOn ? 'ff66FDCF' : "ff626262";
    const buttonTextColor = this.props.isOn ? 'ff66FDCF' : 'ffCBCBCB';

    return (
      <View
        {...moduleStyles.param_module}
      >
        <ParameterToggleButton
          paramId={this.props.paramId}
          onToggled={this.props.onToggled}
          background-color="ff17191f"
          borderColor={buttonBorderColor}
          {...styles.button}
        >
          <Text color={buttonTextColor} {...styles.param_name}>
            {this.props.paramLabel}
          </Text>
        </ParameterToggleButton>
      </View>
    )
  }
}

const styles = {
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
  param_name: {
    fontSize: 15.0,
    lineSpacing: 1.6,
  },
};

export default ButtonModule;
