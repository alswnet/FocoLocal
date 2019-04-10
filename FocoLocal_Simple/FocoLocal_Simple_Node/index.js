var mqtt = require('mqtt');
var SerialPort = require('serialport');

var opciones = {
  username: 'chepecarlos',
  password: 'secretoespecial'
}
var client = mqtt.connect('mqtt://broker.shiftr.io', opciones);

var Arduino = new SerialPort('/dev/ttyUSB0', {
  baudRate: 9600,
  autoOpen: true
});

Arduino.setEncoding('utf8');

Arduino.on('data', function(data) {

  for (var i = 0; i < data.length; i++) {
    if (data[i] == "H") {
      client.publish('ALSW/foco4estado', '1');
    } else if (data[i] == "L") {
      client.publish('ALSW/foco4estado', '0');
    }
  }
});

client.on('connect', function() {
  client.subscribe('ALSW/foco4', function(err) {})

})

client.on('message', function(topic, message) {
  if (message.toString() == "1") {
    console.log("encender");
    Arduino.write("H");
  } else {
    console.log("apagar")
    Arduino.write("L");
  }
})