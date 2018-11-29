var http = require('http');
var express = require('express');
var cookieparser = require('cookie-parser');

// 앱 객체 생성.
var app = express();

// 미들웨어 설정.
app.use(cookieparser());

// 라우터 설정.
app.get('/', (req, res) => {
    res.redirect('/getcookie');
});

app.get('/clear', (req, res) =>{

    console.log('clear');

    res.clearCookie('string');
    res.clearCookie('json');
    res.send('');
});

app.get('/getcookie', (req, res) => {
    res.send(req.cookies);
});

app.get('/setcookie', (req, res) => {
    
    // 쿠키 생성.
    res.cookie('name', 'ronnie', { maxAge: 3000 });
    res.cookie('region', 'seoul', { maxAge: 3000 });

    // 리디렉션.
    res.redirect('/getcookie');
});



http.createServer(app).listen(3000);
console.log('server running');