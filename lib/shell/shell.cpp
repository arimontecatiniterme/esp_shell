#include <Arduino.h>
#include <WiFi.h>
#include <iostream>
#include <string>
#include <SPIFFS.h>
#include <FS.h>
#include <fstream>
#include <algorithm>
#include <regex>
#include <vector>
#include <tuple>
#include <Regexp.h>

#include "input.h"
#include "shell.h"

#define __MACHINE__ ARDUINO

using namespace std;

/* costruttore di default */
shell::shell()
{
  cfgshell.__mode__ = false;
  cfgshell.__cur_path__ = "/";
  cfgshell.__mach__ = "IQ5MT";
  cfgshell.__SesTimeout__ = 30000; // tempo di timeout a 30 secondi
  cfgshell.__prmt__ = cfgshell.__cur_path__;
}

/* converte da std::string a String */
String shell::s2S(string str)
{

  int len = 0;
  int pos;
  char rc;
  String STR;

  len = str.length();
  char buffer[len + 1];

  strcpy(buffer, str.c_str());

  pos = 0;

  while (buffer[pos] != '\0')
  {
    rc = buffer[pos];
    STR = STR + rc;
    pos++;
  }

  STR = STR + '\0';

  return STR;

} // end

/* converte da String a std::string */
string shell::S2s(String STR)
{

  int len = 0;
  int pos;
  char rc;
  string str;

  len = STR.length();
  char buffer[len + 2];

  strcpy(buffer, STR.c_str());

  pos = 0;

  while (buffer[pos] != '\0')
  {
    rc = buffer[pos];
    str = str + rc;
    pos++;
  }

  return str;

} // end

