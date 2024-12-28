let capture;
let smallGraphics;

let scaledWidth;
let scaledHeight;
let x;
let y;

let desiredWidth = 64;
let desiredHeight = 32;

let font = 'Arial Black'

/* let desiredWidth = 3;
let desiredHeight = 3; */

function setup() {
   console.log('bro')

   // Create a canvas that matches the desired output size
   createCanvas(window.innerWidth, window.innerHeight);
   frameRate(20)
   //createCanvas(desiredWidth, desiredHeight)

   let constraints = {
      video: {
         width: desiredWidth,
         height: desiredHeight,
         flipped: false,
         frameRate: 20 // 20fps = 50ms   |   40fps = 25ms
      }
   }

   console.log('testting')

   pixelDensity(1);

   // Create a video capture object at a low resolution (desiredHeightxdesiredWidth)
   capture = createCapture(constraints, () => {
      console.log(capture.width, width)
      console.log(capture.height, height)

      capRatio = capture.width / capture.height;
      canRatio = window.innerWidth / window.innerHeight;

      // webcam is wider than screen
      if (capRatio > canRatio) {
         scaledHeight = capture.height * (window.innerWidth / capture.width);
         scaledWidth = window.innerWidth;

         x = 0;
         y = (window.innerHeight - scaledHeight) / 2;

         console.log('is wider')
      } else {
         scaledHeight = window.innerHeight;
         scaledWidth = capture.width * (window.innerHeight / capture.height);

         x = (window.innerWidth - scaledWidth) / 2;
         y = 0;

         console.log('is taller')
      }
   });

   console.log('capture createed')

   //capture.size(desiredHeight, desiredWidth);
   capture.hide(); // Hide the default HTML video element

   // Create a graphics buffer to hold the low-res capture
   smallGraphics = createGraphics(desiredWidth, desiredHeight);
   smallGraphics.pixelDensity(1);
   smallGraphics.noSmooth(); // Disable smoothing
   pixelDensity(1)
   d = smallGraphics.pixelDensity();
   noSmooth();
}

lvl = 25;
dilate = true

/* let total = 0;
let count = 0;
let bruh = new Date()
let idx = 0; */

function draw() {
   background(color(0, 0, 0));


   /* if (dilate)
      smallGraphics.filter(DILATE, 2); */

   // Upscale the low-res buffer onto the main canvas
   // Disable smoothing for the main canvas

   /* textSize(10);
   //console.log('hi')
   textSize(32);
   fill(255);
   text('hi', 10, 10); */

   smallGraphics.image(capture, 0, 0, desiredWidth, desiredHeight);
   /* smallGraphics.fill(color(0, 255, 0))
   smallGraphics.loadPixels();
   for (let oo = 0; oo < smallGraphics.pixels.length; oo++) {
      //smallGraphics.pixels[oo] = 255
   } */

   smallGraphics.loadPixels();
   /*smallGraphics.pixels[0] = 255
   smallGraphics.pixels[1] = 255
   smallGraphics.pixels[2] = 255*/
   /* for (let bruhhh = 4; bruhhh < smallGraphics.pixels.length; bruhhh++) {
      smallGraphics.pixels[bruhhh] = 0 
   } */
   smallGraphics.updatePixels();


   

   /* if (scaledWidth)
      translate(scaledWidth, 0);
   scale(-1, 1) */
   //scale(-scaledWidth/desiredWidth, scaledHeight/desiredHeight)

   /* translate(desiredWidth*10, desiredHeight*10);
   scale(-10, 10)
   image(smallGraphics, 0, 0, desiredWidth, desiredHeight) */
   //copy(smallGraphics, 0, 0, desiredWidth, desiredHeight, 0, 0, scaledWidth, scaledHeight)

   /* smallGraphics.textSize(30);
   smallGraphics.textFont(font);
   smallGraphics.background('grey')
   smallGraphics.textAlign(LEFT, CENTER)
   smallGraphics.text('bro testing', 5, 32-15)
   smallGraphics.loadPixels(); */
   

   if (x !== undefined && y !== undefined)
      //image(smallGraphics, -x, y, scaledWidth, scaledHeight)
      image(smallGraphics, x, y, scaledWidth, scaledHeight);
   //image(smallGraphics, 0, 0, scaledWidth, scaledHeight)


  /*  let now = new Date();
   count++
   total = now - bruh

   idx++
   idx %= 10 */

   /* if (idx == 9)
      console.log('avg delat', total / count) */
   //smallGraphics.noSmooth()

   noSmooth();

}

