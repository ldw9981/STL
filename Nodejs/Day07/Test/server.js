var http = require('http');

http.createServer(function(req, res) {

    console.log(req.headers.cookie);

    res.writeHead(200, { 
        'Content-Type' : 'text/html',
        'Set-Cookie' : ['name = seryun', 'region = seoul']
    });

    res.end();

}).listen(3000);
console.log('server running');