//************************************************************/
//
//       ID :
// Descrive : elimina gli spazi iniziali e finali
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
string shell::trim(const std::string &source)
{

  std::string s(this.source);
  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

//************************************************************/
//
//       ID :
// Descrive : gestiste l'input del comando
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
string shell::row()
{
  input cmd;
  string sCMD;
  smatch rescmd, resfl;

  //       +----------------------( parte iniziale della stringa )
  //       |
  //       |    +-----------------( considera l'eventuale presenza di spazi iniziali )
  //       |    |
  //       |    |                         +----( elenco comandi possibili)
  //       |    |                         |
  //       | /--+--\ /--------------------^------------------------------------------------------\ 

  regex r("^([ ]{0,}((ls)|(cp)|(grep)|(format)|(edlin)|(rm)|(mv)|(exit)|(cls)|(ifup)|(help)|(cat)))");

  do
  {
    cmd.clear();
    std::cout << "\n...digita un comando valido oppure help per aiuto...\n";
    std::cout << cfgshell.__cur_path__;
    std::cin >> cmd;
    cmd.trim();
    sCMD = cmd;
  } while (!regex_search(sCMD, rescmd, r));

  return cmd;

} // END function input()

/************************************************************/
//
//       ID :
// Descrive : Copia un file
//     Date :
//   Author : Andrea Venuti
//
/************************************************************/
// START function(yoZUM )
void shell::cp(string __files__, string __filed__)
{

  String FILES; // nome del file da copiare
  String FILED; // nome del file di destinazione
  String ROW;   // riga letta del file
  string sLine;

  input row;

  boolean bTest;

  std::smatch result;

  // conversione dei parametri attuali
  FILES = s2S(cfgshell.__cur_path__ + __files__);
  FILED = s2S(cfgshell.__cur_path__ + __filed__);

  bTest = true;

  if (!SPIFFS.exists(FILES))
  { // controlla se il file di origine esiste
    bTest = false;
    std::cout << "\n\n\tIl file di ronigine non esiste\n\n";
  }

  if (SPIFFS.exists(FILED) && bTest)
  { // controlla se il file di destinazione esiste
    bTest = false;
    regex r("(sRow;|S|n|N){1}");

    do
    {
      std::cout << "\n...il file di destinazione esiste...lo cancello ? [sN]....";
      std::cin >> row;
      sLine = row;
    } while (!regex_search(sLine, result, r));

    if (sLine == "sRow;" || sLine == "S")
    { // si e' scelto di cancellare il file di destinazione
      shell::rm(__filed__);
      bTest = true;
    }
  }

  // START while( Osre2 ) - ciclo di copia
  if (bTest)
  {
    File FS = SPIFFS.open(FILES);
    File FD = SPIFFS.open(FILED, FILE_WRITE);

    while (FS.available())
    {
      FD.println(FS.readStringUntil('\n'));
    } // END while( uRVy7 )

    FD.close();

    std::cout << "\n...file copiato...\n";
  } // END while( Osre2 )

  // FS.close();

} // END function( yoZUM )

//************************************************************/
//
//       ID :
// Descrive : rimuove un file
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
// START function(AN4KM )
void shell::rm(string __file__)
{
  String FILE;
  string sLine;
  input row;
  smatch result;

  FILE = s2S(cfgshell.__cur_path__ + __file__);

  if (!SPIFFS.begin(true))
  {
    std::cout << "\n\n\tAn Error has occurred while mounting SPIFFS\n\n";
  }
  else
  {
    if (!SPIFFS.exists(FILE))
    {
      std::cout << "\n\n\tAn File not exist\n\n";
    }
    else
    {

      regex r("(sRow;|S|n|N){1}");

      do
      {
        row.clear();
        std::cout << "\n\n\tCancello il file ? [sN]....";
        std::cin >> row;
        sLine = row;
      } while (!regex_search(sLine, result, r));

      if (sLine == "sRow;" || sLine == "S")
      { // si e' scelto di cancellare il file di destinazione
        if (SPIFFS.remove(FILE))
        {
          std::cout << "\n\n\tFile cancellato....";
        }
        else
        {
          std::cout << "\n\n\tImpossibile rimuovere il file....";
        }
      }
    }
  }

} // END function( AN4KM )

//************************************************************/
//
//       ID :
// Descrive : formatta il FS
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
// START function(AN4KM )
boolean shell::format()
{

  boolean bResult;
  string sLine;
  input row;
  smatch result;

  bResult = false;

  regex r("(sRow;|S|n|N){1}");

  do
  {
    std::cout << "\n\n\tFormatto il FS ? [sN]....";
    std::cin >> row;
    sLine = row;
  } while (!regex_search(sLine, result, r));

  if (sLine == "sRow;" || sLine == "S")
  { // si e' scelto di cancellare il file di destinazione
    if (SPIFFS.format())
    {
      std::cout << "Formattazione eseguita correttamente..."
                << "\n";
      bResult = true;
    }
    else
    {
      bResult = false;
    }

    return bResult;
  }

  return false;
} // end format()

//************************************************************/
//
//       ID :
// Descrive : rinomina un file
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
// START function(6IyDN )
boolean shell::mv(string __cmd__)
{

  //-------------------|--------------------------------
  String FILES, FILED;
  String ROW;
  //--------------------|--------------------------------
  string __rows__;
  string __temp__;
  string sRow, sS, sD;
  string sTemp, sPrec, sSuc, sReg;
  string asSWITCH[8][4];
  //--------------------|--------------------------------
  char *cSwitch;
  //--------------------|--------------------------------
  int iIndex, iMax, iPrec;
  int iSize = 4;
  //--------------------|--------------------------------
  smatch result, result0, result1, result2;
  //--------------------|--------------------------------
  File FD, FS;
  //--------------------|--------------------------------
  vector<string> aOutput;

  regex r("(-(s|d))");
  sPrec = "";
  sSuc = "";
  sRow = "";
  iIndex = 0;

  try
  {
    for (sregex_iterator it = sregex_iterator(__cmd__.begin(), __cmd__.end(), r); it != sregex_iterator(); it++)
    {
      result = *it;

      if (sRow.find(result.str(0)) != std::string::npos)
      {
        std::cout << "\n\n\t"
                  << "ATTENZIONE switch ripetuti....";
        return "";
      }
      else
      {

        if (sPrec != "")
        {

          sReg = "(" + sPrec + ")(.+)(" + result.str(0) + ")";
          sReg = trim(sReg);

          asSWITCH[iIndex - 1][0] = trim(sPrec);
          asSWITCH[iIndex - 1][1] = trim(sReg);
          asSWITCH[iIndex - 1][2] = trim(__cmd__.substr(iPrec + 2, result.position(0) - iPrec - 3));
        }
      }

      sPrec = result.str(0);
      iPrec = result.position(0);
      sRow += result.str(0);
      iIndex++;
    }

    sReg = "(" + result.str(0) + ")(.+)";
    iIndex--;
    asSWITCH[iIndex][0] = trim(sPrec);
    asSWITCH[iIndex][1] = trim(sReg);
    asSWITCH[iIndex][2] = trim(__cmd__.substr(iPrec + 2, __cmd__.length() - iPrec));
  }

  catch (const std::regex_error &r)
  {
    std::cout << "\n\n\tregex_error caught: " << r.what() << '\n';
    if (r.code() == std::regex_constants::error_brack)
    {
      std::cout << "\tThe code was error_brack\n";
    }
  }

  iMax = iIndex;

  // separazione delle regole di estrazione
  sTemp = "";
  sS = "";
  sD = "";

  for (iIndex = 0; iIndex <= iMax; iIndex++)
  {

    sTemp = trim(asSWITCH[iIndex][0]);
    cSwitch = &sTemp[1];

    // std::cout << "\n\t\t" << asSWITCH[iIndex][0] << "(" << cSwitch << ")\t" << asSWITCH[iIndex][1] << "\t" << asSWITCH[iIndex][2];

    switch (*cSwitch)
    {
    case 's':
    { // nome del file
      sS = asSWITCH[iIndex][2];
      FILES = s2S(cfgshell.__cur_path__ + trim(sS));
      if (!SPIFFS.exists(FILES))
      {
        std::cout << "\n\n\t\tIl file di origine non esiste.....\n\n";
        return false;
      }

      break;
    }
    case 'd':
    { // nome della variabile
      sD = asSWITCH[iIndex][2];
      FILED = s2S(cfgshell.__cur_path__ + trim(sD));
      if (SPIFFS.exists(FILED))
      {
        shell::rm(cfgshell.__cur_path__ + trim(sD));
      }
      break;
    }
    }
  }

  if (!SPIFFS.begin(true))
  {
    std::cout << "\n\n\tAn Error has occurred while mounting SPIFFS\n\n";
    return false;
  }
  else
  {
    if (!SPIFFS.rename(FILES, FILED))
    {
      return false;
    }
    else
    {
      return true;
    }
  }

} // END function( 6IyDN )

//************************************************************/
//
//       ID :
// Descrive : elenca i file di una directory
//     Date :
//   Author : Andrea Venuti
//
//************************************************************/
void shell::ls(string sCMD)
{

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  std::cout << "\n";
  while (file)
  {
    std::cout << "  FILE: " << S2s(file.name()) << "\tSIZE: " << file.size() << "\n";
    file = root.openNextFile();
  }
} // END function ls()
