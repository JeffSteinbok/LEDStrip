const path = require('path');
const { merge } = require('webpack-merge');
const common = require('./webpack.common.js');

module.exports = merge(common, {

    output: {
        path: path.resolve(__dirname, './debugDist'),
        sourceMapFilename: "[name].js.map"
    },

   mode: 'development',
   devtool: 'inline-source-map',
   devServer: {
       contentBase: './debugDist',
   },
});