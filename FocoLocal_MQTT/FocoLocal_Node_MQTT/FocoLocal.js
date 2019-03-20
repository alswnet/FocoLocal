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
  if (Mensaje == "Encender1" || Mensaje == "E1") {
    client.publish('ALSW/foco1', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 1');
  } else if (Mensaje == "Apagar1" || Mensaje == "A2") {
    client.publish('ALSW/foco1', '0');
    bot.sendMessage(chatId, 'Apagar Foco 1');
  } else if (Mensaje == "Encender2" || Mensaje == "E2") {
    client.publish('ALSW/foco2', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 2');
  } else if (Mensaje == "Apagar2" || Mensaje == "A1") {
    client.publish('ALSW/foco2', '0');
    bot.sendMessage(chatId, 'Apagar Foco 2');
  } else if (Mensaje == "Encender3" || Mensaje == "E3") {
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 3');
  } else if (Mensaje == "Apagar3" || Mensaje == "A1") {
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Foco 3');
  } else if (Mensaje == "Encender4" || Mensaje == "E4") {
    client.publish('ALSW/foco4', '1');
    bot.sendMessage(chatId, 'Encendiendo Foco 4');
  } else if (Mensaje == "Apagar4" || Mensaje == "A4") {
    client.publish('ALSW/foco4', '0');
    bot.sendMessage(chatId, 'Apagar Foco 4');
  } else if (Mensaje == "ApagarTodo" || Mensaje == "AT") {
    client.publish('ALSW/foco1', '0');
    client.publish('ALSW/foco2', '0');
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Todo');
  } else if (Mensaje == "EncenderTodo" || Mensaje == "ET") {
    client.publish('ALSW/foco1', '1');
    client.publish('ALSW/foco2', '1');
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encender Todo');
  } else {
    bot.sendMessage(chatId, 'Comandos Disponibles, Encender1 o Apagar1 (1-4)');
  }
});

bot.on('polling_error', (error) => {
  //console.log("Eror es " + error.code); // => 'EFATAL'
});
