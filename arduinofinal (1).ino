//importer bibliotheque SPI pour le protocole de communication matérielle
#include <SPI.h>
#include <Servo.h>
//importer la bibliotheque Ethernet de shield pour la connexion internet
#include <Ethernet.h>
#include <UbidotsEthernet.h>
#define TOKEN  "3LokVTBMgsjD7d8LAB558wdIIOfABI"
 //entrer l'ID de la variable gaz dans ubidots
String vargazid ="58ab31897625424a687273f0" ;
String varpuissance1 ="58ab31a47625424a602787c7" ;
String varpuissance2 ="58ab31b17625424a60278838" ;
//entrer le token pour se connecter à votre compte
String token = "3LokVTBMgsjD7d8LAB558wdIIOfABI";
 //Adresse MAC du shield ethernet (sous format des octets (6 Octets))
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 //tableau de caractere nommé server contenant l'adresse de serveur ubidots
char server[]="http://things.ubidots.com";
 //client (agent virtuel) qui va transmettre les données vers ubidots
EthernetClient client;
Ubidots clt(TOKEN);
float gaz,puissance1,puissance2;
int pir=0;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
// méthode de configuration et initialisation 
void setup() {
//Vitesse de transmission de données (9600 bit par second)
  Serial.begin(9600);
  //tester si shield ethernet a commencer de se connecter au routeur, si n'est pas le cas
  if (!Ethernet.begin(mac)) {
  //afficher un message sur votre pc à travers la communication série(cable usb entre arduino & pc)
    Serial.println("Erreur");
  

//boucle infini ou bien tu peut écrire return;
	for(;;);
  }
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    pinMode(8,OUTPUT);
    pinMode(2,INPUT);
   
}
 
void loop() {
//lire la valeur de capteur de gaz branché au pin A1

/*methode de lire temp et hum
 * 
 * 
 * 
 */
  gaz=analogRead(1);
  if(gaz>500)
  {
    digitalWrite(8,HIGH);
    }
    else
      {
    digitalWrite(8,LOW);
    }


  pir=digitalRead(2);
  if(pir==HIGH)
  {
    digitalWrite(8,HIGH);
    }
    else
      {
    digitalWrite(8,LOW);
    }
 
if(Serial.available())
  {
    char c=Serial.read();
    if(c=='1')
    {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    }
    else  if(c=='0')
    {
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    }
  }

    
  puissance1=((analogRead(2) / 4.092)/10);
  puissance2=((analogRead(3) / 4.092)/10);
  Serial.println(puissance1);
   Serial.println(puissance2);
    Serial.println(gaz);
 //appeler méthode save_value qui va enregistrer le contenu des deux variables g et n dans le serveur ubidots
  save_value(gaz, puissance1,puissance2);
 
//le programme va lire les valeurs des capteurs et les stocker dans ubidots chaque 2 secondes
  delay(2000);
}
 
void save_value(float gazValue, float tempValue, float humValue)
{

//la valeur de taille de chaine de caractere sous format json à envoyer vers ubidots
  int num = 0;
  int num1=0;
   int num2=0;
  //varString : la chaine sous forme json à envoyer vers ubidots
  String varString = "{\"value\":" + String(gazValue) + "}";
  String varString1 = "{\"value\":" + String(tempValue) + "}";
String varString2 = "{\"value\":" + String(humValue) + "}";
//retourner la taille de la chaine json envoyée vers ubidots 
 num = varString.length();
  num1 = varString1.length();
  num2 = varString2.length();
 //il faut tester si votre client est connecté ou non
 //80: est le port de protocole http permettant la communication entre arduino et internet
  if (client.connect(server,80)) {
     Serial.println("connected ubidots");
    delay(100);

    // Construct the POST request that we'd like to issue
    client.println("POST /api/v1.6/variables/"+vargazid+"/values HTTP/1.1");
    // We also use the Serial terminal to show how the POST request looks like
    // Specify the contect type so it matches the format of the data (JSON)
    client.println("Content-Type: application/json");
    // Specify the content length
    client.println("Content-Length: "+String(num));
    // Use our own API token so that we can actually publish the data
    client.println("X-Auth-Token: "+token);
    // Specify the host
    client.println("Host: things.ubidots.com\n");
    // Send the actual data
    client.print(varString);
  }
     else
  {
    Serial.println("Erreur connection");
  } 
  //vider client
    client.flush();
  client.stop(); 
    
    if (client.connect(server,80)) {
     Serial.println("connected ubidots");
    delay(100);
    //envoi courant
    client.println("POST /api/v1.6/variables/"+varpuissance1+"/values HTTP/1.1");
    // We also use the Serial terminal to show how the POST request looks like
    // Specify the contect type so it matches the format of the data (JSON)
    client.println("Content-Type: application/json");
    // Specify the content length
    client.println("Content-Length: "+String(num1));
    // Use our own API token so that we can actually publish the data
    client.println("X-Auth-Token: "+token);
    // Specify the host
    client.println("Host: things.ubidots.com\n");
    // Send the actual data
    client.print(varString1);
    }
     else
  {
    Serial.println("Erreur connection");
  }  

 //vider client
    client.flush();
  client.stop(); 

 if (client.connect(server,80)) {
     Serial.println("connected ubidots");
    delay(100);

    // Construct the POST request that we'd like to issue
    client.println("POST /api/v1.6/variables/"+varpuissance2+"/values HTTP/1.1");
    // We also use the Serial terminal to show how the POST request looks like
    // Specify the contect type so it matches the format of the data (JSON)
    client.println("Content-Type: application/json");
    // Specify the content length
    client.println("Content-Length: "+String(num2));
    // Use our own API token so that we can actually publish the data
    client.println("X-Auth-Token: "+token);
    // Specify the host
    client.println("Host: things.ubidots.com\n");
    // Send the actual data
    client.print(varString2);
  }
     else
  {
    Serial.println("Erreur connection");
  } 
 
 //sta: retourne l'état du client
  boolean sta = client.connected();
  if (sta==false)   {
   Serial.println("Serveur déconnecté.");
   client.stop();
  }

  client.flush();
  client.stop();
}
