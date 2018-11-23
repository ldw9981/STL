
var url = require('url');


function route(handle, path, req, res) {

    var obj = url.parse(path);
    //console.log(obj);

    if( typeof handle[obj.pathname] === 'function')    {
        handle[obj.pathname](req,res,obj.query); 
    } else {
        // 404
        res.writeHead(404,{'Content-Type' : 'text/html'});
        res.write('Page Not Found');
        res.end();
    }
}

module.exports = {
    'route' : route
}