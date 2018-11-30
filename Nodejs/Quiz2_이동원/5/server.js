var http = require('http');
var querystring = require('querystring');
var fs = require('fs');

var port = process.env.port || 3000;
http.createServer((req,res)=>{

    if( req.method === 'GET')    {
        fs.readFile('./form.html',(error,htmlData)=>{
            res.writeHead(200,{'Content-Type':'text/html;charset=utf-8'});
            res.write(htmlData);
            res.end();
        });      
    } else if( req.method === 'POST')    {
        // form의 submit후 POST에 대한 이벤트
        req.on('data',(postData)=>{
            res.writeHead(200,{'Content-Type':'text/html;charset=utf-8'});
            
            // postData
            // 'Key'='Value'&'Key'='Value'&'Key'='Value'&'Key'='Value'....
            var obj = querystring.parse(postData.toString());
            res.write(JSON.stringify(obj));
            res.end();
        });
    }
    console.log(req.method);

}).listen(port);
console.log('server running : '+ port);