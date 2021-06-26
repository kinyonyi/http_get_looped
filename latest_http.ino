#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial SIM900(3, 5); // configure software serial port
int SittingRoom = 8;
int Bedroom = 10;
int Security = 11;

char buffer[80];
byte pos = 0;


//resetting the buffer here
void resetBuffer() {
  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}


void setup() {

  pinMode(SittingRoom, OUTPUT);
  pinMode(Bedroom, OUTPUT);
  pinMode(Security, OUTPUT);
  
  SIM900.begin(9600);
  Serial.begin(9600);
  Serial.println("power up" );
  delay(15000);

}

void loop()
{
  Serial.println("SubmitHttpRequest - started" );
  SubmitHttpRequest();
  Serial.println("SubmitHttpRequest - finished" );
  delay(1000);
}

void SubmitHttpRequest()
{
  SIM900.println("AT+CSQ"); // Signal quality check

  delay(2000);

  ShowSerialData();// this code is to show the data from gprs shield, in order to easily see the process of how the gprs shield submit a http request, and the following is for this purpose too.

  SIM900.println("AT+CGATT?"); //Attach or Detach from GPRS Support
  delay(2000);

  ShowSerialData();
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
  delay(2000);

  ShowSerialData();

  SIM900.println("AT+SAPBR=3,1,\"APN\",\"internet\"");//setting the APN, Access point name string
  delay(2000);

  ShowSerialData();

  SIM900.println("AT+SAPBR=1,1");//setting the SAPBR
  delay(10000);

  ShowSerialData();

  SIM900.println("AT+SAPBR=2,1");//getting ip address assigned
  delay(1000);

  ShowSerialData();

  SIM900.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+HTTPPARA=\"CID\",1"); //init the HTTP request
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+HTTPPARA=\"URL\",\"http://www.home-system.cynaut-tech.com/all-settings?username=engdave\"");
  //SIM900.println("AT+HTTPPARA=REDIR,1");
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+HTTPACTION=0");//submit the request
  delay(5000);
  ShowSerialData();

  SIM900.println("AT+HTTPREAD");// read the data from the website you access
  delay(300);
  ShowSerialData();

  SIM900.println("AT+HTTPTERM");
  ShowSerialData();
  delay(300);

  /*
  SIM900.println("AT+SAPBR=0,1");
  delay(5000);
  ShowSerialData();
  delay(5000);
  */
  //SIM900.println("");
  //delay(100);
}

void ShowSerialData() {
  while (SIM900.available() != 0) {
    Serial.write(char (SIM900.read()));
    resetBuffer();
  }
}
