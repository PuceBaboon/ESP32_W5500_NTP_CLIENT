/*
 *   $Id: ESP32_NTP.ino,v 1.8 2019/04/04 04:48:23 gaijin Exp $
 *
 *  UDP NTP client example program.
 * 
 *  Get the time from a Network Time Protocol (NTP) time server
 *  Demonstrates use of UDP sendPacket and ReceivePacket
 * 
 *  Created:  04 Sep 2010 by Michael Margolis
 *  Modified: 09 Apr 2012 by Tom Igoe
 *  Modified: 02 Sep 2015 by Arturo Guadalupi
 *  Munged:   04 Apr 2019 by PuceBaboon (for the ESP32 with a W5500 module)
 * 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "local_config.h"	// <--- Change settings for YOUR network here.



const int NTP_PACKET_SIZE = 48;		// NTP time stamp is in the first 48 bytes of the message.
byte packetBuffer[NTP_PACKET_SIZE];	// Buffer for both incoming and outgoing packets.

// A UDP instance to let us send and receive packets over UDP.
EthernetUDP Udp;


/*
 * Wiz W5500 reset function.  Change this for the specific reset
 * sequence required for your particular board or module.
 */
void WizReset() {
    Serial.print("Resetting Wiz W5500 Ethernet Board...  ");
    pinMode(RESET_P, OUTPUT);
    digitalWrite(RESET_P, HIGH);
    delay(250);
    digitalWrite(RESET_P, LOW);
    delay(50);
    digitalWrite(RESET_P, HIGH);
    delay(350);
    Serial.println("Done.");
}


/*
 * This is a crock. It's here in an effort
 * to help people debug hardware problems with
 * their W5100 ~ W5500 board setups.  It's 
 * a copy of the Ethernet library enums and
 * should, at the very least, be regenerated
 * from Ethernet.h automatically before the
 * compile starts (that's a TODO item).
 *
 */
/*
 * Print the result of the hardware status enum
 * as a string.
 * Ethernet.h currently contains these values:-
 *
 *  enum EthernetHardwareStatus {
 *  	EthernetNoHardware,
 *  	EthernetW5100,
 *  	EthernetW5200,
 *  	EthernetW5500
 *  };
 *
 */
void prt_hwval(uint8_t refval) {
    switch (refval) {
    case 0:
        Serial.println("No hardware detected.");
        break;
    case 1:
        Serial.println("WizNet W5100 detected.");
        break;
    case 2:
        Serial.println("WizNet W5200 detected.");
        break;
    case 3:
        Serial.println("WizNet W5500 detected.");
        break;
    default:
        Serial.println
            ("UNKNOWN - Update espnow_gw.ino to match Ethernet.h");
    }
}


/*
 * Print the result of the ethernet connection
 * status enum as a string.
 * Ethernet.h currently contains these values:-
 *
 *  enum EthernetLinkStatus {
 *     Unknown,
 *     LinkON,
 *     LinkOFF
 *  };
 *
 */
void prt_ethval(uint8_t refval) {
    switch (refval) {
    case 0:
        Serial.println("Uknown status.");
        break;
    case 1:
        Serial.println("Link flagged as UP.");
        break;
    case 2:
        Serial.println("Link flagged as DOWN. Check cable connection.");
        break;
    default:
        Serial.println
            ("UNKNOWN - Update espnow_gw.ino to match Ethernet.h");
    }
}


void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n\tUDP NTP Client v3.0\r\n");

    // Use Ethernet.init(pin) to configure the CS pin.
    Ethernet.init(5);           // GPIO5 on the ESP32.
    WizReset();

    /* 
     * Network configuration - all except the MAC are optional.
     *
     * IMPORTANT NOTE - The mass-produced W5500 boards do -not-
     *                  have a built-in MAC address (depite 
     *                  comments to the contrary elsewhere). You
     *                  -must- supply a MAC address here.
     */
    Serial.println("Starting ETHERNET connection...");
    Ethernet.begin(eth_MAC, eth_IP, eth_DNS, eth_GW, eth_MASK);

    delay(200);

    Serial.print("Ethernet IP is: ");
    Serial.println(Ethernet.localIP());

    /*
     * Sanity checks for W5500 and cable connection.
     */
    Serial.print("Checking connection.");
    bool rdy_flag = false;
    for (uint8_t i = 0; i <= 20; i++) {
        if ((Ethernet.hardwareStatus() == EthernetNoHardware)
            || (Ethernet.linkStatus() == LinkOFF)) {
            Serial.print(".");
            rdy_flag = false;
            delay(80);
        } else {
            rdy_flag = true;
            break;
        }
    }
    if (rdy_flag == false) {
        Serial.println
            ("\n\r\tHardware fault, or cable problem... cannot continue.");
        Serial.print("Hardware Status: ");
        prt_hwval(Ethernet.hardwareStatus());
        Serial.print("   Cable Status: ");
        prt_ethval(Ethernet.linkStatus());
        while (true) {
            delay(10);          // Halt.
        }
    } else {
        Serial.println(" OK");
    }

    Udp.begin(localPort);
}

void loop() {
    sendNTPpacket(timeServer);  // Send an NTP packet to the time server.

    // Wait to see if a reply is available.
    delay(1000);
    if (Udp.parsePacket()) {
        // We've received a packet, read the data from it.
        Udp.read(packetBuffer, NTP_PACKET_SIZE);        // Read the packet into the buffer.

        // The timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, extract the two words.
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

        // Next, combine the four bytes (two words) into a long integer.
        // This is NTP time (seconds since Jan 1 1900).
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        Serial.print("Seconds since Jan 1 1900 = ");
        Serial.println(secsSince1900);

        // Now convert NTP time into everyday time.
        Serial.print("Unix time = ");
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800.
        const unsigned long seventyYears = 2208988800UL;
        // Subtract seventy years.
        unsigned long epoch = secsSince1900 - seventyYears;
        // ...and then print Unix time.
        Serial.println(epoch);


        // Print the hour, minute and second.
        Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT).
        Serial.print((epoch % 86400L) / 3600);  // Print the hour (86400 equals secs per day).
        Serial.print(':');
        if (((epoch % 3600) / 60) < 10) {
            // For the first 10 minutes of each hour, insert a leading '0' character.
            Serial.print('0');
        }
        Serial.print((epoch % 3600) / 60);      // Print the minute (3600 equals secs per minute).
        Serial.print(':');
        if ((epoch % 60) < 10) {
            // For the first 10 seconds of each minute, insert a leading '0' character.
            Serial.print('0');
        }
        Serial.println(epoch % 60);     // Print the second.
    }

    // Wait for a while before asking for the time again.
    Serial.print("Sleeping: ");
    for (uint8_t i = 0; i < SLEEP_SECS; i++) {
        delay(1000);			// One second delay "ticks".
	Serial.print(".");
    }
    Serial.println("\n\r---\n\r");

    // You only need to call maintain if you're using DHCP.
    // Ethernet.maintain();
}

// Send an NTP request to the time server at the given address (defined in local_conf.h).
void sendNTPpacket(const char *address) {
    // Set all bytes in the buffer to 0.
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    // Initialize values needed to form NTP request
    // (see http://en.wikipedia.org/wiki/Network_Time_Protocol).
    packetBuffer[0] = 0b11100011;      // LI, Version, Mode
    packetBuffer[1] = 0;        // Stratum, or type of clock
    packetBuffer[2] = 6;        // Polling Interval
    packetBuffer[3] = 0xEC;     // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // All NTP fields have been given values, now
    // send a packet requesting a timestamp.
    Udp.beginPacket(address, 123);      // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}
