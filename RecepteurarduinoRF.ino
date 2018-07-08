#include <VirtualWire.h>
#include<SPI.h> //protocole de communication hardware
#include<Ethernet.h> //bibliotheque de shield ethernet
byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED}; //adresse mac du shield ethernet
char server[]="192.168.1.11";
EthernetClient clt; //agent virtuel qui se communique avec le php
String url=""; //variable contenant le chemin de fichhier php
char c;
String str;
void setup()
{
    Serial.begin(9600);  
    Serial.println("setup");
    vw_setup(2000); 
    vw_rx_start();  
    if(!Ethernet.begin(mac))
    {
      Serial.println("Connexion échouée");
      for(;;);  //boucle inffini  
    }
    else
    {
      Serial.println("Connexion réussie");
    }
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
}
void loop()
{
  float t,c;
    int buf[2];
    uint8_t buflen = sizeof(buf);
    if (vw_get_message((uint8_t *)buf, &buflen))
    {
  t=buf[0];
  c=buf[1];  
 
 url="GET /emission/pfe.php?temp="+(String(buf[0]))+"&cou="+(String(buf[1]))+" HTTP/1.0";
 if(clt.connect(server,80))
 {
  Serial.println(url);
  clt.println(url);
  clt.println("Host: 192.168.1.11");
  clt.println();
 }
 else
 {
  Serial.println("erreur");
  return;
 }
    }


  for(;;) { 
    if(clt.available()) {  
      c = clt.read();    
      if(c == '\n') {        
   if(str.startsWith("<Led1>")) { 
          str.replace("<Led1>", "");  
          str.replace("</Led1>", "");  
          if(str == "1") 
          {   
            if((t>30)  || (c>20))
  {
    digitalWrite(8,LOW);
  }
  else
  {
     digitalWrite(8,HIGH); 
  }     
          }
          else 
          {    digitalWrite(8, LOW);   
                   
          }
        }
         else if(str.startsWith("<Led2>")) {
         
          str.replace("<Led2>", "");   
          str.replace("</Led2>", "");  
          Serial.println(str);
          if(str == "1") 
          {   
            if((t>30)  || (c>20))
  {
    digitalWrite(9,LOW);
  }
  else
  {
     digitalWrite(9,HIGH); 
  }     
          }
          else 
          {    digitalWrite(9, LOW);   
                   
          }
      }
   str = "";
    }
      else
        str += c; 
    }
  
    if(!clt.connected()) 
      break;              
  }

    clt.flush();
    clt.stop();
}
