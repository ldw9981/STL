//모듈얻기
var http = require('http');
var fs = require('fs');

function onRequest(req,res)
{
    fs.readFile('./slash.jpg',function(error,image){

        res.writeHead(200,{ 'Content-Type' : 'image/jpeg' });
        res.write(image);

        console.log('got request');
    
        res.end();
    });


}

//var server = http.createServer(onRequest);
//server.listen(3000);
http.createServer(onRequest).listen(3000);



console.log('서버 실행중');
