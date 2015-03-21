/*
 Commande à distance par SMS
 Cet exemple utilise le shield GSM/GPRS officiel Arduino.
 Réceptionne le SMS, analyse le contenu, puis régi aux 
 différentes commande. Ici ce sont 3 Leds. 
 R pour led rouge, V pour led verte, J pour led jaune.
 A pour allumer, n'importe quelle autre led pour éteindre.
 Exemple d'un SMS RAVEJA.
 Si il receptionne la lettre T, envoi de la lecture de la
 tension A5 par émission d'un SMS.  
*/

// librairie.
#include <GSM.h>

GSM gsmAccess;
GSM_SMS sms;

#define tension_1  A5

// Entrer votre numéro de téléphone !:
char remoteNumber[20] ="XXXXXXXXXX";  

// Numéro des Pins pour connection Leds :
int pinR = 11;  
int pinV = 9;
int pinJ = 5;

int ifor = 0;   // Pins pour connection Leds.
int ifor2 = 0;  // Pins pour connection Leds.

char data[9];  // pour réception SMS.
int sensorvalue; // pour lecture analogique.
float voltage;   // pour lecture analogique.

void setup() 
{
  // initialisation du port série.
  Serial.begin(9600); 

  Serial.println("Commande a distance par SMS");

  // initialisation des pins en sortie :
  pinMode(pinR, OUTPUT);
  pinMode(pinV, OUTPUT);
  pinMode(pinJ, OUTPUT);

  // état de la connection :
  boolean notConnected = true;

// Code PIN de la carte SIM. (Si utilisé !) :
#define PINNUMBER ""


  // Démarrage shield GSM :
  
  while(notConnected)
  {
    //   Si code PIN remplacer la ligne suivante par :
    //   if(gsmAccess.begin(PINNUMBER)==GSM_READY)
    if(gsmAccess.begin()==GSM_READY) 
      notConnected = false;
    else
    {
      Serial.println("Non connecté");
      delay(1000);
    }
  }

  Serial.println("GSM pret a recevoir");
  Serial.println("Attente des messages");
}

void loop() 
{

  char c;

  // Y a t-il des SMS à lire ?.
  if (sms.available())
  { 
    int smsdata = (sms.available());
    
    Serial.println("Message recu de :");

    // Recherche du numero d'appel.
    // Puis on l'affiche dans le moniteur.
    sms.remoteNumber(remoteNumber, 20);
    // exemple : +33123456789
    //Serial.println(remoteNumber);

    // Voici un exemple de filtrage  
    // Les messages commencant par un # seront écartés.
    if(sms.peek()=='#')
    {
      Serial.println("Discarded SMS");

      // SMS effacé de la mémoire :
      sms.flush();
    }

    // Lecture et affichage du contenu du message.
    int count = 0;

    while(c=sms.read())
    { 
      data[count] = c; 
      count++;
    }
    ifor = 0; 
    for (ifor = 0; ifor < count ; ifor = ifor+2 )
    {
      ifor2=ifor+1;
      switch(data[ifor])
      {        
      case 'R':        
        if (data[ifor2] == 'A')       
        {
          digitalWrite(pinR, HIGH);
          Serial.println("Led Rouge allume");
        }
        else
        {
          digitalWrite(pinR, LOW);
          Serial.println(" Led Rouge eteinte");
        } 
        break;
        
      case 'V':
        if (data[ifor2] == 'A')
        {
          digitalWrite(pinV, HIGH);
          Serial.println("Led Verte allume");
        }
        else
        {
          digitalWrite(pinV, LOW);
          Serial.println("Led Verte eteinte");
        } 
        break;
        
       case 'J': 
        if (data[ifor2] == 'A')
        {
          digitalWrite(pinJ, HIGH);
          Serial.println("Led Jaune allume");
        }
        else
        {
          digitalWrite(pinJ, LOW);
          Serial.println("Led Jaune eteinte");
        } 
        break; 

        case 'T':
        Serial.println(data[ifor]);
        sensorvalue = analogRead(tension_1);
        voltage = sensorvalue * (5.0 / 1023.0);
        sms.beginSMS(remoteNumber);
        sms.print(voltage);
        sms.print(" Volts");
        sms.endSMS();
        Serial.println("\nMessage expedie!\n");
        break;
      }
    } 

    Serial.println("\nFIN OF MESSAGE");

    // effacé message de la mémoire du modem.
    sms.flush();
    Serial.println("MESSAGE EFFACE");
  }

  delay(1000);

}  // Fin boucle void loop().




