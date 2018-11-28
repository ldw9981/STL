var dummyDB = (() => {
    // 변수선언
    var dummyDB = {};
    var storage = [
        //{ id: 0, name : 'ldw9981' , region : 'seoul'}
    ];
    var count = 1;

    dummyDB.get = (id) => {

        console.log('dummyDB.get');
        if (id) {
            // 변수 가공
            id = (typeof id === 'string') ? Number(id) : id;

            // 데이터 선택
            for (var ix in storage) {
                if (storage[ix].id === id) {
                    return storage[ix];
                }
            }
            console.log('not exist');
        } else {
            //id 입력하지 않은 경우에는 전체 DB반환
            return storage;
        }
    }

    // 데이터 추가
    dummyDB.insert = (data) => {
        data.id = count++;
        storage.push(data);
        return data;
    }

    dummyDB.remove = (id) => {

        // 데이터 가공
        id = (typeof id === 'string') ? Number(id) : id;

        // 데이터 제거
        for (var ix in storage) {
            if (storage[ix].id === id) {
                storage.splice(ix,1);
                return true;
            }
        }
        // 못찾으면 실패
        return false;
    }
    return dummyDB;
})();   // 함수 선언과 동시에 호출한다.



module.exports = {
    dummyDB : dummyDB
}