window.onresize = function () {
   // assigns new values for width and height variables
   w = window.innerWidth;
   h = window.innerHeight;
   //canvas.size(w,h);
   //resizeCanvas(w, h)
   // TODO CAMERON RECALCULATE X AND Y AND SIZE AND SHIT
}

let frame2;

let d = 1;
let elapsedFrames = 0;
let lastMax = 0;
let lastLowest = 255;
let numFramesToCalcBrightness = 40;

async function sendFrames() {
   let frame;
   bands = calcBands(4, 0, 255);
   let forst = bands.at(0);

   interval = setInterval(() => {
      frame = [];



      for (let _y = 0; _y < desiredHeight; _y++) {
         for (let _x = 0; _x < desiredWidth; _x++) {
            const i = 4 * d * (_y * d * smallGraphics.width + _x);
            let avg = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2] + smallGraphics.pixels[i+1]) / 3) & 0xFF;
            //let avg = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2]) / 2) & 0xFF
            if (elapsedFrames < numFramesToCalcBrightness) {
               frame.push(0, 0, 0)
               //console.log(avg, lastMax)
               if (avg > lastMax) {
                  lastMax = avg;
                  //console.log(avg)
               }
               if (elapsedFrames > (numFramesToCalcBrightness/2) && avg < lastLowest)
                  lastLowest=avg   
               continue
            }

            /* const rand = Math.random() < 0.001;
               if (rand) {
                  frame.push(Math.random()*255&0xFF, Math.random()*255&0xFF, Math.random()*255&0xFF)
                  continue;
            } */
            
            bands.some((range, idx) => { 
               let isBelow = avg < range.start;
               let isAbove = avg > range.end;

               if (isBelow) {
                  if (idx == 0) {
                     //frame.push(0, 0, 0)
                     frame.push(0, 0, 255)
                     return true
                  }
               } else if (isAbove) {
                  if (idx+1 == bands.length) {
                     let okcool = (avg*range.val)&0xFF
                  /* let r = smallGraphics.pixels[i]*range.val*0&0xFF;
                  let g = smallGraphics.pixels[i+1]*range.val*0.8&0xFF;
                  let b = smallGraphics.pixels[i+2]*range.val*0.1&0xFF; */
                     frame.push(okcool, okcool, okcool)
                     return true
                  }
               } else if (!isBelow && !isAbove){ // avg >= range.start && avg <= range.end
                  let okcool = (avg*range.val)&0xFF
                  /* let r = smallGraphics.pixels[i]*range.val*0&0xFF;
                  let g = smallGraphics.pixels[i+1]*range.val*0.8&0xFF;
                  let b = smallGraphics.pixels[i+2]*range.val*0.1&0xFF; */
                  frame.push(okcool, okcool, okcool)
                  return true
               }
            })
         }
      }

      if (elapsedFrames < numFramesToCalcBrightness && lastMax != 0) {
         elapsedFrames++
         //window.bands = calcBands(4, 255)
         //console.log(bands)
      }

      if (elapsedFrames == numFramesToCalcBrightness) {
         /* lastMax=255
         lastLowest=0 */
         elapsedFrames++

         bands = calcBands(5, lastLowest, lastMax)
      }

      sendMessage2(`{"pixels": "[${frame.join(', ')}]"}`)
   }, 50, "param");
}

