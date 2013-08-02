
GifWrapper = require('./index.js');

gif = new GifWrapper(512, 512);

arr = gif.decode("import/homer.gif");
console.log("Ok");
gif.info();

gif.push(arr[0]);
gif.push(arr[1]);
gif.push(arr[2]);
gif.push(arr[3]);
gif.push(arr[4]);
gif.push(arr[5]);
gif.push(arr[6]);
gif.push(arr[7]);

gif.encode();
console.log("Ok");
