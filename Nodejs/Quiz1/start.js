function excute(req,res) {
    res.writeHead(200,{'Content-Type' : 'text/html'});
    res.write('This is excute functinon in start.js.');
    res.end();
}

module.exports = {
    'excute' : excute
}


