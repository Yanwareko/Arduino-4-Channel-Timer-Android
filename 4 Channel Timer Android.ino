/*Relay 4 Channel

  List Inisial code

   1  =====>    Relay 1 On 
   2  =====>    Relay 1 On
   3  =====>    Relay 1 On
   4  =====>    Relay 1 On
   5  =====>    All Relay On

-------------------------------

   A  =====>    Relay 1 Off
   B  =====>    Relay 1 Off
   C  =====>    Relay 1 Off
   D  =====>    Relay 1 Off
   E  =====>    All Relay Off

-------------------------------

   M  =====>    Set Relay 1 On 
   N  =====>    Set Relay 2 On 
   O  =====>    Set Relay 3 On 
   P  =====>    Set Relay 4 On 
 
-------------------------------

   Q  =====>    Set Relay 1 Off
   R  =====>    Set Relay 2 Off
   S  =====>    Set Relay 3 Off
   T  =====>    Set Relay 4 Off 

-------------------------------
  Relay pin
  Relay 1 pin 8
  Relay 2 pin 9
  Relay 3 pin 10
  Relay 4 pin 11
-------------------------------
  DS1307
  SDA   pin   A4
  SCL   pin   A5
-------------------------------
  Send Android Status
  a = Relay 1 On 
  b = Relay 1 Off
  c = Relay 2 On 
  d = Relay 2 Off 
  e = Relay 3 On 
  f = Relay 3 Off
  g = Relay 4 On
  h = Relay 4 Off 
*/

//================================================== LIBRARY ================================================== 

  #include <SPI.h>               // SPI Library
  #include <TimerOne.h>          // Timer1 Library
  #include <SoftwareSerial.h>    // Software Serial / Dual Serial
  #include <RTClib.h>            // Real Time Clock Library
  #include <EEPROM.h>            // EEPROM

//================================================== Inisialisasi Relay ================================================== 

  #define Relay1      8
  #define Relay2      9
  #define Relay3      10
  #define Relay4      11

//================================================== Variabel ================================================== 

  char command[20];            
  char KodePerintah[3];   
  char isi1[6];
  char isi2[6];
  char *index1;
  char *index2;
  
  //RTC Hari dalam bahasa Indonesia
  char  daysOfTheWeek[7][11] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Juma't", "Sabtu"};

  // Variabel
  int i,a;
  int b =0;

  int d = 0;
  int e = 0;
  int f = 0;
  int g = 0;
  
  //Variabel Incoming Serial
  int membacainput,KodeJam,KodeMenit;

  //Variabel timer 
  int jam   = 10;
  int menit = 35;
  int Jamset,Mntset;            // Variabel Relay

  // Variabble Set Timer

  int Jam1,Jam2,Jam3,Jam4,JamA,JamB,JamC,JamD;
  int Menit1,Menit2,Menit3,Menit4,MenitA,MenitB,MenitC,MenitD;
/*  
  // -------------------- Jam ON --------------------
  int JamA  = 8;
  int JamB  = 8; 
  int JamC  = 9;
  int JamD  = 9;;

  // -------------------- Menit ON --------------------
  int MenitA  = 0;
  int MenitB  = 0;
  int MenitC  = 30;
  int MenitD  = 30;
  
  // -------------------- Jam Off --------------------  
  int Jam1  = 16;
  int Jam2  = 16;
  int Jam3  = 17;
  int Jam4  = 17;
  
  // -------------------- Menit Off --------------------
  int Menit1  = 30;
  int Menit2  = 30;
  int Menit3  = 0;
  int Menit4  = 0;

*/

  //Internal Timer Scaning
  unsigned long milisterkini=0;
  unsigned long milisset=0;

  boolean setawal = true;
//================================================== Inisialisasi Pin Modul ================================================== 
    
  RTC_DS3231 RTC;//SDA -> A4, SCL:  ->  A5
  
  SoftwareSerial BT (0,1);

//================================================== Setup ================================================== 

