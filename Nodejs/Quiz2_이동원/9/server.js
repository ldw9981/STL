// 모듈 추출.
var http = require('http');
var express = require('express');
// var bodyparser = require('body-parser');

// 라우터 설정 함수.
function setRouter(app, handle) {
    for (var ix in handle) {
        switch (handle[ix].method) {
            case 'get' : 
                app.get(handle[ix].path, handle[ix].callback);
            break;
            case 'post' :
                app.post(handle[ix].path, handle[ix].callback);
            break;
            case 'put' : 
                app.put(handle[ix].path, handle[ix].callback);
            break;
            case 'delete' :
                app.delete(handle[ix].path, handle[ix].callback);
            break;
            default: break;
        }
    }
}

// 서버 실행 함수.
function serverStart(handle) {

    // 서버 생성.
    var app = express();
    // 미들웨어 설정.
    //app.use(bodyparser({extended : false}));
    app.use(express.urlencoded({extended : false}));
    // 라우터 설정.
    setRouter(app, handle);
    // 포트 설정.
    var port = process.env.port || 3000;
    // 서버 실행.
    http.createServer(app).listen(port);
    console.log('server running at ' + port);
}

// 모듈 내보내기.
module.exports = {
    serverStart : serverStart
}