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
    client.query(query,  [data.userid, data.password], (error, result) => {
        if (error) {
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

var getUserItem = (res,data) => {
    // 쿼리 문자열.
    var query = 'select * from useriteminfo where user_idx=?';
    // 쿼리 실행.
    client.query(query,  [data.user_idx], (error, result) => {
        if (error) {
            console.log('쿼리 검색 실패: ' + error);
            res.send('쿼리 검색 실패: ' + error);
        } else {
            // if (result !== undefined)
            if (result.length > 0) {
                console.log('getUserItem 성공');
                res.send(result);
            } else {
                console.log('getUserItem 실패');
                res.send('getUserItem 실패');
            }
        }
    });
}

var addUserItem = (res, data) => { 
    var query = 'insert into useriteminfo(user_idx,item_idx,count) value(?,?,?)';
    client.query(query, [ data.user_idx,data.item_idx ,data.count ] , (error, result) => {
        if (error) {
            console.log('쿼리 실패: ' + error);
            res.send('쿼리 실패: ' + error);
        } else {          
            if (result.affectedRows > 0) {
                console.log('addUserItem 성공');
                res.send('addUserItem 성공');
            } else {
                console.log('addUserItem 실패');
                res.send('addUserItem 실패');
            }
        }
    });
}

var updateUserItem = (res,data) => {
 
    var query = 'update useriteminfo set count = ? where id=? and user_idx =?';
    client.query(query, [data.count, data.useritem_idx,data.user_idx], (error, result) => {
        if (error) {
            console.log('쿼리 실패: ' + error);
            res.send('쿼리 실패: ' + error);
        } else {
           
            if (result.affectedRows > 0) {
                console.log('updateUserItem 성공');
                res.send('updateUserItem 성공');
            } else {
                console.log('updateUserItem 실패');
                res.send('updateUserItem 실패');
            }
        }
    });
}

var deleteUserItem = (res, data) => {

    // 쿼리 문자열.
    var query = 'delete from useriteminfo where user_idx=? and id=?';
    // 쿼리 실행.
    client.query(query, [data.user_idx,data.useritem_idx], (error, result) => {
        if (error) {
            console.log('쿼리 검색 실패: ' + error);
            res.send('쿼리 검색 실패: ' + error);
        } else {
            // 지워진 갯수
            if (result.affectedRows > 0) {
                console.log('deleteUserItem 성공');
                res.send('deleteUserItem 성공');
            } else {
                console.log('deleteUserItem 실패');
                res.send('deleteUserItem 실패');
            }
        }
    });
}


//모듈 내보내기.
module.exports = {
    login : login,
    register : register,
    getUserItem : getUserItem,
    addUserItem : addUserItem,
    updateUserItem : updateUserItem,
    deleteUserItem : deleteUserItem


}