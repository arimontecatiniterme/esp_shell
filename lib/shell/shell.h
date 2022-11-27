#include <Arduino.h>
#include <iostream>
#include <string>

#include "input.h"

#ifndef __SHELL__
#define __SHELL__

using namespace std;

class shell {

private:
  struct {
    string  __cmd__;                                                         // ultimo comando digitato
    boolean __mode__;                                                        // indica se la modalita' shell e' attiva
    string  __cur_path__;                                                    // path corrente dove verra' applicato il command
    int     __SesTimeout__;                                                  // imposta il tempo di timeout
    string  __prmt__;                                                        // stringa di prompt ( NOME MACHHINA + PATH)
    string  __mach__;                                                        // 
  } cfgshell;                                                                // configurazione dell'ambiente di shell
  
  String      s2S(string);                                                   // converte da String a string
  string      S2s(String);                                                   // converte da string a String

  string      trim(const std::string & );                                    // elimina gli spazi iniziali e finali
  string      rsearch(string, string);                                       // esegue la ricerca con regexp

  void        s2IP(string, int[]);                                           // converte una stringa in un indirizzo IP

  
public:
  shell();                                                                   // costruttore di default

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------  
  void        ifconfig();                                                    // stampa lo stato della configurazione di rete
  void        setMode(boolean __set__){cfgshell.__mode__=__set__;}           // imposta la modalita' di shell
  void        ls(string);                                                    // elenca i file della directory passata come parametro
  void        init(int);                                                     // avvia/ferma un servizio
  void        edlin(string);                                                 // editor di linea  
  void        rm(string);                                                    // cancella un file
  void        cp(string, string);                                            // copia un file
 
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------  
  boolean     mkdir();                                                       // crea una directory
  boolean     format();                                                      // formatta il file system
  boolean     mv(string);                                                    // sposta un file
 
  int         ifup(string);                                                  // attiva la scheda wifi in una delle seguenti modalita' (c - client / a - acces point )
 
  string      grep(string);                                                  // legge il file delle configurazioni e ritora il valore della variabile cercata 
  string      row();                                                       // attiva l'input del comando
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------  

  
}; // end class shell




#endif // __SHELL__

