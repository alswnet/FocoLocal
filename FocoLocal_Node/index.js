const TelegramBot = require('node-telegram-bot-api');

// replace the value below with the Telegram token you receive from @BotFather
const token = '583335038:AAErAWHntnMDTd7uArtWvV8tUrPw2Lj9P4A';

// Create a bot that uses 'polling' to fetch new updates
const bot = new TelegramBot(token, {
  polling: true
});

var SerialPort = require('serialport');
var port = new SerialPort('/dev/ttyUSB1', {
  baudRate: 9600
});

bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  var Mensaje = msg.text;
  if (Mensaje == "Encender1") {
    port.write("a");
    bot.sendMessage(chatId, 'Encendiendo Foco 1');
  } else if (Mensaje == "Apagar1") {
    port.write("b");
    bot.sendMessage(chatId, 'Apagando Foco 1');

  }
  if (Mensaje == "Encender2") {
    port.write("c");
    bot.sendMessage(chatId, 'Encendiendo Foco 2');

  } else if (Mensaje == "Apagar2") {
    port.write("d");
    bot.sendMessage(chatId, 'Apagando Foco 2');

  }
  if (Mensaje == "Encender3") {
    port.write("e");
    bot.sendMessage(chatId, 'Encendiendo Foco 3');

  } else if (Mensaje == "Apagar3") {
    port.write("f");
    bot.sendMessage(chatId, 'Apagando Foco 3');

  } else {
  //  bot.sendMessage(chatId, 'No entiendo el Comando');
  }
});