frames = [];
async function sendFramesOLD() {
   //const d = smallGraphics.pixelDensity();
   const d = 1;
   lfoIdxr = 0;
   lfoIdxg = 0;
   lfoIdxb = 0;
   direction = 1;
   speedr = 1.2;
   speedg = 1.7;
   speedb = 1.4;
   range = 3;

   setInterval(() => {
      let frame = [];

      //console.log('sending')

      //console.clear()
      // this way for human readability
      /* for (let _x = 0; _x < desiredWidth; _x++) {
         //let strrr = '';

         for (let _y = desiredHeight-1; _y >= 0; _y--) {
            const i = 4 * d * (_y * d * smallGraphics.width + _x);
            sum = (smallGraphics.pixels[i] + smallGraphics.pixels[i + 1] + smallGraphics.pixels[i + 2])
            strrr += (sum / 3).toFixed(0).toString().padStart(3, '0') + ' ';
         
            frame.push(smallGraphics.pixels[i], smallGraphics.pixels[i+1], smallGraphics.pixels[i+2])
         }

         console.log(strrr)
      } */


        
            //let strrr = '';
   
         // this way for RPI_LED
         for (let _y = 0; _y < desiredHeight; _y++) {
            for (let _x = 0; _x < desiredWidth; _x++) {

               const i = 4 * d * (_y * d * smallGraphics.width + _x);   
               /* let diff = 100 // cool red blue
               frame.push(Math.max(0, smallGraphics.pixels[i]-diff-20), Math.max(0, smallGraphics.pixels[i+1]-diff-60), Math.max(0, smallGraphics.pixels[i+2]-diff))
               continue; */

               let avg = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2] + smallGraphics.pixels[i+1]) / 3) & 0xFF

               //wow = avg;
               // avg of red blue
               //let dude = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2]) / 2).toFixed(0);
               
               
               // 3 band filtered
               /* let wow = Number((dude > 160 ? Math.min(255, Number(dude)+0) :
                  (dude < 90  ? Math.max(0, dude-40) :
                        Math.max(0, dude -200)))).toFixed(0); */
                        let wow = Number((avg > 160 ? Math.min(255, Number(avg)+0) :
                  (avg < 90  ? Math.max(0, avg-40) :
                        Math.max(0, avg -200)))).toFixed(0);


               /* let dude = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2] + smallGraphics.pixels[i+1]) / 3).toFixed(0);
               if (isNaN(dude)) {
                  frame.push(0, 0, 0)
                  continue
               }

               let wow = Number((dude > 160 ? Math.min(255, Number(dude)+0) :
                  (dude < 90  ? Math.max(0, dude-40) :
                        Math.max(0, dude -200)))).toFixed(0); */
               frame.push(wow??0, wow??0, wow??0)
               continue



               // really good!! inverted ish, use wow for all 3
               /* dude2 = Math.max(0,120-dude)
               wow = Math.max(0, 255-dude)
               if (dude > 126)
                  wow = Math.max(0, dude-200).toFixed(0)
               else if (dude < 2)
                  wow = Math.min(255, wow+0) // try +20
               else 
                  wow = Math.max(0, dude2*3)
               
               frame.push(wow??0, wow??0, wow??0)
               continue */

               
               // simple 2 band
               //let dude = ((smallGraphics.pixels[i] + smallGraphics.pixels[i+2] + smallGraphics.pixels[i+1]) / 3).toFixed(0);
               //wow = dude > 180 ? Math.min(255, dude) : Math.max(0, dude-100);
               /* let idk = Math.min(255, (Number(wow)*0.6)).toFixed(0)
               frame.push(wow??0, idk??0, idk??0)
               continue */

               /* wow = 0
               if (avg > 120)
                  wow = Math.max(avg * 1.4, 255)&0xFF;
               else if (avg > 60 && avg < 100)
                  wow = (avg * 1)&0xFF;
               else if (avg <= 60)
                  wow = (avg * 0.2)&0xFF; */

               /* let idk = Math.min(255, (Number(wow)*0.6)).toFixed(0)
               frame.push(wow??0, idk??0, idk??0) */

               //wow = avg*1.2 & 0xFF;

               // lfo
               wowr =
                  (((smallGraphics.pixels[i] > lfoIdxr-range && smallGraphics.pixels[i] < lfoIdxr+range) ?
                  Math.min(wow+50, smallGraphics.pixels[i]*1.2) : 0)+wow)/2>>0;

               wowg =
                  (((smallGraphics.pixels[i+1] > lfoIdxg-range && smallGraphics.pixels[i+1] < lfoIdxg+range) ?
                  Math.min(wow+50, smallGraphics.pixels[i+1]*1.2) : 0)+wow)/2>>0;

               wowb =
                  (((smallGraphics.pixels[i+2] > lfoIdxb-range && smallGraphics.pixels[i+2] < lfoIdxb+range) ?
                  Math.min(wow+50, smallGraphics.pixels[i+2]*1.2) : 0)+wow)/2>>0;

               //wow2 = Number((wow2 + wow)/2).toFixed(0)
               /* wow2 = Number(Math.min(255, Math.max(wow2, wow)))
               wow = (wow * 0.5).toFixed(0)
               wow2 = (wow2 * 0.5).toFixed(0) */

               //frame.push(Math.min(90, Number(wow2)*1.5).toFixed(0)??0, wow2??0, wow2??0)
               frame.push(wowr??0, wowg??0, wowb??0)
            }
   
            //console.log(strrr)
         }

      lfoIdxr += speedr * direction
      lfoIdxg += speedg * direction
      lfoIdxb += speedb * direction
      if ((lfoIdxr + speedr * direction) > 255 || (lfoIdxr + speedr * direction) < 0 ) {
         //direction *= -1;
         lfoIdxr = 0
      } 
      if ((lfoIdxg + speedg * direction) > 255 || (lfoIdxg + speedg * direction) < 0 ) {
         //direction *= -1;
         lfoIdxg = 0
      }
      if ((lfoIdxb + speedb * direction) > 255 || (lfoIdxb + speedb * direction) < 0 ) {
         //direction *= -1;
         lfoIdxb = 0
      }

      sendMessage2(`{"pixels": "[${frame.join(', ')}]"}`)
      /* frames.push(`[${frame.join(', ')}]`)
      if (frames.length == 10) {
         sendMessage2(`{"frames": [${frames.join(', ')}]}`)
         frames = [];
      } */
   }, 50, "param")
}

