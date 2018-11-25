var fs = require('fs');


// 파일 io모듈

function onReadFinished(error,text) {
    console.log(text);
}

// 기본이 비동기
fs.readFile('./text.txt','utf-8',onReadFinished);

/* var result = fs.readFileSync('./text.txt','utf-8');
console.log(result); */


/*
fs.readFile('./test.txt',function(error,text) {
    console.log(text);
});
*/

console.log('hello world');