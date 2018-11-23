var http = require('http');
var fs = require('fs');

function serverStart(route,handle) {
    http.createServer((req,res) =>
    {    
        console.log(req.url);  
        var path = req.url;

        //요청 경로 처리
        route(handle,path,req,res);
    }).listen(3000);
    console.log('실행중');
}


module.exports = {
    "serverStart" : serverStart
}