void setup() {
  RTC.begin();                                                        // Initialize RTC Modul
  Serial.begin(9600);
  BT.begin(9600);
  EEPROM.begin();
  
  pinMode (Relay1  ,OUTPUT);
  pinMode (Relay2  ,OUTPUT);
  pinMode (Relay3  ,OUTPUT);
  pinMode (Relay4  ,OUTPUT);
  //All Relay Standby Off
      digitalWrite  (Relay1,HIGH);
      digitalWrite  (Relay2,HIGH);
      digitalWrite  (Relay3,HIGH);
      digitalWrite  (Relay4,HIGH);

  
}

//================================================== Scan waktu dari EEPROM ================================================== 

void membacawaktu(){
  
  // -------------------------------------------------- membaca rtc --------------------------------------------------  
  DateTime now = RTC.now();                                       // Baca Data RTC Sekarang
  Jamset  = now.hour();
  Mntset  = now.minute();
  Serial.println("Waktu Terkini : ");
  Serial.print("Hari ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(", ");
  Serial.print(now.day());
  Serial.print("-");
  Serial.print(now.month());
  Serial.print("-");
  Serial.println(now.year());
  Serial.print(" Jam : ");
  Serial.print(Convert(Jamset));
  Serial.print(":");
  Serial.println(Convert(Mntset));

  // Read Data Timer Relay On
      JamA    = EEPROM.read (0);
      MenitA  = EEPROM.read (1);
      JamB    = EEPROM.read (2);
      MenitB  = EEPROM.read (3);  
      JamC    = EEPROM.read (4);
      MenitC  = EEPROM.read (5);
      JamD    = EEPROM.read (6);
      MenitD  = EEPROM.read (7);
  // Read Data Timer Relay Off
      Jam1    = EEPROM.read (10);
      Menit1  = EEPROM.read (11);
      Jam2    = EEPROM.read (12);
      Menit2  = EEPROM.read (13);  
      Jam3    = EEPROM.read (14);
      Menit3  = EEPROM.read (15);
      Jam4    = EEPROM.read (16);
      Menit4  = EEPROM.read (17);  
  // -------------------------------------------------- Menampilkan Data Tersimpan --------------------------------------------------
    Serial.println("Timer Relay 1 : ");
      Serial.print("ON : ");
      Serial.print(Convert(JamA));
      Serial.print(":");
      Serial.print(Convert(MenitA));
      Serial.print(" | OFF : ");
      Serial.print(Convert(Jam1));
      Serial.print(":");
      Serial.println(Convert(Menit1));
  
    Serial.println("Timer Relay 2 : ");
      Serial.print("ON : ");
      Serial.print(Convert(JamB));
      Serial.print(":");
      Serial.print(Convert(MenitB));
      Serial.print(" | OFF : ");
      Serial.print(Convert(Jam2));
      Serial.print(":");
      Serial.println(Convert(Menit2));
  
    Serial.println("Timer Relay 3 : ");
      Serial.print("ON : ");
      Serial.print(Convert(JamC));
      Serial.print(":");
      Serial.print(Convert(MenitC));
      Serial.print(" | OFF : ");
      Serial.print(Convert(Jam3));
      Serial.print(":");
      Serial.println(Convert(Menit3));
  
    Serial.println("Timer Relay 4 : ");
      Serial.print("ON : ");
      Serial.print(Convert(JamD));
      Serial.print(":");
      Serial.print(Convert(MenitD));
      Serial.print(" | OFF : ");
      Serial.print(Convert(Jam4));
      Serial.print(":");
      Serial.println(Convert(Menit4));
          
    Serial.println("==============================");
  
  delay(10);
  
  // -------------------------------------------------- Membaca Waktu Timer  --------------------------------------------------
  
  if (Jamset == JamA && Mntset == MenitA){
    digitalWrite  (Relay1, LOW);
    Serial.println("Relay 1 On Sesuai Timer");
    Serial.println("==============================");
    BT.print("a");
    d = 1;
  }
      if (Jamset == Jam1 && Mntset == Menit1){
        digitalWrite  (Relay1, HIGH);
        Serial.println("Relay 1 Off Sesuai Timer");
        Serial.println("==============================");
        BT.print("b");
        d = 0;
      }
      
  delay(10);
  if (Jamset == JamB && Mntset == MenitB){
    digitalWrite  (Relay2, LOW);
    Serial.println("Relay 2 On Sesuai Timer");
    Serial.println("==============================");
    BT.print("c");
    e = 1;
  }
      if (Jamset == Jam2 && Mntset == Menit2){
        digitalWrite  (Relay2, HIGH);
        Serial.println("Relay 2 Off Sesuai Timer");
        Serial.println("==============================");
        BT.print("d");
        e = 0;
      }
      
  delay(10);    
  if (Jamset == JamC && Mntset == MenitC){
    digitalWrite  (Relay3, LOW);
    Serial.println("Relay 3 On Sesuai Timer");
    Serial.println("==============================");
    BT.print("e");
    f = 1;
  }
      if (Jamset == Jam3 && Mntset == Menit3){
        digitalWrite  (Relay3, HIGH);
        Serial.println("Relay 3 Off Sesuai Timer");
        Serial.println("==============================");
        BT.print("f");
        f = 0;
      }

  delay(10);
  if (Jamset == JamD && Mntset == MenitD){
    digitalWrite  (Relay4, LOW);
    Serial.println("Relay 4 On Sesuai Timer");
    Serial.println("==============================");
    BT.print("g");
    g = 1;
  }
      if (Jamset == Jam4 && Mntset == Menit4){
        digitalWrite  (Relay4, HIGH);
        Serial.println("Relay 4 Off Sesuai Timer");
        Serial.println("==============================");
        BT.print("h");
        g = 0;
      }
      
  delay(100);
  // -------------------------------------------------- Menampilkan status relay --------------------------------------------------
  Serial.println("Status Relay : ");
  if ( d  == 0){
    Serial.println("Relay 1 Off");
  }
    else{
      Serial.println("Relay 1 ON");
    }

  Serial.println(" ");
  if ( e  == 0){
    Serial.println("Relay 2 Off");
  }
    else{
      Serial.println("Relay 2 ON");
    }

  Serial.println(" ");
  if ( f  == 0){
    Serial.println("Relay 3 Off");
  }
    else{
      Serial.println("Relay 3 ON");
    }
  
  Serial.println(" ");
  if ( g  == 0){
    Serial.println("Relay 4 Off");
  }
    else{
      Serial.println("Relay 4 ON");
    }
  Serial.println("==============================");  
}


//================================================== Program Utama ================================================== 

void loop() {

  if (setawal){
    membacawaktu();
    setawal = false;
  }
 //Scan RTC
 milisterkini = (millis()/1000);

  if ((milisterkini - milisset) >= 60){
    membacawaktu();
    milisset = milisterkini;
  }
  
 if (BT.available() > 0) {
  
  membacainput = BT.readBytesUntil('\n', command, sizeof(command) - 1);
  command[membacainput] = '\0';  
  Serial. print("Input masuk dari Android = ");    
  Serial.println(command);   
  
    //Split pertama , Membagi berdasar separator ;  
      index1 = strchr(command, ';');
      *index1 = '\0';                             
      strcpy(KodePerintah, command);
      strcpy(isi1, index1 + 1);
        //Split kedua, membagi menjadi 3, berdasar separator ;
          index2 = strchr(isi1, ';');
          *index2 = '\0';               
          strcpy(isi2, index2 + 1);
          KodeJam = atoi (isi1);
          KodeMenit = atoi (isi2); 

  //Serial Monitor Hasil Input Masuk
  Serial.print("Hasil Pencacahan Kode, Perintah : ");              
  Serial.print(KodePerintah);
  if (isDigit(isi1[0])){ //Jika Kode Perintah adalah Input Timer maka akan menampilkan waktu dan jam
      Serial.print(" | ");
      Serial.print(" Jam : ");
      Serial.print(KodeJam);
      Serial.print(" ");
      Serial.print(" Menit : ");
      Serial.println(KodeMenit);
  }
      else {
        Serial.println(" ");
      }
      
  // Pencacahan Bersadar KodePerintah
  
  switch (KodePerintah[0]){
    
    //Manual On
    case '1': digitalWrite  (Relay1,LOW);Serial.println("Relay 1 On");Serial.println("==============================");BT.print("a");d = 1;break;
    case '2': digitalWrite  (Relay2,LOW);Serial.println("Relay 2 On");Serial.println("==============================");BT.print("c");e = 1;break;
    case '3': digitalWrite  (Relay3,LOW);Serial.println("Relay 3 On");Serial.println("==============================");BT.print("e");f = 1;break;
    case '4': digitalWrite  (Relay4,LOW);Serial.println("Relay 4 On");Serial.println("==============================");BT.print("g");g = 1;break;
    case '5': digitalWrite  (Relay1,LOW);digitalWrite  (Relay2,LOW);digitalWrite  (Relay3,LOW);digitalWrite  (Relay4,LOW);Serial.println("All Relay ON");break;  //Saklar On Semua

    //Manual Off
    case 'A': digitalWrite  (Relay1,HIGH);Serial.println("Relay 1 Off");Serial.println("==============================");BT.print("b");d = 0;break;
    case 'B': digitalWrite  (Relay2,HIGH);Serial.println("Relay 2 Off");Serial.println("==============================");BT.print("d");e = 0;break;
    case 'C': digitalWrite  (Relay3,HIGH);Serial.println("Relay 3 Off");Serial.println("==============================");BT.print("f");f = 0;break;
    case 'D': digitalWrite  (Relay4,HIGH);Serial.println("Relay 4 Off");Serial.println("==============================");BT.print("h");g = 0;break;
    case 'E': digitalWrite  (Relay1,HIGH);digitalWrite  (Relay2,HIGH);digitalWrite  (Relay3,HIGH);digitalWrite  (Relay4,HIGH);Serial.println("All Relay Off");break;      //Saklar Off Semua

    // -------------------------------------------------- Setting Jam Off --------------------------------------------------
    case 'M':Jam1   = KodeJam;
             Menit1 = KodeMenit;
             Serial.print("Set Jam OFF Relay 1 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(10,Jam1);
             EEPROM.write(11,Menit1);
             Serial.println("==============================");
             break;
             
    case 'N':Jam2   = KodeJam;
             Menit2 = KodeMenit;
             Serial.print("Set Jam OFF Relay 2 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(12,Jam2);
             EEPROM.write(13,Menit2);
             Serial.println("==============================");
             break;
             
    case 'O':Jam3   = KodeJam;
             Menit3 = KodeMenit;
             Serial.print("Set Jam OFF Relay 3 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(14,Jam3);
             EEPROM.write(15,Menit3);
             Serial.println("==============================");
             break;
             
    case 'P':Jam4   = KodeJam;
             Menit4 = KodeMenit;
             Serial.print("Set Jam OFF Relay 4 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(16,Jam4);
             EEPROM.write(17,Menit4);
             Serial.println("==============================");
             break;
    
    // -------------------------------------------------- Set Timer On -------------------------------------------------- 
    case 'Q':JamA   = KodeJam;
             MenitA = KodeMenit;
             Serial.print("Set Jam ON Relay 1 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(0,JamA);
             EEPROM.write(1,MenitA);
             Serial.println("==============================");
             break;
             
    case 'R':JamB   = KodeJam;
             MenitB = KodeMenit;
             Serial.print("Set Jam ON Relay 2 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(2,JamB);
             EEPROM.write(3,MenitB);
             Serial.println("==============================");
             break;
             
    case 'S':JamC   = KodeJam;
             MenitC = KodeMenit;
             Serial.print("Set Jam ON Relay 3 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(4,JamC);
             EEPROM.write(5,MenitC);
             Serial.println("==============================");
             break;
             
    case 'T':JamD   = KodeJam;
             MenitD = KodeMenit;
             Serial.print("Set Jam ON Relay 4 : ");
             Serial.print(Convert(KodeJam));
             Serial.print(":");
             Serial.println(Convert(KodeMenit));
             EEPROM.write(6,JamD);
             EEPROM.write(7,MenitD);
             Serial.println("==============================");
             break; 
    } 
  }
}


//============================================== Fungsi Convert 2 Digit  ==============================================
String Convert(int Num){                                              // Variable String Convert nilai Int menjadi 2 digit untuk tampilkan jam
  if(Num < 10){                                                       // Jika Num kurang dari 10
    return "0"+String(Num);                                           // Tambahkan Angka '0' diawal variable, dan convert String
  }
  else{                                                               // Jika Num lebih dari 10
    return String(Num);                                               // Convert String
  }
}

//======================================================== END  ========================================================
