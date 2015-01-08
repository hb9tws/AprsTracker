#include "Arduino.h"

namespace GPS
{
  
  // Max length of an NMEA data record is 80 characters plus some delimiters
  const byte BUFFLEN = 85;
  
  class Decoder
  {
     public:
       Decoder();

       void Parse(byte x);
       boolean IsValid();
       
       String Time();
       String Date();
       char* Longitude();
       char* Latitude();
       double Speed();
       double Angle();
       
     protected:
       char _buffer[BUFFLEN+1];
       byte _cursor;
       
       boolean _valid;
       String _time;
       String _date;
       char   _state;

       char _longitude[10];
       char _latitude[9];       
       double _speed;
       double _angle;
       
       void DecodeData();
       
       void DecodeTime(char* data);
       void DecodeDate(char* data);
       void DecodeState(char* data);
       void DecodeLongitude(char* data);
       void DecodeLatitude(char* data);
       void DecodeNorthSouth(char* data);
       void DecodeEastWest(char* data);
       void DecodeSpeed(char* data);
       void DecodeAngle(char* data);
  };
}
