//importer bibliotheque SPI pour le protocole de communication matérielle
#include <SPI.h>
//importer la bibliotheque Ethernet de shield pour la connexion internet
#include <Ethernet.h>
 //entrer l'ID de la variable gaz dans ubidots
String gazVarId = "5888983076254260eb9e0b42";
//entrer l'ID de la variable niveau de corbeille dans ubidots
String pressionVarId = "588c6a4c7625423983744168";
//entrer le token pour se connecter à votre compte
String token = "RSxtSXua0EDzEW3QUTmUTk2zku5rBO";
 //Adresse MAC du shield ethernet (sous format des octets (6 Octets))
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 //tableau de caractere nommé server contenant l'adresse de serveur ubidots
char server[]="http://things.ubidots.com";
 //client (agent virtuel) qui va transmettre les données vers ubidots
EthernetClient client;

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
}
 
void loop() {
//lire la valeur de capteur de gaz branché au pin A1
 float g = analogRead(1);
 //lire la valeur de capteur de pression branché au pin A2
  float n = analogRead(2);

 //appeler méthode save_value qui va enregistrer le contenu des deux variables g et n dans le serveur ubidots
  save_value(g, n);
//le programme va lire les valeurs des capteurs et les stocker dans ubidots chaque 5 secondes
  delay(200);
}
 
void save_value(float gazValue, float pressValue)
{

//la valeur de taille de chaine de caractere sous format json à envoyer vers ubidots
  int num = 0;
  int num1=0;
  //varString : la chaine sous forme json à envoyer vers ubidots
  String varString = "{\"value\":" + String(gazValue) + "}";
  String varString1 = "{\"value\":" + String(pressValue) + "}";

//retourner la taille de la chaine json envoyée vers ubidots 
 num = varString.length();
  num1 = varString1.length();
 //il faut tester si votre client est connecté ou non
 //80: est le port de protocole http permettant la communication entre arduino et internet
  if (client.connect(server,80)) {
     Serial.println("connected ubidots");
    delay(100);

    // Construct the POST request that we'd like to issue
    client.println("POST /api/v1.6/variables/"+gazVarId+"/values HTTP/1.1");
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
    client.flush();
  client.stop(); 
    
    if (client.connect(server,80)) {
     Serial.println("connected ubidots");
    delay(100);
    //envoi pression
    client.println("POST /api/v1.6/variables/"+pressionVarId+"/values HTTP/1.1");
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

 //sta: retourne l'état du client
  boolean sta = client.connected();
  if (sta==false)   {
   Serial.println("Serveur déconnecté.");
   client.stop();
  }

  client.flush();
  client.stop();
}