lowest = 0;

function randomIntFromInterval(min, max) { // min and max included 
   return Math.floor(Math.random() * (max - min + 1) + min);
 }
 
 const rndInt = randomIntFromInterval(1, 6);

function calcBands(numbands, min, max) {
   numbands = Number(numbands);
   max = Number(max);

   /* let colors = []
   let max2 = 100;
   let numbands2 = numbands
   let bruh = max2/numbands2
   for (let c = max2; c > 0; c-=bruh) {
      colors.push(c)
   }
   console.log(colors) */

   let colors = []
   let mul = 1/numbands
   let func = (x) => {
      return 1-Math.pow(Math.abs(Math.sin((Math.PI*((mul*x+1)/2)))),0.5)
   }

   for (let i = numbands; i >= 0; i--) {
      console.log(i)
      if (i == numbands)
         colors.push(1)
      else
         colors.push(func(i).toFixed(2))
   }

   //let gapSize = 50
   //let gapSize = ((max/(numbands * 2))>>0)
   let gapSize = 0;
   //max = lastMax;
   let bandWidth = ((((max-lastLowest) - (numbands - 1) * gapSize)/numbands)>>0) // Math.floorDiv((max - (numbands - 1) * gapSize), numbands)
   let bands = new Array()

   let start = min

   for (let bandIdx = 0; bandIdx < numbands; bandIdx++) {
      let bandStart = start
      let bandEnd = bandStart + bandWidth


      let range = {
         start: bandStart,
         end: (bandIdx == numbands-1) ? max : bandEnd, // replace 255 with max
         //val: (bandIdx == numbands-1) ? max-1 : ((max/numbands)>>0)*(bandIdx+1)
         val: colors[numbands-bandIdx-1]
      };

      bands.push(range)

      start = bandEnd + gapSize
   }

   console.log(bands)

   return bands
}

function rampedVals(numbands) {
   let vals = [];
   let mul = 1/numbands;

   let i1 = (Math.PI*((mul*x+1)/2));
   let i2 = (Math.sin(i1))
   let i3 = Math.abs(i2)*0.5
   let i4 = 1-i3;
}