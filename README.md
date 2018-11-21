# Jello Jiggler

_Jello Jiggler_ lets you jiggle a blob of jello remotely.

The project uses:

- An Azure function which shows simple UI and acts as an API endpoint.
- An Adafruite Huzzah board which talks to the API and activates a servo.
- A servo and platform that hold up a plate to hold and shake a jello blob.

## Hardware

### Materials

- Arduino with ESP8266 WiFi board.
- Small RC Servo.
- 3D printed platform.

The project uses an [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/3046) board. This board can be programmed using Arduino IDE, and has WiFi included on it. The board is easy to work with, can be operated off a 3V battery pack or a phone charging "brick" using the USB cable, and is widely available.

The servo I used is a [TowerPro SG92R](http://www.towerpro.com.tw/product/sg92r-7/). It hass small enough power requirements to be rand directly off the arduino board, eliminating the need for extra power circuitry.

In addition to the hadware, some kind of platform had to be mad so that the servo can sit below the plate and would, well, jiggle the jello!. To that end, I 3D printed a platform. The [file can be found  here( https://github.com/nurih/jello-jiggler/raw/master/Jello-Jiggler-Platform.stl ) It's not terribly important that you use this platform, you can just glue the servo to something, or make a cardboard platform - go wild.

## Software

### Azure Function

The code for the Azure function itself is located in the [azure-function/Jiggle directory](azure-function/Jiggle).

The azure function serves two functions: API for the Arduino to check for jiggle commands, and a UI for humans to use when commanding the jiggler.

When deployed to Azure, the function is essentially a tiny website with a single endpoint. The url for your website will be `https://your-function-app-name.azurewebsites.net/api/Jiggle` where `your-function-app-name` would be replaced with whatever you named your function app.

If you hit that endpoint with a browser, you will see a web page letting you click a button to make the jello jiggle. When the __Jiggle__ button is hit, the API enqueues a request for the jiggler to jiggle. The queue is an ephemeral one - it's in memory only. If the app is restarted or moved, the queue disappears. But it works for short periods which is all I needed. 

If you hit the endpoint `https://your-function-app-name.azurewebsites.net/api/Jiggle?pop=true`, you will get back a single queued jiggler request. This is exactly what the Arduino board does: It periodically polls the API to dequeue a command. When it sees a `jiggle=true` command, it executes it.


### Arduino

TBD


