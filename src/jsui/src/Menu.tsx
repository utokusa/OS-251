import React, { Component, ReactNode } from 'react'

import {
  Text,
  View
} from 'react-juce'
import { textColor } from './Colors'

class Menu extends Component {
  render (): ReactNode {
    return (
      <View {...styles.menu}>
        <View {...styles.logo}>
          <Text {...styles.logo_text}>
            OS-251
          </Text>
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
  }
}

export default Menu
