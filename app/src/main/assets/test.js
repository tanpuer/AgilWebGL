let file = new File("test.js");
let result = file.exist();
let content = file.read();
console.log("result is ", result, content);

console.log("__dirname is ", __dirname);
path.resolve("test", "demo.js");