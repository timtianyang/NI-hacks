#include <ESP8266WiFi.h>
const char* ssid = "AirPennNetSucks";
const char* password = "88888888";

const int RED_LED=2;
//const int GREEN_LED=2;
const int an1_enable=12;
const int an2_enable=13;
const int an3_enable=5;
const int an4_enable=4;

String Rcolors[]={"FF0000","FA0000","F60000","F20000","F00000",
"EF0000","EA0000","E60000","E20000","E00000",
"DF0000","DA0000","D60000","D20000","D00000","CE0000"};
String Gcolors[]={"00FF00","00FA00","00F600","00F200","00F000",
"00EF00","00EA00","00E600","00E200","00E000",
"00DF00","00DA00","00D600","00D200","00D000","00CE00"};

int pressureMap[5];
int left=0;
int right=0;
int center=0;
int vb=0;

WiFiServer server(80);

void Data(int leftUp,int rightUp,int leftDown,int rightDown);
String getColorString(int sensorReading);

int getAverageReading(int newReading);
int readFromAnalog(int inputPin);


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  //pinMode(DOOR_SWITCH_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
 //pinMode(GREEN_LED, OUTPUT);
 delay(100);
  analogWrite(RED_LED,0);
  


//connect to wifi and start a web server
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {
  
 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(10);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();


  int leftUp=readFromAnalog(1);
  int rightUp=readFromAnalog(2);

  int leftDn=readFromAnalog(3);
  int rightDn=readFromAnalog(4);

  Data(leftUp,rightUp,leftDn,rightDn);

  //Serial.println(leftUp);
  Serial.println(left);
  Serial.println(right);
  Serial.println(center);
 
  Serial.println("=====");


  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<script>");
  client.println("function startup(){");
  client.println("window.navigator.vibrate(5000);");
  client.println("}</script>");
  
  client.println("<meta http-equiv=\"refresh\" content=\"2\" >");

  if(vb>=3){
    vb=0;
    left=0;right=0;center=0;
    analogWrite(RED_LED,0);

    }
  if((left>=10) || (right >=10) || (center >=10)){
     client.println("<body onload='startup()'>");
     Serial.println("!@#!@#@#$@#$#$%$%^%^&^&*");  
     vb++;  
    analogWrite(RED_LED,255);
 
    }
  else {
    client.println("<body>");
    }
 //client.println("<body onload='startup()'>");
  client.println("<p align=\"center\"><font size=\"25\" color=\"blue\">Patient name: Omkar Nalawade</font></p>");
  client.println("<br>");  
  client.println("The sensor values are");
   client.println("<br>");
  //client.println("Up right ");
  client.println("Up right "+(String)rightUp);
  client.println("Up left "+(String)leftUp);
  client.println("Down right "+(String)rightDn);
  client.println("Down left "+(String)leftDn);

  
  client.println("<table width=\"800\" align=\"center\">");
  client.println("<tr>");
  client.println("<td height=\"450\" bgcolor=\"#"+getColorString(leftUp)+"\"></td>");
  client.println("<td height=\"450\" bgcolor=\"#"+getColorString(rightUp)+"\"></td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td height=\"450\" bgcolor=\"#"+getColorString(leftDn)+"\"></td>");
  client.println("<td height=\"450\" bgcolor=\"#"+getColorString(rightDn)+"\"></td>");
  client.println("</tr>");
  client.println("</body>");
  
  client.println("</html>");
  delay(1000);        // delay in between reads for stability
}


String getColorString(int sensorReading){
   int temp=(sensorReading)/64;
  if(temp>=16){temp=15;}
      else if(temp<0){temp=0;}
    if(sensorReading>=512){
       
      
      Serial.println(Rcolors[temp]);
      return Rcolors[temp];}
    else{
     
      Serial.println(Gcolors[temp]);
      return Gcolors[temp];}
}
void Data(int leftUp,int rightUp,int leftDown,int rightDown)
{ 
  if( leftUp> 1000 && leftDown >1000 && rightUp<200 && rightDown <200)
  left++;
  else if (leftUp<200 && leftDown <200 && rightUp>1000 && rightDown >1000)
  right++;
  else if (leftUp>1000 && leftDown>1000 && rightUp>1000 && rightDown >1000)
  center++;

  }


int getAverageReading(int newReading){
  static int analogCounter=0;
  static int analogData[16];
  int i=0;
  int result=0;
  analogData[analogCounter++]=newReading;
  
  analogCounter&=0x0F;

  for(i=0;i<16;i++){
    result+=analogData[i];
  }
  return result/16;
}
int readFromAnalog(int inputPin){
  switch(inputPin){
    case 1:
        pinMode(an1_enable,OUTPUT);
        digitalWrite(an1_enable,HIGH); 
        
        digitalWrite(an2_enable,LOW);
        pinMode(an2_enable,INPUT);
        digitalWrite(an3_enable,LOW);
        pinMode(an3_enable,INPUT);
        digitalWrite(an4_enable,LOW);
        pinMode(an4_enable,INPUT);
        return analogRead(A0);

    case 2:
        pinMode(an2_enable,OUTPUT);
        digitalWrite(an2_enable,HIGH); 
        
        digitalWrite(an1_enable,LOW);
        pinMode(an1_enable,INPUT);
        digitalWrite(an3_enable,LOW);
        pinMode(an3_enable,INPUT);
        digitalWrite(an4_enable,LOW);
        pinMode(an4_enable,INPUT);
        return analogRead(A0);

    case 3:
        pinMode(an3_enable,OUTPUT);
        digitalWrite(an3_enable,HIGH); 
        
        digitalWrite(an2_enable,LOW);
        pinMode(an2_enable,INPUT);
        digitalWrite(an1_enable,LOW);
        pinMode(an1_enable,INPUT);
        digitalWrite(an4_enable,LOW);
        pinMode(an4_enable,INPUT);
        return analogRead(A0);

    case 4:
        pinMode(an4_enable,OUTPUT);
        digitalWrite(an4_enable,HIGH); 
        
        digitalWrite(an2_enable,LOW);
        pinMode(an2_enable,INPUT);
        digitalWrite(an3_enable,LOW);
        pinMode(an3_enable,INPUT);
        digitalWrite(an1_enable,LOW);
        pinMode(an1_enable,INPUT);
        return analogRead(A0);    
  }    
}
