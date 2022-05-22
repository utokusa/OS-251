import React, { Component, ReactNode } from 'react'

import {
  Text,
  View
} from 'react-juce'
import { textColor } from './Colors'
import { ClippingIndicator } from './ClippingIndicator'
import { PresetManager } from './PresetManager'

class Menu extends Component {
  render (): ReactNode {
    return (
      <View {...styles.menu}>
        <View {...styles.logo}>
          <Text {...styles.logo_text}>
            OS-251
          </Text>
          <ClippingIndicator {...styles.clipping_indicator}/>
          <PresetManager {...styles.preset_manager}/>
        </View>
      </View>
    )
  }
}

const styles = {
  menu: {
    width: '100%',
    flex: 1.0,
    flexDirection: 'column',
    justifyContent: 'space-around',
    marginBottom: -14,
    paddingLeft: 12.0,
    paddingRight: 12.0,
    borderWidth: 4
  },
  logo: {
    marginLeft: 13
  },
  logo_text: {
    fontSize: 20.0,
    color: textColor
  },
  clipping_indicator: {
    height: 20,
    width: 70,
    marginTop: 1,
    marginLeft: 190
  },
  preset_manager: {
    height: 20,
    width: 360,
    marginTop: 1,
    marginLeft: 40
  }
}

export default Menu
