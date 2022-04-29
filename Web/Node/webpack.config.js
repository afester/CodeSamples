module.exports = {
    mode: 'development',
    entry: {
        index: './src/index.js',
        react: './src/Recipe.jsx',
    },
    // watchOptions: {
    //     poll: true
    // },
    output: {
        filename: '[name].bundle.js'
    },
    module: {
        rules: [{ test: /\.jsx$/, exclude: /node_modules/, loader: "babel-loader" }]
    },
    resolve: {
        extensions: [ '.js', '.jsx'],
    },
    devtool: "source-map",
}
