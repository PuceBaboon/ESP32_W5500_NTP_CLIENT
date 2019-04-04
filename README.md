#### ESP32_W5500_NTP_CLIENT
An implementation of the Ethernet library UdpNtpClient example program ...for the ESP32 and W5500


#### What is this and what does it do?
This is just a simple, working example of the Ethernet UdpNtpClient example program to get you going with the ESP32 and hardwired Ethernet, using the common (and easily available) W5500 module.  The hardware interconnection list (pin-to-pin) is included below.

It only initializes and uses the hardwired Ethernet connection, not the WiFi.  It will connect to the specified NTP server and display the UTC time, then go into a short sleep loop (printing occasional "." characters, so that you can see it's still running), before repeating.

### The code
This is a PlatformIO build (and if you haven't tried PlatformIO yet, you really should, it's great!), but is easily adapted to the Arduino IDE, if that happens to be your weapon of choice.

Under PlatformIO, the platformio.ini file will automatically download and install the correct Ethernet library for you when you type "pio run" for the first time.

The setup() section of the code does have some clunky hardware checks, so pay attention to the serial output from your ESP32 when you first run it; it will try to guide you to solutions for (simple!) hardware issues.

**You do** need to update the src/local_config.h file to fit with your specific home network; it will not work unless it can look up the IP address of the remote NTP server (via DNS) and find a valid route to it (your gateway).


#### DHCP
The original ESP8266 version of this project timed-out and crashed when configured to use DHCP rather than a hard-coded IP.  I would recommend you stick with a hard-coded IP for initial testing, as I haven't tried DHCP at all, yet (let me know how it goes if you do).


### Practical notes
The physical connections between the ESP32 and W5500 are different, but simpler than the ESP8266 version (you don't need to buffer SCS chip-select signal on the ESP32).

-  **GPIO23**   <-->   **MOSI**
-  **GPIO19**   <-->   **MISO**
-  **GPIO18**   <-->   **SCLK**
-  **GPIO5**    <-->   **SCS**
-  **GPIO26**   <-->   **RESET**

As always, you do need a good solid DC supply for both boards and a good ground connection between them in addition to the data lines listed above.

