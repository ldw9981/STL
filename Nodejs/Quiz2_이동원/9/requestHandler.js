// 모듈 추출.
var fs = require('fs');

// DB 변수.
var mysqlDB;

// DB 연결 함수.
function connectDB(database) {
    mysqlDB = database;
}

// /경로 처리 함수.
function start(req, res) { 
    res.redirect('/register');
}

// 사용자 등록 폼 보여주는 함수.
function showRegisterForm(req, res) {
    fs.readFile('./register.html', (error, html) => {
        //res.setHeader('Content-Type', 'text/html');
        res.type('text/html');
        res.send(html);
    });
}

// 로그인 폼 보여주는 함수.
function showLoginForm(req, res) {
    fs.readFile('./login.html', (error, html) => {
        //res.setHeader('Content-Type', 'text/html');
        res.type('text/html');
        res.send(html);
    });
}

// 사용자 정보 등록.
function register(req, res) { 
    
    // 저장할 데이터 설정.
    var data = {
        userid : req.body.id,
        nickname : req.body.nickname,
        password : req.body.password
    }

    console.log(data);

    // DB에 데이터 추가 요청.
    mysqlDB.register(res, data);
}

// 로그인 요청 함수.
function login(req, res) { 
    
    // 로그인에 사용할 데이터 설정.
    var data = {
        userid : req.body.id,
        password : req.body.password
    }
    
    console.log(data);
    // 로그인 요청.
    mysqlDB.login(res, data);
}

function getUserItem(req,res) {
    // 저장할 데이터 설정.
    var data = {
        user_idx : req.body.user_idx
    }
    console.log(data);
    mysqlDB.getUserItem(res, data);
}

function addUserItem(req,res) {
    var data = {
        user_idx : req.body.user_idx,
        item_idx : req.body.item_idx,
        count : req.body.count,
    }
    console.log(data);
    mysqlDB.addUserItem(res, data);
}

function updateUserItem(req,res) {
    var data = {
        useritem_idx : req.body.useritem_idx,
        user_idx : req.body.user_idx,
        item_idx : req.body.item_idx,
        count : req.body.count,
    }    
    console.log(data);
    mysqlDB.updateUserItem(res, data);
}

function deleteUserItem(req,res) {  
    var data = {
        user_idx : req.body.user_idx,
        useritem_idx : req.body.useritem_idx
    }    
    console.log(data);
    mysqlDB.deleteUserItem(res, data);
}


module.exports = {
    connectDB : connectDB,
    start : start,
    showRegisterForm : showRegisterForm,
    showLoginForm : showLoginForm,
    register : register,
    login : login,

    getUserItem : getUserItem,
    addUserItem : addUserItem,
    updateUserItem : updateUserItem,
    deleteUserItem : deleteUserItem,
}