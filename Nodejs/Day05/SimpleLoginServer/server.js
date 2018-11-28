var http = require('http');
var express = require('express');
var fs = require('fs');
var bodyparser = require('body-parser');
var crypto = require('crypto');


// 로그인 정보 객체
var loginInfo = {
    id : 'ronniej',
    pw : '1234',
}

// Express 객체
var app = express();

app.use(bodyparser.urlencoded({extended:false}));

// 라우팅 설정.
app.get('/',(req,res)=>{


 //   var jsonString = JSON.stringify(loginInfo);
 //   console.log(jsonString);


    // redirection  경로를 바꿔준다.
    res.redirect('/login');
});

app.get('/login',(req,res)=>{

    fs.readFile('./login.html',(error,html)=>{
        res.setHeader('Content-Type','text:html');
        res.send(html);
    });
});

app.post('/login',(req,res)=>{

    var idsha256 = crypto.createHash('sha256');
    idsha256.update(req.body.login);
    var pwsha256 = crypto.createHash('sha256');
    pwsha256.update(req.body.login);



    // 로그인 비교.
    if (loginInfo.id === req.body.login && loginInfo.pw === req.body.password) {
        
        res.send('로그인 성공');
    } else {
        res.send('로그인 실패');
    }

});


// 서버실행
app.listen(3000);
http.createServer(app);
console.log('server running');