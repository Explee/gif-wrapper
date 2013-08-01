GifWrapper = require('./index.js');

gif = new GifWrapper(512, 512);

arr = gif.decode("import/homer.gif");
###console.log(arr[0]);###
console.log("Ok");
gif.info();

gif.push(arr[0]);

gif.encode();
console.log("Ok");

console.log(gif.delay);
console.log("Ok");