const TelegramBot = require('node-telegram-bot-api');
var config = require('./config.js');
var mqtt = require('mqtt');
const bot = new TelegramBot(config.token, {
  polling: true
});

// TODO: Sistem de cambio con 1 digito
// TODO: Mayuscula y miniscula
// TODO: Estado sistema
// TODO: Ver pro que se apaga todo por error al apagar y encender rapido
var client = mqtt.connect('mqtt://broker.mqtt-dashboard.com');

client.on('connect', function() {
  client.subscribe('ALSW/foco1estado');
  client.subscribe('ALSW/foco2estado');
  client.subscribe('ALSW/foco3estado');
  client.subscribe('ALSW/foco4estado');

//  client.subscribe('ALSW/foco1');
//  client.subscribe('ALSW/foco2');
//  client.subscribe('ALSW/foco3');
//  client.subscribe('ALSW/foco4');
})

client.on('message', function(topic, message) {
  console.log("Topic " + topic + " - " + message.toString());
  if(topic.indexOf("ALSW/foco") == 0){
    
  }
})



bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  var Mensaje = msg.text;
  var MensajeMinus = Mensaje.toLowerCase();
  if (MensajeMinus == "estado" || MensajeMinus == "e") {
    bot.sendMessage(chatId, 'No se XD');
  } else if (MensajeMinus == "apagartodo" || MensajeMinus == "at") {
    client.publish('ALSW/foco1', '0');
    client.publish('ALSW/foco2', '0');
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Todo');
  } else if (MensajeMinus == "encendertodo" || MensajeMinus == "et") {
    client.publish('ALSW/foco1', '1');
    client.publish('ALSW/foco2', '1');
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encender Todo');
  } else if (MensajeMinus.indexOf("encender") == 0) {
    var NumeroEncender = MensajeMinus.split("encender");
    CambiarEstado(chatId, NumeroEncender[1], '1');
  } else if (MensajeMinus.indexOf("apagar") == 0) {
    var NumeroApagar = MensajeMinus.split("apagar");
    CambiarEstado(chatId, NumeroApagar[1], '0');
  } else if (MensajeMinus.indexOf("e") == 0) {
    var NumeroEncender = MensajeMinus.split("e");
    CambiarEstado(chatId, NumeroEncender[1], '1');
  } else if (MensajeMinus.indexOf("a") == 0) {
    var NumeroApagar = MensajeMinus.split("a");
    CambiarEstado(chatId, NumeroApagar[1], '0');
  } else {
    bot.sendMessage(chatId, 'Comandos Disponibles, Encender1 o Apagar1 (1-4)');
  }
});

bot.on('polling_error', (error) => {
  //console.log("Eror es " + error.code); // => 'EFATAL'
});

function CambiarEstado(IDChat, Numero, Valor) {
  if (Number(Numero)) {
    Numero = Number(Numero);
    client.publish('ALSW/foco' + Numero, Valor);
    bot.sendMessage(IDChat, 'Encendiendo Foco ' + Numero);
  } else {
    bot.sendMessage(IDChat, "\"" + Numero + "\" no es un numero (1-4)");
  }
}
