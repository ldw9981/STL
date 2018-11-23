function addNumber(a,b)
{
    return a + b;
}

function substract(a,b)
{
    return a-b;
}

var person = {
    name :'ronnie',
}

// 모듈 추출
module.exports = { 
    "addNumber" : addNumber,
    "substract" : substract,
    "person" : person
}