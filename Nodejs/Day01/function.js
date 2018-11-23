var testObj = {
    name : 'test',
    func : function()
    {
        console.log('testObj function');
    }
};

function add(a,b)
{
    return a+b;
}

function sayHello() {
    console.log('hello');
}

function testFunc(func)
{
    func();
}

var sayHello = function() {  
    console.log('hello');
}

//sayHello();
testFunc(sayHello);

var number = add(10,20);
console.log(number);