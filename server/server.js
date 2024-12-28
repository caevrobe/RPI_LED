const path = require('path');
const http = require('http');
const https = require('https');
const express = require('express');
const events = require('events');
const { WebSocketServer } = require('ws');
const fs = require('fs');




/* Server setup ****************************************************************************************/

const listeners = {};
const ids = [];
let frame = [];

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ noServer: true });
const publicPath = path.join(__dirname, './public');

app.enable('trust proxy');
app.use(express.static(publicPath));

server.on('upgrade', (request, socket, head) => {
   wss.handleUpgrade(request, socket, head, (ws) => {
      wss.emit('connection', ws, request);
   });
});

server.listen(process.env.PORT || 7777, () => {
   console.log(`Server running on port ${process.env.PORT || 7777}.`);
});

/* Event + socket handling *****************************************************************************/

wss.on('connection', (ws, req) => {
   let connStr = `${req.socket.remoteAddress}:${req.socket.remotePort}`;

   console.log(`${connStr} connected!`);

   ws.on('message', (data) => {
      //console.log('msg', `'${data.toString()}'`)

      if (data.toString().replace('\0', '') === "listener") {
         listeners[connStr] = ws;
         console.log('lisrtnenenr')
      }


      else {
         //console.log('hello/')

         let dude = JSON.parse(data);
         if (dude.hasOwnProperty('pixels'))
            frame = JSON.parse(dude.pixels)
      }

         

      ws.send('')

      //ws.send(`received ${data.toString()} ok`)
      
      //data = JSON.parse(data);

      // todo add error response to client if route doesn't exist (maybe?)
      //routes[data.route]?.(data.data, ws, req);
   });

   ws.on('close', () => {
      console.log(`  ${req.socket.remoteAddress}:${req.socket.remotePort} disconnected!`);
      delete(listeners[connStr]);
   });
});


setInterval(() => {
   Object.keys(listeners).forEach(id => {
      listeners[id].send(JSON.stringify({pixels: frame}))
   })
}, 50);