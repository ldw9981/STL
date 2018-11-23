//모듈얻기
var http = require('http');

function onRequest(req,res)
{
    res.writeHead(200,{ 'Content-Type' : 'text/html; charset=utf-8'});
    res.write('this is the first nodejs server. NodeJs 첫번쨰 서버입니다.','utf-8');

    console.log('got request');

    res.end();
}

//var server = http.createServer(onRequest);
//server.listen(3000);
http.createServer(onRequest).listen(3000);



console.log('서버 실행중');
