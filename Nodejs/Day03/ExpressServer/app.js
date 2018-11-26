var http = require('http');
var express = require('express');
var fs = require('fs');
// Express객체 생성

var app = express();

function start(req,res) {
    res.writeHead(200,"Content-Type","text/html");
    res.write("Hello Express");
    res.end();
}

function favicon(req,res) {
    fs.readFile('./logo.png',(error,image)=>{
        if(error) {
            console.log(error);
        } else {
            res.writeHead(200,{'Content-Type' : 'image/png'});
            res.write(image);
            res.end();
        }
    });
}

//app.use(onRequest);
//app.get('./',start);
//app.get('./favicon.ico',favicon);

app.listen(3000);
http.createServer(app);
console.log("running");