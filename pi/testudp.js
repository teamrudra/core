var PORT = 3301;
var HOST = '127.0.0.1';

var dgram = require('dgram');
var message = new Buffer('<124,55>');

var client = dgram.createSocket('udp4');
client.send(message, 0, message.length, PORT, HOST, function(err, bytes) {
    if (err) throw err;
    console.log('UDP message sent to ' + HOST +':'+ PORT);
    client.close();
});
