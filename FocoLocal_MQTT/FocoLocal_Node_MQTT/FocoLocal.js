const TelegramBot = require('node-telegram-bot-api');
var config = require('./config.js');
var mqtt = require('mqtt');
const bot = new TelegramBot(config.token, {
  polling: true
});

// TODO Sistem de cambio con 1 digito
// TODO Mayuscula y miniscula
// TODO Estado
console.log(config.token)
var client = mqtt.connect('mqtt://broker.mqtt-dashboard.com');

client.on('connect', function() {
  client.subscribe('ALSW/foco1estado');
  client.subscribe('ALSW/foco2estado');
  client.subscribe('ALSW/foco3estado');
  client.subscribe('ALSW/foco4estado');
})

client.on('message', function(topic, message) {
  console.log("Topic " + topic + " - " + message.toString())
})

bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  var Mensaje = msg.text;
  console.log("Mensaje " + Mensaje);
  if (Mensaje.toLowerCase() == "encender1" || Mensaje.toLowerCase() == "e1") {
    client.publish('ALSW/foco1', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 1');
  } else if (Mensaje.toLowerCase() == "apagar1" || Mensaje.toLowerCase() == "a1") {
    client.publish('ALSW/foco1', '0');
    bot.sendMessage(chatId, 'Apagar Foco 1');
  } else if (Mensaje.toLowerCase() == "encender2" || Mensaje.toLowerCase() == "e2") {
    client.publish('ALSW/foco2', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 2');
  } else if (Mensaje.toLowerCase() == "apagar2" || Mensaje.toLowerCase() == "a2") {
    client.publish('ALSW/foco2', '0');
    bot.sendMessage(chatId, 'Apagar Foco 2');
  } else if (Mensaje.toLowerCase() == "encender3" || Mensaje.toLowerCase() == "e3") {
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 3');
  } else if (Mensaje.toLowerCase() == "apagar3" || Mensaje.toLowerCase() == "a3") {
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Foco 3');
  } else if (Mensaje.toLowerCase() == "encender4" || Mensaje.toLowerCase() == "e4") {
    client.publish('ALSW/foco4', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 4');
  } else if (Mensaje.toLowerCase() == "apagar4" || Mensaje.toLowerCase() == "a4") {
    client.publish('ALSW/foco4', '0');
    bot.sendMessage(chatId, 'Apagar Foco 4');
  } else if (Mensaje.toLowerCase() == "apagartodo" || Mensaje.toLowerCase() == "at") {
    client.publish('ALSW/foco1', '0');
    client.publish('ALSW/foco2', '0');
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Todo');
  } else if (Mensaje.toLowerCase() == "encendertodo" || Mensaje.toLowerCase() == "et") {
    client.publish('ALSW/foco1', '1');
    client.publish('ALSW/foco2', '1');
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encender Todo');
  } else if (Mensaje.toLowerCase() == "estado" || Mensaje.toLowerCase() == "e") {
    bot.sendMessage(chatId, 'No se XD');
  } else {
    bot.sendMessage(chatId, 'Comandos Disponibles, Encender1 o Apagar1 (1-4)');
  }
});

bot.on('polling_error', (error) => {
  //console.log("Eror es " + error.code); // => 'EFATAL'
});
