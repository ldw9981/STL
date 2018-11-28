var http = require('http');
var express = require('express');
var bodyparser = require('body-parser');
var fs = require('fs');
var database = require('./database');

// 서버객체생성
var app = express();
app.use(bodyparser.urlencoded({extended : false}));

// 라우터 설정
app.get('/user',(req,res) => {
    database.get(res);
});

app.get('/user/:id',(req,res) => {
    database.get(res,req.params.id);
});

app.post('/user',(req,res) => {

    // 변수 선언 (폼데이터 파싱후 저장)
    var name = req.body.name;
    var region = req.body.region;

    if (name && region) {

        var data = {
            name: name,
            region : region
        }

        database.insert(res,data);
    } else {
        throw new Error('error');
    }
});

app.put('/user/:id',(req,res)=> {
    //추출
    var id = req.params.id;
    var name = req.body.name;
    var region = req.body.region;

    //조회
    var item = database.get(res,id);
    item.name = name || item.name;  //왼쪽이 없으면 오른쪽꺼 쓴다.
    item.region = region || item.region;
});

app.delete('/user/:id',(req,res)=>{
    database.remove(res,req.params.id);
});


// env.PORT가 널이면 3000으로 쓴다.
// 환경변수는 대소문자 가리지않는다.
var port = process.env.PORT || 3000;
app.listen(port);
http.createServer(app);
console.log('running' + port);
