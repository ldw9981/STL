// 모듈 추출
var server = require('./server');
var requestHandler = require('./requestHandler')
var router = require('./router');

// 경로요청처리 목록
var handle = {
    '/' : requestHandler.start,
    '/favicon.ico' : requestHandler.favicon,
    '/upload' : requestHandler.upload,
    '/show' : requestHandler.show,

};

// 서버 실행
server.serverStart(router.route,handle);