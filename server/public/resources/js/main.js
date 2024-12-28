let smallGraphics;
let textGraphics;

let scaledWidth;
let scaledHeight;
let x;
let y;

let desiredWidth = 64;
let desiredHeight = 32;
let textspeed = 3;

written = false

// entire message pixels 
// 1 arr represents row (x), containing all y pixels ascending
msg = []

function setup() {
   console.log('bro')

   // Create a canvas that matches the desired output size
   createCanvas(window.innerWidth, window.innerHeight);
   //createCanvas(desiredWidth, desiredHeight)
   pixelDensity(1);
   frameRate(20)

   // Create a graphics buffer to hold the low-res capture
   smallGraphics = createGraphics(desiredWidth, desiredHeight);
   smallGraphics.pixelDensity(1);
   smallGraphics.noSmooth(); // Disable smoothing
   pixelDensity(1)
   d = smallGraphics.pixelDensity();
   noSmooth();

   scaledHeight = 32 * (window.innerWidth / 64);
   scaledWidth = window.innerWidth;

   Pxxl.LoadFont("resources/fonts/c64.bdf", async function(font) {
      var pixels = font.getPixels("back up bitch");
   
      smallGraphics.background(50, 20, 20)
      smallGraphics.loadPixels();

      console.log(font)
      window.font = font

      //let xscale = (Math.floor(32/font.SIZE[0]))
      //let yscale = (Math.floor(32/font.SIZE[0]))
      //let _size = 32/(font.FONT_ASCENT[0]+font.FONT_DESCENT[0]-Math.abs(font.FONTBOUNDINGBOX[3]))
      let _size = (32-2)/(font.SIZE[0]);
      _size = Math.floor(_size)
      //_size = 2
      console.log(_size)
      /* let xscale = _size;
      let yscale = _size; */
      let yscale = 4;
      let xscale = 3;
      //let yscale = 2.9

      textGraphics = createGraphics(Math.max(...pixels.map(el => (el.x+1)*xscale)), desiredHeight);
      textGraphics.pixelDensity(1)
      console.log(pixels)
      

      textGraphics.loadPixels()

      let lvl = 255;

      for (var p = 0; p < pixels.length; p++) {
         var pixel = pixels[p],
         x = pixel.x*xscale,
         //y = (pixel.y-font.FONTBOUNDINGBOX[3])*yscale;
         y = (pixel.y-font.FONTBOUNDINGBOX[3]/4)*yscale;

         for (var dx = 0; dx < xscale; dx++) {
            for (var dy = 0; dy < yscale; dy++) {
               let i = 4 * (y + dy) * textGraphics.width + 4 * (x + dx);
               /* textGraphics.pixels[i]=abs(sin((x+dx+y+dy)/20))*255;
               textGraphics.pixels[i+1]=abs(cos((x+dx+y+dy)/20))*255;
               textGraphics.pixels[i+2]=abs(cos((x+dx+y+dy+3)/20))*255; */
               textGraphics.pixels[i]=lvl
               //textGraphics.pixels[i+1]=lvl;
               //textGraphics.pixels[i+2]=lvl;
               textGraphics.pixels[i+3]=255;

               /* textGraphics.updatePixels();
               await new Promise(r => setTimeout(r, 1)); */
            }
         }
      }

      textGraphics.updatePixels();
      howmanyframesbro = (textGraphics.width+40)/textspeed

      for (let jj = 0; jj < howmanyframesbro; jj++) {

      }
   });

   
}

let dude = 0;
frameidx = 0;

function draw() {
   background(color(0, 0, 0));

   //smallGraphics.loadPixels();

   if (textGraphics != undefined) {
      smallGraphics.background(0, 0, 0)
      smallGraphics.image(textGraphics, -dude, 0, textGraphics.width, textGraphics.height)
      if (dude > textGraphics.width-smallGraphics.width) {
         smallGraphics.image(textGraphics, -dude+textGraphics.width+40, 0, textGraphics.width, textGraphics.height)
         //console.log(dude)
      }

      smallGraphics.loadPixels();

      for (var dx = 0; dx < smallGraphics.width; dx++) {
         for (var dy = 0; dy < smallGraphics.height; dy++) {
            let i = 4 * (dy) * smallGraphics.width + 4 * (dx);
            if (smallGraphics.pixels[i] == 255) {
               //console.log('hi')
               smallGraphics.pixels[i] = Math.abs(Math.sin(3*Math.PI*frameidx/howmanyframesbro))*255
               smallGraphics.pixels[i+1] = Math.abs(Math.sin(1+Math.PI*frameidx/howmanyframesbro))*255
               //smallGraphics.pixels[i] = Math.abs(sin(Math.PI*(frameidx/howmanyframesbro)+((dx+dy))/40))*255
               //smallGraphics.pixels[i] = Math.abs(sin(Math.PI*((dx+dy+frameidx/howmanyframesbro)/100)))*255
               //smallGraphics.pixels[i] = Math.abs(sin(Math.PI*(dx/64)))*255
               //smallGraphics.pixels[i+1] =  Math.abs(sin(Math.PI*(dx/64)))*255;
               smallGraphics.pixels[i+2] =  Math.abs(sin(Math.PI*(frameidx/howmanyframesbro)+((dx+dy))/40))*255;
            }
         }
      }

      smallGraphics.updatePixels();


      dude+=textspeed;
      dude %= textGraphics.width+40;
      frameidx++
      frameidx %= howmanyframesbro
   }

   image(smallGraphics, 0, 0, scaledWidth, scaledHeight);
}

window.onresize = function () {
   // assigns new values for width and height variables
   // w = window.innerWidth;
   // h = window.innerHeight;
   //canvas.size(w,h);
   //resizeCanvas(w, h)
   // TODO CAMERON RECALCULATE X AND Y AND SIZE AND SHIT
}

async function sendFrames() {
   let frame;

   
   interval = setInterval(() => {
      frame = [];
      smallGraphics.loadPixels()

      for (let _y = 0; _y < desiredHeight; _y++) {
         for (let _x = 0; _x < desiredWidth; _x++) {
            const i = 4 * d * (_y * d * smallGraphics.width + _x);
            frame.push(smallGraphics.pixels[i],smallGraphics.pixels[i+1], smallGraphics.pixels[i+2])
         }
      }

      sendMessage2(`{"pixels": "[${frame.join(', ')}]"}`)
   }, 50, "param");
}