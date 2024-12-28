let bruh = new Date()

let scheme = location.protocol.startsWith('https') ? 'wss' : 'ws'
//let ws = new WebSocket(`${scheme}://${location.host.split(':')[0]}:7777`)
let ws = new WebSocket(`${scheme}://${location.host}`);


ws.onopen = () => {
   console.log('Websocket opened..')

   sendFrames();
}

ws.onerror = (err) => {
   console.warn("errror :)", err)
}

ws.onclose = () => {
   console.log('Websocket closed.')
}

let total = 0;
let count = 0;

let idx = 0;

ws.onmessage = function(event) {
   let now = new Date();
   count++
   total += now - bruh

   idx++
   idx %= 50

   if (idx == 49) {
      console.log('avg delat', total / count)
   }

   //console.log(`${event.data  }`)
   //console.log('gotback')

   bruh = now
}

async function sendMessage2(data) {
   bruh = new Date();
   //console.log('sending')
   //let msg = `{"route": "${route}", "data": ${JSON.stringify(dataObj)}}`;

   ws?.send(data);
}

async function sendMessage(route, dataObj) {
   bruh = new Date();
   console.log('sending')
   //let msg = `{"route": "${route}", "data": ${JSON.stringify(dataObj)}}`;

   ws?.send(msg);
}