let video;
let framesData = []; // Array to store pixel arrays for each frame
let offscreenCanvas; // Offscreen canvas for resizing frames
let frameIndex = 0; // Current frame index being processed
let isProcessing = false;
let fps = 30; // Desired frame rate for extraction
let totalFrames = 0; // Total number of frames to process
let playhead = 0;

function setup() {
   createCanvas(window.innerWidth, window.innerHeight);
   background(220);
   frameRate(30)
   noSmooth()
   pixelDensity(1)

   scaledHeight = 32 * (window.innerWidth / 64)
   scaledWidth = window.innerWidth;

   // Create an offscreen canvas for resizing frames to 64x32
   offscreenCanvas = createGraphics(64, 32);
   offscreenCanvas.noSmooth();
   offscreenCanvas.pixelDensity(1)

   // Add a file input element
   let fileInput = createFileInput(handleFile);
   fileInput.position(10, 10);
}

function handleFile(file) {
   if (file.type === 'video') {
      if (video) {
         video.remove(); // Remove any previously loaded video
      }

      video = createVideo(file.data, onVideoLoad);
      console.log(video.size())
      //video.pixelDensity(1)
      video.size(64, 32)
      //video.hide(); // Hide the video element
   } else {
      console.log('Not a video file!');
   }
}

function onVideoLoad() {
   // Start at the beginning of the video
   totalFrames = Math.floor(video.duration() * fps); // Total frames to extract
   console.log(`Total frames to extract: ${totalFrames}`);
   isProcessing = true;
   document.querySelector('video').addEventListener('seeked', () => {
      //console.log('onseek handkerr')
      processFrame();
      frameIndex++;
      extractFrames(); // Continue to the next frame
      /* if (Math.abs(video.time() - currentTime) < 0.05) { // Ensure seek is accurate
        processFrame();
        frameIndex++;
        extractFrames(); // Continue to the next frame
      } else {
        console.log(`Seeking failed for frame ${frameIndex}, retrying...`);
        extractFrames(); // Retry seeking
      } */
   });
   //extractFrames(); // Start extracting frames
   video.time(0);
}

function extractFrames() {
   if (frameIndex >= totalFrames) {
      console.log('All frames extracted!', framesData);
      //sendFrames()
      isProcessing = false;
      return; // Stop when all frames are processed
   }

   let currentTime = frameIndex / fps; // Time for the current frame
   video.time(currentTime); // Seek to the specific time
}

function processFrame() {
   // Draw the current video frame onto the offscreen canvas
   //offscreenCanvas.image(video, 0, 0, offscreenCanvas.width, offscreenCanvas.height);
   // Access the pixel array of the resized frame
   //offscreenCanvas.loadPixels();
   //let pixelArray = Array.from(offscreenCanvas.pixels); // Copy pixel data
   video.loadPixels()
   let pixelArray = Array.from(video.pixels)
   
   framesData.push(pixelArray); // Store the pixel data

   // Optionally, display the frame on the main canvas for debugging
   image(video, 0, 0, width, height);

   //console.log(`Processed frame ${frameIndex}`);
}

function draw() {
   // Update the UI to indicate processing status
   if (isProcessing) {
      fill(0);
      textAlign(CENTER);
      textSize(16);
      text(`Processing frame ${frameIndex} of ${totalFrames}...`, width / 2, height - 20);
   } else if (framesData.length > 0) {
      background(0)
      fill(0);
      textAlign(CENTER);
      textSize(16);
      text('Processing complete!', width / 2, height - 20);
      offscreenCanvas.loadPixels()
      for (let px = 0; px < framesData[playhead].length; px++) {
         offscreenCanvas.pixels[px] = framesData[playhead][px];
      }

      offscreenCanvas.updatePixels()

      image(offscreenCanvas, 0, 0, scaledWidth, scaledHeight)

      /* loadPixels()
      for (let px = 0; px < framesData[playhead].length; px++) {
         pixels[px] = framesData[playhead][px];
      }
      updatePixels() */
      playhead++;
      playhead %= framesData.length
   }
}



async function sendFrames() {
   let frame;

   
   interval = setInterval(() => {
      frame = [];
      offscreenCanvas.loadPixels()

      for (let _y = 0; _y < 32; _y++) {
         for (let _x = 0; _x < 64; _x++) {
            const i = 4 * 1 * (_y * 1 * offscreenCanvas.width + _x);
            frame.push(offscreenCanvas.pixels[i],offscreenCanvas.pixels[i+1], offscreenCanvas.pixels[i+2])
         }
      }

      sendMessage2(`{"pixels": "[${frame.join(', ')}]"}`)
   }, 50, "param");
}