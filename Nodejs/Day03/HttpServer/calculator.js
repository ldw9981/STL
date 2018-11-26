function add(a,b) {
    return Number(a)+Number(b);
}
function mul(a,b) {
    return Number(a)*Number(b);
}
function sub(a,b) {
    return Number(a)-Number(b);
}
function div(a,b) {
    return Number(a)/Number(b);
}


 function calculate(req,res,query) {
    console.log(query);
    var temp = query.split("=");
    var func = temp[0];
    var param = temp[1].split("&");
   
    console.log(func);
    console.log(param);

    var resultCalculate = module.exports[func](param[0],param[1]);
    console.log(resultCalculate);

    res.writeHead(200,{ 'Content-Type' : 'text/html; charset=utf-8'});
    var resultText = param[0] + ' ' +  func + ' ' + param[1] + ' ' + '=' + ' ' + String(resultCalculate);
    res.write(String(resultText));
    res.end();
 }

module.exports = {
    'add' : add,
    'mul' : mul,
    'sub' : sub,
    'div' : div,
    'calculate' : calculate
}