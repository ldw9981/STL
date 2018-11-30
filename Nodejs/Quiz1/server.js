var http = require('http');
var start = require('./start');
var show = require('./show');

var pathHandler = {
    '/' : start.excute,
    '/show' : show.excute,
}

http.createServer( (req,res) => {
    var path = req.url;
    console.log(req.url);  
     console.log(path);  
    console.log(pathHandler);  

    if( typeof pathHandler[path] === 'function')    {
        pathHandler[path](req,res); 
    } else {
        // 404
        res.writeHead(404,{'Content-Type' : 'text/html'});
        res.write('Page Not Found');
        res.end();
    }  

}).listen(3000);

console.log('running');