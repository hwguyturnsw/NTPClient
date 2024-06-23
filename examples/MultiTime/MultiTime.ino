/*
****************************************************************
    File: MultiTime.ino
    Author: ejc
    Created: 06.22.2024
    Description: Multiple time functions with forked and updated header and cpp file. This allows you to have a formatted time based on the time offset AND have UTC time at the same time.

    Modifications:
    [06.22.2024] - [ejc] - [Initial release]
****************************************************************
*/

#include <NTPClient.h>
// Change next line to use with another board/shield
#include <WiFi.h> // Works with ESP32
//#include <WiFi101.h> // For WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "<SSID>";		// Replace <SSID> with your WiFi's SSID
const char *password = "<PASSWORD>";	// Replace <PASSWORD> with your WiFi's password
int loop_count = 0;						// Loop counter for updating NTP

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be changed later with setTimeOffset() ). 
// Additionally you can specify the update interval (in milliseconds, can be changed using setUpdateInterval() ).

// NTP	  Name	 	 UDP      NTP Server        Offset  Update Interval
NTPClient timeClient(ntpUDP, "us.pool.ntp.org", -18000, 60000);

void setup(){
	Serial.begin(115200);	// Start the serial port
	delay(100);

	WiFi.begin(ssid, password);		// Start WiFi

	while ( WiFi.status() != WL_CONNECTED ) {
		delay (500);
		Serial.print(".");
	}

	timeClient.begin();		// Start the TimeClient
	timeClient.update();	// Update the time
}

void loop() {
	// Print Local time
	Serial.print(F("Local Time: "));
	Serial.println(timeClient.getFormattedTime());
	// Print UTC Time (No Offset)
	Serial.print(F("UTC Time: "));
	Serial.println(timeClient.getFormattedTimeUTC());
	
	// Time Update Loop Counter (We don't have to poll the time server constantly. Over time this will drift so we will check again in two days time.
	if (loop_count > 172800){	// Update the time via NTP if 2 days have passed.
		Serial.print(F("Updating Time From NTP...\n"));
		timeClient.update();
		Serial.print(F("Time has just been updated!\n\n"));
	}
		else {
			Serial.print(F("The loop counter hasn't reached enough cycles to update the time.\n\n"));
		}
	loop_count++;		// Increment the loop counter
	delay(1000);
}