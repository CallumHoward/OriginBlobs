module.exports = {
  entry: {
    main: "./client.js",
  },
  devtool: "eval-source-map",
  devServer: {
    contentBase: ".",
    port: 8081,
    hot: true,
  },
  resolve: {
    extensions: ["*", ".js"],
  },
  output: {
    path: __dirname + "dist",
    publicPath: "/dist",
    filename: "main.js",
  },
};
