var fs = require('fs');
var formidable = require('formidable')
function start(req,res) {    

    //fs.readFile('./index.html',(error,data) => {
    fs.readFile('./postRequest.html',(error,data) => {

        res.writeHead(200,{ 'Content-Type' : 'text/html; charset=utf-8'});
        res.write(data);
        res.end();
    });

    // res.writeHead(200,{'Content-Type' : 'text/html'});
    // res.write('Hello World');
    // res.end();
}

function favicon(req,res) {
    fs.readFile('./logo.png',(error,data) => {

        res.writeHead(200,{'Content-Type' : 'image/png'});
        res.write(data);
        res.end();
    });
}

function upload(req,res) {

    // formidable 모듈 초기화
    var form = new formidable.IncomingForm();
    form.parse(req,function(error,fields,files) {

        console.log(files.image.path);  //postRequest.html에서 form의 name이 키로 사용됨

        fs.rename(files.image.path,'./image/upload.png',function(error) {
            res.writeHead(200,{'Content-Type' : 'text/html'});
            res.write('<img src=/show />');
            res.end();
        });
    });

    // res.writeHead(200,{'Content-Type' : 'text/html'});
    // res.write('upload');
    // res.end();
}

function show(req,res)
{
    fs.readFile('./image/upload.png',function(error,iamge) {

        res.writeHead(200,{'Content-Type' : 'image/png'});
        res.write(iamge);
        res.end();
    });
}


module.exports = {
    'start' : start,
    'favicon' : favicon,
    'upload' : upload,
    'show' : show
 }