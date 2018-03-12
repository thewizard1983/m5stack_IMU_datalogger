#include <M5Stack.h>

boolean sdOK = false;
const char fileName[3] = "acq";
const char fileExt[4] = ".bin";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  M5.begin();
  if (!SD.begin()) {
    M5.Lcd.printf("Errore SD - scheda danneggiata o da fomrattare.\n");
    sdOK = false;
  }
  sdOK = true;
  listDir();
  delay(2000);
  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(0,0);
}

void loop() {
  if (M5.BtnA.wasPressed())
  {
    if (!sdOK)
      M5.Lcd.printf("\nControllare la scheda SD.\n");
    else
    {
      //Check presenza cartella salvataggi e creo nuovo file
      if (!SD.exists("/IMU_ACQ")) 
      {
        M5.Lcd.printf("\nDirectory non esistente.\n");
        M5.Lcd.printf("\nCreo la cartella.\n");
        if (SD.mkdir("/IMU_ACQ"))
          M5.Lcd.printf("\nCartella Creata\n");
        else{
          M5.Lcd.printf("\nErrore creazione cartella.\n");
          sdOK = false;
        } 
      }
      Serial.println(checkFile());
       
    }
  }
  M5.update();
}

String checkFile(){
  char nomeFile[10];
  int n = 0;
  do{
  if (n<10)
    sprintf(nomeFile,"%s00%d%s",fileName,n,fileExt);
  else if(n<100)  
    sprintf(nomeFile,"%s0%d%s",fileName,n,fileExt);
  else
    sprintf(nomeFile,"%s%d%s",fileName.toCharArray(),n,fileExt);
    n++;
  }while (!SD.exist(nomeFile));
  return nomeFile;
}

void listDir() {
  uint8_t cardType = SD.cardType();
  M5.Lcd.printf("SD Card Type: ");
  if (cardType == CARD_MMC) {
    M5.Lcd.printf("MMC\n");
  } else if (cardType == CARD_SD) {
    M5.Lcd.printf("SDSC\n");
  } else if (cardType == CARD_SDHC) {
    M5.Lcd.printf("SDHC\n");
  } else {
    M5.Lcd.printf("UNKNOWN\n");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  M5.Lcd.printf("SD Card Size: %lluMB\n", cardSize);

  SDFile root = SD.open("/");
  if (!root) {
    M5.Lcd.printf("Failed to open directory\n");
    return;
  }
  if (!root.isDirectory()) {
    M5.Lcd.printf("Not a directory");
    return;
  }

  SDFile file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      M5.Lcd.printf("\nDIR : ");
      M5.Lcd.println(file.name());
    } else {
      M5.Lcd.printf("FILE: ");
      M5.Lcd.print(file.name());
      M5.Lcd.printf("\t\tSIZE: ");
      M5.Lcd.println(file.size());
    }
    file = root.openNextFile();
  }
}
