var person = {
    name : 'Ronnie',
    age : 34,
    job : 'free-lancer'
}

var person1 = {
    name : 'Ronnie',
    age : 34,
    job : 'free-lancer',
    test : person
}

var testObj = new Object();
testObj.name = "Seyun";
testObj["job"] = "tutor";
var keyName = "name";
testObj["job"] = "tutor";

console.log(person);
console.log(person1);
console.log(testObj);
console.log(testObj[keyName]);


