var server = require('./server');
var requestHandler = require('./requestHandler')
var handle = {
    '/' : requestHandler.start,
    '/favicon.ico' : requestHandler.favicon,
    '/upload' : requestHandler.upload,
    '/show' : requestHandler.show,
}
/*
    'start' : start,
    'favicon' : favicon,
    'upload' : upload,
    'show' : show,
*/
server.serverStart(handle);