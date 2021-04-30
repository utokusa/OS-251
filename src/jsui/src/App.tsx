import Menu from './Menu'
import React, { Component, ReactNode } from 'react'

import {
  View
} from 'react-juce'

interface IState {
  isEnvForAmpOn?: boolean
  isSyncOn?: boolean
  isChorusOn?: boolean
}

function BodyView (props: any): any {
  return React.createElement('BodyView', props, props.children)
}

class App extends Component<{}, IState> {
  render (): ReactNode {
    return (
      <View {...styles.container}>
        <Menu {...styles.menu}/>
        <BodyView {...styles.body}/>
      </View>
    )
  }
}

const styles = {
  container: {
    width: '100%',
    height: '100%',
    flexDirection: 'column',
    backgroundColor: 'ff17191f'
  },
  menu: {
    width: '100%',
    height: 25
  },
  body: {
    width: '100%',
    height: 625
  }
}
export default App
