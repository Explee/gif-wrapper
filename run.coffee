NodeGif = require('./index.js');

gif = new NodeGif(512, 512);

gif.decode("import/homer.gif");
console.log("Ok");
gif.info();

gif.encode();
console.log("Ok");

console.log(gif.delay);
console.log("Ok");