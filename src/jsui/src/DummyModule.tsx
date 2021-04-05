import moduleStyles from './ModuleStyles'
import React, { Component, ReactNode } from 'react'

import {
  View
} from 'react-juce'

class DummyModule extends Component {
  render (): ReactNode {
    return (<View {...moduleStyles.param_module} />)
  }
}

export default DummyModule
