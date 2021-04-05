import moduleStyles from './ModuleStyles'
import React, { Component } from 'react';

import {
  View,
} from 'react-juce';

class DummyModule extends Component {
  render() {
    return (<View {...moduleStyles.param_module} />)
  }
}

export default DummyModule;
