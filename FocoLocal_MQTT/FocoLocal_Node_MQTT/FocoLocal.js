// TODO: Ver pro que se apaga todo por error al apagar y encender rapido

const TelegramBot = require('node-telegram-bot-api');
var config = require('./config.js');
var mqtt = require('mqtt');
const bot = new TelegramBot(config.token, {
  polling: true
});

var Estado = [0, 0, 0, 0, 0];
var CantidadFoco = 3;
var client = mqtt.connect('mqtt://broker.mqtt-dashboard.com');

client.on('connect', function() {
  client.subscribe('ALSW/foco1estado');
  client.subscribe('ALSW/foco2estado');
  client.subscribe('ALSW/foco3estado');
  client.subscribe('ALSW/foco4estado');
})

client.on('message', function(topic, message) {
  var IDTopic = topic.substring(9, 10);
  IDTopic = Number(IDTopic);
  if (message.toString() == "1") {
    Estado[IDTopic] = "Encendido"
  } else {
    Estado[IDTopic] = "Apagado"
  }
  console.log("Foco " + IDTopic + " Estado " + Estado[IDTopic]);
})

bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  var Mensaje = msg.text;
  Mensaje = Mensaje.toLowerCase();
  if (Mensaje == "ayuda" || Mensaje == "inicio" || Mensaje == "\/start") {
    bot.sendMessage(chatId, "Foco Inteligente ChatBot de Local ALSW comandos:")
      .then(() => {
        return bot.sendMessage(chatId, "Encender o \"E\" mas numero ej: E1")
      })
      .then(() => {
        return bot.sendMessage(chatId, "Apagar o \"A\" mas numero ej: Apagar 2")
      })
      .then(() => {
        return bot.sendMessage(chatId, "EnenderTodo o \"ET\"")
      })
      .then(() => {
        return bot.sendMessage(chatId, "ApagarTodo o \"AT\"")
      })
      .then(() => {
        return bot.sendMessage(chatId, "Estado o \"E\"")
      })
      .then(() => {
        return bot.sendMessage(chatId, "Ayuda")
      })
  } else if (Mensaje == "estado" || Mensaje == "e") {
    bot.sendMessage(chatId, "Foco 1 - " + Estado[1])
      .then(() => {
        return bot.sendMessage(chatId, "Foco 2 - " + Estado[2])
      })
      .then(() => {
        return bot.sendMessage(chatId, "Foco 3 - " + Estado[3])
      })
  } else if (Mensaje == "apagartodo" || Mensaje == "at") {
    client.publish('ALSW/foco1', '0');
    client.publish('ALSW/foco2', '0');
    client.publish('ALSW/foco3', '0');
    bot.sendMessage(chatId, 'Apagar Todo');
  } else if (Mensaje == "encendertodo" || Mensaje == "et") {
    client.publish('ALSW/foco1', '1');
    client.publish('ALSW/foco2', '1');
    client.publish('ALSW/foco3', '1');
    bot.sendMessage(chatId, 'Encender Todo');
  } else if (Mensaje.indexOf("e") == 0) {
    var NumeroEncender = Mensaje.split(/encender|e| /);
    console.log("Mensaje " + NumeroEncender[1]);
    CambiarEstado(chatId, NumeroEncender[1], '1');
  } else if (Mensaje.indexOf("a") == 0) {
    var NumeroApagar = Mensaje.split(/apagar|a| /);
    CambiarEstado(chatId, NumeroApagar[1], '0');
  } else {
    bot.sendMessage(chatId, 'Comandos Disponibles, para mas info \"Ayuda\"');
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
