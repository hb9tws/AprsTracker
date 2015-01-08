#include "gps.h"

using namespace GPS;

Decoder::Decoder()
{
  _cursor = 0;
  memset(_buffer,0,BUFFLEN+1);
  
  _state = 0;
  _valid = false;
  
}

void Decoder::Parse(byte x)
{
  // handle beginning of message and also avoid buffer overlow
  if ( _cursor >= BUFFLEN || x == '$' )
  {
     _cursor = 0;
     memset(_buffer,0,BUFFLEN);
  }

  // remember incoming data  
  _buffer[_cursor++] = (char) x;

  // detect end of message
  if ( x == '\r' )
  {
     DecodeData();  
  }
}


void Decoder::DecodeData()
{
  // split message into the individual tokens
  const char* delimiter = ",";
  char* token = strtok(_buffer, delimiter);
  
  int field = 0;
  if ( token != NULL )
  {
     // We understand only GPRMC messages and ignore everything else
     if (field == 0 && strcmp( "$GPRMC", token) == 0 )
     {
       _valid = false;
       while (token != NULL)  
       {
         switch(field)
         {
           case 1:
             DecodeTime(token);
             break;

           case 2:
             DecodeState(token);
             break;

           case 3:
             DecodeLatitude(token);
             break;
           case 4:
             DecodeNorthSouth(token);
             break;
           case 5:
             DecodeLongitude(token);
             break;
           case 6:
             DecodeEastWest(token);
             break;

           case 7:
             DecodeSpeed(token);
             break;
           case 8:
             DecodeAngle(token);
             break;

           case 9:
             DecodeDate(token);
             
             // This is the last data field we use. So we just do a simple check for a valid data record. 
             // Using the checksum is not yet implemented.
             if ( _state == 'A' )
               _valid = true;
             else
               _valid = false;
             break;

           default:
             // here is the place for handling addtional data, so far we ignore it
             break;
         }
         token = strtok(NULL,delimiter);
         ++field;
       }
     }
  }
}

boolean Decoder::IsValid()
{
  return _valid;
}

String Decoder::Time()
{
  if (!_valid) return NULL;
  return _time;  
}

String Decoder::Date()
{
  if (!_valid) return NULL;
  return _date;
}

char* Decoder::Longitude()
{
  if (!_valid) return NULL;
  return _longitude;
}

char* Decoder::Latitude()
{
  if (!_valid) return NULL;
  return _latitude;
}

double Decoder::Speed()
{
  if (!_valid) return -1;
  return _speed;
}

double Decoder::Angle()
{
  if (!_valid) return -1;
  return _angle;
}


void Decoder::DecodeTime(char * data)
{
  String str = data;
  if (str.length() >= 6)
  {
     _time = str.substring(0,2);
     _time.concat(":");
     _time.concat( str.substring(2,4));
     _time.concat(":");
     _time.concat( str.substring(4,6));
  }
}

void Decoder::DecodeDate(char * data)
{
  String str = data;
  if (str.length() == 6)
  {
     _date = str.substring(0,2);
     _date.concat(".");
     _date.concat( str.substring(2,4));
     _date.concat(".20");
     _date.concat( str.substring(4,6));
  }
}

void Decoder::DecodeState(char * data)
{
  if (data == NULL) return;
  _state = data[0];
}

void Decoder::DecodeLongitude(char * data)
{
  if (data == NULL) return;
  memcpy(_longitude, data, 8);
  _longitude[8] = 0;
}

void Decoder::DecodeLatitude(char * data)
{
  if (data == NULL) return;
  String str = data;
  memcpy(_latitude, data, 7);
  _latitude[7] = 0;
}

void Decoder::DecodeNorthSouth(char * data)
{
  if (data == NULL) return;
  _latitude[7] = data[0];
  _latitude[8] = 0;
}

void Decoder::DecodeEastWest(char * data)
{
  if (data == NULL) return;
  _longitude[8] = data[0];
  _longitude[9] = 0;
}

void Decoder::DecodeSpeed(char * data)
{
  if (data == NULL) return;
  _speed = atof(data);
}

void Decoder::DecodeAngle(char * data)
{
  if (data == NULL) return;
  _angle = atof(data);
}






