function exitListener() {
    console.log('Bye Bye');
}
function testListener() {
    console.log('Test Test');
}



// 이벤트 등록
process.on('test',testListener);
process.on('exit',exitListener);    // 'exit'는 시스템 이벤트임

setTimeout( function() {    
    process.emit('test')
}, 2000);

/*
var count = 0;

    
function test() {
    count++;
    if (count > 3)
    {
        return;
    }

    setTimeout(test,2000);
    process.emit('exit');
}
*/