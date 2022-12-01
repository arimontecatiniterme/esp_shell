/* librerie native */
#include <Arduino.h>
#include <iostream>
#include <WiFi.h>      //libreria per la gestione del wifi
#include <WebServer.h> // libreria per la gestione del webserver
#include <SPIFFS.h>
#include <FS.h>

/* librerie personali ./lib/..... */
#include "input.h"
#include "Regexp.h"
#include "shell.h"

#define BUTTON 16 // pin per l'accesso al menu

/* Definizione delle variabili globali */

/* Definisce la struttura di una nuova varibile, variabile di tipo board */
struct
{
  string __name__;    // nome dispositivo
  string __cgffile__; // file di configurazione
  int __timeout__;    // valore di timeout
  boolean bshMode;    // indica che la borda e' in modalita' shell
} typedef machine;

/* oggetto di tipo string che rappresenta l'input da tastiera */
string scmd;

/* Nuova variabile di tipo board */
machine myBoard;

/* oggetto di tipo shell */
shell sh;

/* funzione senza scopo da utilizzare per impegnare il tempo */
void makesAnything()
{
  std::cout << "...questa funzione stampa qualcosa...\n";
}

/* funzione di esempio di input */
void inputExample()
{

  /* oggetto di tipo input */
  input in;

  /* definizione delle variabili locali */
  String STR;
  string str;

  in.setEcho(false);

  std::cout << "...input mode..." << std::endl;

  std::cin >> in;
  in.trim();

  str = in;
  std::cout << "string=" << str << "\n";

  STR = in;
  std::cout << "String=" << STR << "\n";

  std::cout << std::endl
            << "in=" << in << std::endl;
  std::cout << "str==in -> " << (str == in) << "\n";
  std::cout << "STR==in -> " << (STR == in) << "\n";
  std::cout << "in==str -> " << (in == str) << "\n";
  std::cout << "in==STR -> " << (in == STR) << "\n";

  str += in;
  std::cout << "str+=input " << str << "\n";
}

void setup()
{

  Serial.flush();
  Serial.begin(9600);
  Serial.println("Serial is ready to accept input");

  pinMode(BUTTON, INPUT);

  Serial.print(F("Inizializing FS..."));
  if (SPIFFS.begin(true))
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }

  /* impostazione della configurazione generale della scheda */
  myBoard.__timeout__ = 30000;        // 30 sec di timeout
  myBoard.__name__ = "WEMOS D1 R32";  // nome macchina
  myBoard.__cgffile__ = "config.ini"; // file di configurazione del sw
}

void loop()
{

  // alla pressione del tasto BUTTON

  // if (digitalRead(BUTTON)==HIGH) {

  std::cout << "shell..."
            << "\n";
  sh.setMode(true);

  scmd = sh.row();

  std::cout << "...ha inserito il comando " << scmd << std::endl;




  //}// end if

  makesAnything();
  delay(2000);
}
