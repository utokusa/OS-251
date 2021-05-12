import moduleStyles from './ModuleStyles'
import ParameterToggleButton from './ParameterToggleButton'
import IModuleColor from './ModuleColors'

import React, { Component, ReactNode } from 'react'

import {
  Text,
  View
} from 'react-juce'

interface IProps {
  paramId: string
  paramLabel?: string
  isOn?: boolean
  color: IModuleColor
  onToggled?: (toggled: boolean) => void
}

class ButtonModule extends Component<IProps> {
  render (): ReactNode {
    const buttonBorderColor = this.props.isOn === true ? this.props.color.primary : this.props.color.secondary
    const buttonTextColor = this.props.isOn === true ? this.props.color.primary : 'ff626262'

    return (
      <View
        {...moduleStyles.param_module}
      >
        <ParameterToggleButton
          paramId={this.props.paramId}
          onToggled={this.props.onToggled}
          background-color='ff282423'
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
    height: '17.5%'
  },
  param_name: {
    fontSize: 15.0,
    lineSpacing: 1.6
  }
}

export default ButtonModule
