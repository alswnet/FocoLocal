const TelegramBot = require('node-telegram-bot-api');
const token = '583335038:AAErAWHntnMDTd7uArtWvV8tUrPw2Lj9P4A';// @FocoALSWbot
var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://broker.mqtt-dashboard.com')

const bot = new TelegramBot(token, {polling: true});

 bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  var Mensaje = msg.text;
  if(Mensaje == "Apagartodo"){
    bot.sendMessage(chatId, 'Apagando todos los focos');
    client.publish('ALSWin', 'f0');
  }
  else if(Mensaje == "Encendertodo"){
    bot.sendMessage(chatId, 'Encendiendo todos los focos');
    client.publish('ALSWin', 'f1');
  }
  if(Mensaje == "Apagarcero"){
    bot.sendMessage(chatId, 'Apagando el foco cero');
    client.publish('ALSWin', 'a0');
  }
  else if(Mensaje == "Encendercero"){
    bot.sendMessage(chatId, 'Encendiendo el foco cero');
    client.publish('ALSWin', 'a1');
  }
else if(Mensaje == "Apagaruno"){
    bot.sendMessage(chatId, 'Apagando el foco uno');
    client.publish('ALSWin', 'b0');
  }
  else if(Mensaje == "Encenderuno"){
    bot.sendMessage(chatId, 'Encendiendo el foco uno');
    client.publish('ALSWin', 'b1');
  }
  else if(Mensaje == "Apagardos"){
      bot.sendMessage(chatId, 'Apagando el foco dos');
      client.publish('ALSWin', 'c0');
    }
    else if(Mensaje == "Encenderdos"){
      bot.sendMessage(chatId, 'Encendiendo el foco dos');
      client.publish('ALSWin', 'c1');
    }
  else{
    bot.sendMessage(chatId, 'No entiendo el comando');
  }
});

client.on('connect', function () {
  client.subscribe('ALSWin', function (err) {
    if (!err) {
      client.publish('ALSWout', 'Hello desde el server');
    }
  })
})
