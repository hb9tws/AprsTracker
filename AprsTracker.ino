#define ADC_REFERENCE REF_3V3
#define OPEN_SQUELCH true

#include <LibAPRS.h>
#include "gps.h"


char* BeaconComment;
int BeaconIntervall;
int BeaconCounter;
String Timestamp;

GPS::Decoder gpsDecoder;


// Callback function
void aprs_msg_callback(struct AX25Msg *msg)
{
  // As tracker we ignore all incoming packets ;-)
}

void setup()  
{
  BeaconCounter = 1;

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // Configure
  BeaconComment = "APRS Tracker with MicroModem - Software Test";
  BeaconIntervall = 60;
  
  // Setup APRS
  APRS_init(ADC_REFERENCE, OPEN_SQUELCH);
  APRS_setPreamble(1200);
  APRS_setTail(100);
  APRS_setCallsign("HB9TWS", 5);

  APRS_printSettings();
  Serial.print(F("Free RAM:     ")); Serial.println(freeMemory());
}

void loop() // run over and over
{
  if (Serial.available())
  {
      gpsDecoder.Parse(Serial.read() );
  }
  
  
  if ( gpsDecoder.IsValid() && !Timestamp.equals(gpsDecoder.Time()) )
  {
    BeaconCounter--;
    Timestamp = gpsDecoder.Time();
    
    ShowCounter();
    ShowGPSData(); 
    
    if ( BeaconCounter <= 0 )
    {
       BeaconCounter = BeaconIntervall;
       SendBeacon();
    }
  }
}


void SendBeacon()
{
     // Update location 
     APRS_setLat( gpsDecoder.Latitude() );
     APRS_setLon( gpsDecoder.Longitude() );
  
     // And send the update
     Serial.println();
     Serial.println(F("<<< Sending APRS Beacon >>>"));
     Serial.println();
     APRS_sendLoc( BeaconComment, strlen( BeaconComment));  
}

void ShowCounter()
{
    Serial.println();
    Serial.print(F("Next beacon : "));
    Serial.print( BeaconCounter );
    Serial.println(F(" seconds to go."));
    Serial.println(F("-------------------------------"));
}

void ShowGPSData()
{
    Serial.print(F("Time        : "));
    Serial.println( gpsDecoder.Time() );
    Serial.print(F("Date        : "));
    Serial.println( gpsDecoder.Date() );
    Serial.print(F("Longitude   : "));
    Serial.println( gpsDecoder.Longitude() );
    Serial.print(F("Latitude    : "));
    Serial.println( gpsDecoder.Latitude() );
    Serial.print(F("Speed(kn)   : "));
    Serial.println( gpsDecoder.Speed() );
    Serial.print(F("Track angle : "));
    Serial.println( gpsDecoder.Angle() );
}

