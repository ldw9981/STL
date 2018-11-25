var fs = require('fs');

function start(req,res) {    

    fs.readFile('./index.html',(error,data) => {

        res.writeHead(200,{ 'Content-Type' : 'text/html; charset=utf-8'});
        res.write(data);
        res.end();
    });

    // res.writeHead(200,{'Content-Type' : 'text/html'});
    // res.write('Hello World');
    // res.end();
}

function favicon(req,res) {
    fs.readFile('./logo.png',(error,data) => {

        res.writeHead(200,{'Content-Type' : 'image/png'});
        res.write(data);
        res.end();
    });
}

module.exports = {
    'start' : start,
    'favicon' : favicon
 }