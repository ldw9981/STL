// 모듈 추출.
var mysql = require('mysql');

// DB 연결 정보 설정.
var client = mysql.createConnection({
    host : 'localhost',
    //port : '3306',
    user : 'root',
    password : '1234',
    database : 'userdb'
});

// DB 연결.
client.connect((error) => {
    if (error) {
        console.log('DB 연결 오류: ' + error);
    }
});

// 로그인 처리 함수.
var login = (res, data) => {
    // 쿼리 문자열.
    var query = 'select * from userinfo where userid=? and password=?';
    // 쿼리 실행.
    client.query(query,  [data.userid, data.password], (erorr, result) => {
        if (erorr) {
            console.log('쿼리 검색 실패: ' + error);
            res.send('쿼리 검색 실패: ' + error);
        } else {
            // if (result !== undefined)
            if (result.length > 0) {
                console.log('로그인 성공');
                res.send('로그인 성공');
            } else {
                console.log('로그인 실패');
                res.send('로그인 실패');
            }
        }
    });
}

// 등록.
var register = (res, data) => {
    // 쿼리 문자열.
    var query = 'select * from userinfo where id=?';
    // 쿼리 실행.
    client.query(query, data.userid, (error, result) =>{
        // 결과 확인.
        if (result.length > 0) {
            console.log('ID가 사용중입니다.');
            res.send('ID가 사용중입니다.');
        } else {
            query = 'insert into userinfo set?';
            client.query(query, data, (error, result) =>{
                if (error) {
                    console.log('사용자 등록 실패: ' + error);
                    res.send('사용자 등록 실패: ' + error);
                } else {
                    console.log('사용자 등록 성공');
                    res.send('사용자 등록 성공');
                }
            });
        }
    });
}

//모듈 내보내기.
module.exports = {
    login : login,
    register : register
}