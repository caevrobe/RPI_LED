# Raspberry Pi LED Display
![LED panel live camera demo](./img/camerademo.gif)

Raspberry PI LED panel display with web controller.
 * [display](#display) - C++ RPI LED panel display / websocket client (listener)
 * [server](#server) - Node.js websocket server and frontend LED panel controller (sender/listener)
 * [RPI](#rpi) - Scripts and configs for setting up Raspberry PI
<br><br><br><br>




## Display

### Requirements
* Windows-specific:
   * **SFML** - C++ graphics library used for LED panel emulator. I use SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit, not sure if it matters. Make sure to copy the .dll files from the SFML bin/ directory to the same directory as your built .exe.
* RPI-specific:
   * **[hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)** - C++ Rasperry PI LED panel library.
* General:
   * **make** - Build tool. On Windows I use [gnuwin32's Make for Windows.](https://gnuwin32.sourceforge.net/packages/make.htm)  
   * **g++** - C++ compiler. For Windows, I use TDM-GCC-64. [On Windows, you can also use a toolchain to build for the RPI.](#building-display)
   * **Bash version >= 4** - Required to use the associative arrays in the build script.
   * **[Boost](https://www.boost.org/users/history/version_1_86_0.html)** - C++ Websocket library. You'll need separate versions for Unix and Windows, place these in display/lib/boostWindows/ and display/lib/boostUnix/ (these should contain the boost/ directory.) I use version 1.86.
   * **[simdjson](https://github.com/simdjson/simdjson)** - C++ JSON parsing library.

The display/lib/ directory should look this:

<pre>
lib
 ├── boostUnix
 │   └── boost
 ├── boostWindows
 │   └── boost
 ├── rpi-rgb-led-matrix
 └── simdjson
</pre>

### <h3 id="building-display">Building</h2>
Build with `./build.sh --env=[envname]`. Environments are defined in environment.sh as associative arrays, names prefixed with "cfg_". You can build for the Raspberry Pi on Windows using GCC from the [Prebuilt Windows Toolchain for Raspberry Pi](https://gnutoolchains.com/raspberry/) for your OS version. You *MUST* add -lstdc++ when using this toolchain.

### Running
Start RPI_LED.exe (RPI_LED on RPI). There should be a file called main.conf in the same directory with the address and port for the server the client should connect to:
<pre>
wsAddress=10.0.0.178
wsPort=7777
</pre>
If using SFML on Windows, the directory should also contain the SFML .dll files. 
<br><br><br>



## Server

### Requirements
* **Node.js**
* **[remcoder/Pxxl.js](https://github.com/remcoder/Pxxl.js)** - BDF font library used by the frontend controller. Put this in server/public/resources/js/.
* **Fonts!!** - Not sure if I can add the fonts to the repo. I'm using [artwiz-fonts-wl](https://github.com/whitelynx/artwiz-fonts-wl) and a few from [hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix).

### Running
`node server.js`

<br><br><br>



## RPI
<br><br><br>