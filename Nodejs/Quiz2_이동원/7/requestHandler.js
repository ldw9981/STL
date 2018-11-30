var fs = require('fs');



// express의 res를 사용하므로 send()를 호출가능하다
function start(req,res) {
    fs.readFile('./postRequest.html',(error,html)=> {
        res.send(html.toString());

    });

}

function favicon(req,res) {
    fs.readFile('./logo.png',(error,image)=> {
        res.send(image);

    });
}

function upload(req,res) {
    
    // 동기방식으로 폴더검사및 만들기
    var dir = './image';
    if(!fs.existsSync(dir)) {
        fs.mkdirSync(dir);
    }

    //업로드된 파일 이름(경로 포함) 변경
    fs.rename(req.files.image.path,'./image/upload.png',(error) => {
        res.send('<img src=/show />');
    });
}

function show(req,res) {
    fs.readFile('./image/upload.png',(error,image)=> {
        if(error) {
            res.send(error.toString());
        } else {
            res.send(image);
        }
    });
}

module.exports = {
    'start' : start,
    'favicon' : favicon,
    'upload' : upload,
    'show' : show,
}

