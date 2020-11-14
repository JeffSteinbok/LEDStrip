const path = require('path');
const CopyWebpackPlugin = require("copy-webpack-plugin");

module.exports = {
    mode: 'development',
    entry: {
        index: './htmlsrc/Index.tsx',
    },
    output: {
        filename: '[name].js',
        sourceMapFilename: "[name].js.map"
    },
    
    module: {
        rules: [
            {
                test: /\.(js|jsx)/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                    options: {
                        "presets": ["@babel/preset-env", "@babel/preset-react"]
                    }
                },
            },
            {
                test: /\.ts(x?)$/,
                exclude: /node_modules/,
                use: [
                    {
                        loader: "ts-loader"
                    }
                ]
            }
        ]
    },
    resolve: {
        extensions: ['*', '.js', '.jsx', '.tsx']
    },
    externals: {
        "react": "React",
        "react-dom": "ReactDOM",
        "axios": "axios"
    },
    plugins: [
        new CopyWebpackPlugin({
            patterns: [
            {
                from: './index.html',
                context: './htmlsrc'
            }
        ]})
    ]
};
