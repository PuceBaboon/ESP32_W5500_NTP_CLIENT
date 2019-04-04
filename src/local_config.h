/*
 *   $Id: local_config.h,v 1.5 2019/04/04 23:06:53 gaijin Exp $
 *
 * Change settings below to customize for -YOUR- local network.
 * 
 */


/*
 * W5500 "hardware" MAC address.
 */
uint8_t eth_MAC[] = { 0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x01 };


/*
 * Define the static network settings for this gateway's ETHERNET connection
 * on your LAN.  These values must match YOUR SPECIFIC LAN.  The "eth_IP"
 * is the IP address for this gateway's ETHERNET port.
 */
IPAddress eth_IP(192, 168, 1, 100);		// *** CHANGE THIS to something relevant for YOUR LAN. ***
IPAddress eth_MASK(255, 255, 255, 0);		// Subnet mask.
IPAddress eth_DNS(192, 168, 1, 2);		// *** CHANGE THIS to match YOUR DNS server.           ***
IPAddress eth_GW(192, 168, 1, 1);		// *** CHANGE THIS to match YOUR Gateway (router).     ***


#define RESET_P	26				// Tie the Wiz820io/W5500 reset pin to ESP32 GPIO26 pin.

const uint16_t localPort = 55432;		// Local port for UDP packets.

/*
 * Choose the NTP server pool for your geographical region for best
 * performance (fewer lost packets).
 *
 * *** Uncomment only one of the following "timeServer[]" defines. ***
 */
const char timeServer[] = "pool.ntp.org";		// Default NTP server pool.
// const char timeServer[] = "africa.pool.ntp.org";		// Regional server pool.
// const char timeServer[] = "asia.pool.ntp.org";		// Regional server pool.
// const char timeServer[] = "europe.pool.ntp.org";		// Regional server pool.
// const char timeServer[] = "north-america.pool.ntp.org";	// Regional server pool.
// const char timeServer[] = "oceania.pool.ntp.org";		// Regional server pool.
// const char timeServer[] = "south-america.pool.ntp.org";	// Regional server pool.
// const char timeServer[] = "time.nist.gov";			// Original example target server (least preferred).

const uint8_t SLEEP_SECS = 15;			// Number of seconds to sleep between queries to the time
						// server. Please don't set this any lower than 10 unless
						// timeServer[] is a local NTP server on -your- network.


