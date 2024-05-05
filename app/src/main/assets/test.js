let file = new File("test.js");
let result = file.exist();
let content = file.read();
console.log("result is ", result, content);