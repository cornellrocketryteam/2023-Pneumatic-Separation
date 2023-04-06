#include <Arduino.h>
#include <String.h>

#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_MCP9808.h>

#include "main.h"

#include "MS5611.h"

#include <RadioLib.h>

using namespace std;

SYSTEMS current_state = {

	{0, 0, 0, 0,
	 false}, // GPS
	{0, 0, 0, 0, 0,
	 false}, // ALTIMETER
	{0, 0, 0,
	 false}, // ACCEL
	{0, 0, 0,
	 0, 0, 0,
	 false}, // GYRO
	{0, 0, 0,
	 false}, // MAG
	{0, 1000,
	 false}, // SD
	{RADIOLIB_ERR_NONE,
	 false} // RF

};


// MS5611 Altimeter
MS5611 ms5611;

//MCP9808 Thermometer
Adafruit_MCP9808 mcp9808;
#define SEA_LEVEL_PRESURE 102106.3 // Calculated using Duffield hall senior meche lounge as reference, 246.48 m in elevation

#define SEPARATION 22

#define LED 25

bool apogee = false; // flag used to mark apogee detection
bool armed = false;	 // flag used to mark arrival at arming altitude

double alt_sum = 0.0;
int next_alt = 0;

double filtered_alt1 = 0.0; // The altitude two before the current reading
double filtered_alt2 = 0.0; // The previous filtered altitude
double filtered_alt3 = 0.0; // the most recent relative alt run through a boxcar filter

bool deployed = false;
unsigned long drogue_deploy_start;


void init_sensors()
{
  Wire.setSDA(21);
  Wire.setSCL(20);
	Wire.begin();


	// MS5611 Altimeter

  if (ms5611.begin())
	{
		// This check actually freezes the processor if it fails,
		// Need to fix the library to fail gracefully
		current_state.ALTIMETER.INIT = true;
		current_state.ALTIMETER.REF_PRESURE = ms5611.readPressure();
	}

	if (!current_state.ALTIMETER.INIT)
    Serial.print("ALTIMETER UNABLE TO INITIALIZE\n");

	//	Log.critical("ALTIMETER UNABLE TO INITIALIZE\n");
}

void poll_sensors()
{

	// MS5611 Altimeter
	if (current_state.ALTIMETER.INIT)
	{
		long realPressure = ms5611.readPressure();
		current_state.ALTIMETER.TEMP = ms5611.readTemperature();
		current_state.ALTIMETER.PRESSURE = ms5611.readPressure();
		current_state.ALTIMETER.ABS_ALTITUDE = ms5611.getAltitude(realPressure, SEA_LEVEL_PRESURE);
		current_state.ALTIMETER.REL_ALTITUDE = ms5611.getAltitude(realPressure, current_state.ALTIMETER.REF_PRESURE);
		alt_sum += current_state.ALTIMETER.REL_ALTITUDE;
		next_alt += 1;
	}
}

bool apogee_detect()
{
	double diff1 = filtered_alt1 - filtered_alt2;
	double diff2 = filtered_alt2 - filtered_alt3;
	bool mag1 = (diff1 > -1.0 && diff1 < 0.0) || (diff1 < 1.0 && diff1 > 0.0);
	bool mag2 = (diff2 > -1.0 && diff2 < 0.0) || (diff2 < 1.0 && diff2 > 0.0);

	if (filtered_alt1 > filtered_alt2 && filtered_alt2 > filtered_alt3)
	{
		//Log.debug("Apogee reached");
		return true;
	}
	return false;
}

void run_filter()
{
	if (next_alt % 10 == 0)
	{
		filtered_alt1 = filtered_alt2;
		filtered_alt2 = filtered_alt3;
		filtered_alt3 = alt_sum / 10.0;
		alt_sum = 0;
	}
}

bool seperation_logic(bool a)
{

	if (!a && current_state.ALTIMETER.REL_ALTITUDE > 1524) // above 5000 feet (1524 m)
	{
		a = true;
		//Log.debug("Arming altitude reached (5000 ft)");
	}

	if (a && !deployed && current_state.ALTIMETER.REL_ALTITUDE < 914.4) // below 3000 feet
	{
		//Log.debug("Beginning wire heating");
		digitalWrite(SEPARATION, HIGH);
		drogue_deploy_start = millis();
		deployed = true;
	}

	if (deployed && millis() - drogue_deploy_start > 12000)
	{
		//Log.debug("End wire heating");
	  digitalWrite(SEPARATION, LOW);
	}

	return a;
}

void setup()
{
	Serial.begin(115200);
	// init_sd();
	// init_rf();
	pinMode(SEPARATION, OUTPUT);
  pinMode(LED, OUTPUT); // LED
  init_sensors();
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED,LOW);
}

void loop()
{
  Serial.print("We're here\n");
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  delay(500);
	poll_sensors();
	run_filter();
	if (armed && !apogee && next_alt % 10 == 0)
	{
		apogee = apogee_detect();
	}
	armed = seperation_logic(armed);
}
