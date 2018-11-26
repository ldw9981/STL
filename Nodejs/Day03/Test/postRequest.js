var http = require('http');
var fs = require('fs');
var querystring = require('querystring');

function onRequest(req,res) {
    var method = req.method;
    if( method === 'GET') {
        fs.readFile('./post.html',function(error,html) {
            res.writeHead(200,{'Content-Type' : 'text/html'});
            res.write(html);
            res.end();
        });
    } else if (method === "POST") {
        req.on('data',function(data) {
            res.writeHead(200,{'Content-Type' : 'text/html'});
            res.write(data);
            res.end();

            // 쿼리문을 파싱하여 오브젝트 생성한다.
            var obj = querystring.parse(data.toString());
            console.log(obj.data_a + " : " + obj.data_b);
        });
    }

}

http.createServer(onRequest).listen(3000);

console.log('실행중');