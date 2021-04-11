const path = require('path')

module.exports = {
  entry: './src/index.tsx',
  resolve: {
    extensions: ['.tsx', '.ts', '.js']
  },
  output: {
    path: path.join(__dirname, '/build/js'),
    filename: 'main.js',
    sourceMapFilename: '[file].map',
    devtoolModuleFilenameTemplate: info =>
      `webpack:///${info.absoluteResourcePath.replace(/\\/g, '/')}`
  },
  devtool: 'source-map',
  module: {
    rules: [
      {
        test: /\.(ts|tsx)$/,
        use: 'ts-loader',
        exclude: /node_modules/
      },
      {
        test: /\.(js|jsx)$/,
        exclude: /node_modules/,
        use: ['babel-loader']
      },
      {
        test: /\.svg$/,
        exclude: /node_modules/,
        use: ['svg-inline-loader']
      },
      {
        test: /\.(png|jpeg|jpg|gif)$/,
        use: [
          {
            loader: 'url-loader',
            options: {
              limit: true,
              esModule: false
            }
          }
        ]
      }
    ]
  }
}
