var http = require('http');
var express = require('express');
var multyparty = require('connect-multiparty');

// 라우터 설정 함수
var setRouter = (app,handle) => {
    for( var path in handle)
    {
        app.get(path,handle[path]);
        app.post(path,handle[path]);
    }
}

// handle 라우터로 사용
var serverStart = function (handle) {
    //Express생성
    var app = express();

    // connect-multiparty 미들웨어 사용설정
    app.use(multyparty());

    // 라우터 설정
    setRouter(app,handle);

    //포트열기
    app.listen(3000);

    //서버실행
    http.createServer(app);
    console.log('Server running');
}


module.exports = {
    'serverStart' : serverStart
}