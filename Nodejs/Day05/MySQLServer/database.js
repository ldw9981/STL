var mysql = require('mysql');

// 연결
var client = mysql.createConnection({
    host : 'localhost',
    user : 'root',
    password : '1234',
    database : 'restdb'
});

// 데이터베이스 연결
client.connect();

// DB처리 함수
var get = (res,id ) => {
    console.log('DB Get 요청');
    if(id)
    {
        //변수가공
        id = (typeof id === 'string') ? Number(id) : id;

        var query = 'select * from userinfo where id=?';
        client.query(query,id,(error,result,fields) => {
            if(error) {
                res.send('쿼리 실패' + query);
            } else {
                res.send(result);
            }
        });

    } else {

        var query = 'select * from userinfo';
        client.query(query,id,(error,result,fields) => {
            if(error) {
                res.send('쿼리 실패' + query);
            } else {
                res.send(result);
            }
        });
    }
}


var insert = (res,data ) => {
  
    var query = 'insert into userinfo(name,region) value (?,?)';
    client.query(query,[data.name,data.region],(error,result,fields) => {
        if(error) {
            res.send('쿼리 실패' + query);
        } else {
            res.send(result);
        }
    });
}

var remove = (res,id) => {
    var msg = 'ID가없음';
    if(id) {
        var query = 'delete from userinfo where id=?';
        id = (typeof id === 'string') ? Number(id) : id;
        client.query(query,id,(error,result,fields) => {
            if(error) {
                res.send('쿼리 실패' + query);
            } else {
                res.send(result);
            }
        });
    } else {
        console.log(msg);
        res.send(msg)
    }
}

module.exports = {
    get : get,
    insert : insert,
    remove : remove
}