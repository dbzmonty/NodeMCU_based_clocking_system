// INCLUDE RÉSZ
#include <ESP8266WiFi.h>           // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>

// RFID RÉSZ
#define RST_PIN 20 // RST-PIN for RC522 - RFID - SPI - Module GPIO15 
#define SS_PIN  2  // SDA-PIN for RC522 - RFID - SPI - Module GPIO2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

// OLED RÉSZ
#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

// WIFI RÉSZ
char ssid[] = "Dbz-Villamnet";          // your SSID
char pass[] = "19850317";               // your SSID Password
WiFiClient client;                      // Use this for WiFi instead of EthernetClient

// MYSQL RÉSZ
IPAddress server_addr(192,168,1,151);   // IP of the MySQL *server* here
char user[] = "dbz";                    // MySQL user login username
char password[] = "12345";              // MySQL user login password
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cur = MySQL_Cursor(&conn);

void setup()
{
  Serial.begin(115200);

  // RFID START
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522

  // OLED START
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);  
  display.setCursor(0,0);

  // CSATLAKOZÁS A WIFI-HEZ
  display.println("Connecting to: ");
  display.println(ssid);
  display.display();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  display.println("Connected to WiFi");
  display.println("My IP address is: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
  display.setTextSize(2);  
}

void loop()
{  
  display.clearDisplay();
  display.display();
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {   
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {   
    delay(50);
    return;
  }

  // Shows the card ID on the serial console
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  
  content.toUpperCase();
  
  // KONVERTÁLÁS STRING-BŐL CHAR-BA A CARD-ID-T
  String s = content.substring(1);
  int n = s.length(); 
  char card_id[n+1];
  strcpy(card_id, s.c_str()); 
    
  // CSATLAKOZÁS A MYSQL-HEZ
  if (conn.connect(server_addr, 3306, user, password)) {
    Serial.println("MYSQL CONNECTED");
  }
  else {
    Serial.println("MYSQL CAN'T CONNECT");
    display.setCursor(0,0);
    display.println("NETWORK");
    display.println("FAILURE");
    display.display();
  }
  
  //USER LEKÉRDEZÉSE CARD_ID ALAPJÁN
  char QUERY_POP[] = "SELECT user_id, first_name, last_name, is_clocked_in FROM clocking.users WHERE card_id = '%s'";
  char query[128];
  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  //  változót adunk meg a lekérdezés paraméterének
  //  kimenete, bemenete (%i int ; %s string azaz char[]), amit hozzáfűzünk  
  sprintf(query, QUERY_POP, card_id);
  cur_mem->execute(query);
  column_names *cols = cur_mem->get_columns();
   
  // KIOLVASNI AZ EREDMÉNYT
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {        
      if (strcmp((row->values[3]), "0")  == 0)
      {
        Serial.println("Nincs bent");
        
        // HA NINCS BENT, AKKOR LÉTREHOZNI EGY ÚJ EVENT-ET
        char query_new_event_pop[] = "INSERT INTO clocking.events (user_id, event_ended) VALUES (%i, 0)";       
        char query_new_event[128]; 
        
        // INT-BEN RÖGZÍTENI AZ USER ID-JÉT
        int user_id = atol(row->values[0]);
        sprintf(query_new_event, query_new_event_pop, user_id);
        
        // FUTTATNI A LEKÉRDEZÉST
        cur_mem->execute(query_new_event);
        
        // AZ USER-T ÁTÁLLÍTANI BENT LÉVŐNEK
        char query_edit_user_pop[] = "UPDATE clocking.users SET is_clocked_in=1 WHERE user_id=%i"; 
        char query_edit_user[128]; 
        sprintf(query_edit_user, query_edit_user_pop, user_id);

        // FUTTATNI A LEKÉRDEZÉST
        cur_mem->execute(query_edit_user);   

        // KÖSZÖNTENI A DOLGOZÓT
        String first_name = row->values[1];
        String last_name = row->values[2];
        display.setCursor(0,0);
        display.println("Welcome");
        display.println(first_name);
        display.println(last_name);
        display.display();        
      }
      else if (strcmp((row->values[3]), "1")  == 0)
      {
        Serial.println("Bent van");
        // HA BENT VAN AKKOR KILÉPTETNI
        // INT-BEN RÖGZÍTENI AZ USER ID-JÉT
        int user_id = atol(row->values[0]);
        
        // AZ USER-T ÁTÁLLÍTANI KIJELENTKEZETTNEK
        char query_edit_user_out_pop[] = "UPDATE clocking.users SET is_clocked_in=0 WHERE user_id=%i"; 
        char query_edit_user_out[128]; 
        sprintf(query_edit_user_out, query_edit_user_out_pop, user_id);

        // FUTTATNI A LEKÉRDEZÉST
        cur_mem->execute(query_edit_user_out);   

        // AZ EVENTET LEZÁRNI
        char query_edit_event_pop[] = "UPDATE clocking.events SET event_ended=1 WHERE user_id=%i AND event_ended=0"; 
        char query_edit_event[128]; 
        sprintf(query_edit_event, query_edit_event_pop, user_id);

        // FUTTATNI A LEKÉRDEZÉST
        cur_mem->execute(query_edit_event);   

        // ELKÖSZÖNNI A DOLGOZÓTÓL
        String first_name = row->values[1];
        String last_name = row->values[2];
        display.setCursor(0,0);
        display.println("Bye Bye");
        display.println(first_name);
        display.println(last_name);
        display.display();        
      }        
    }    
  } while (row != NULL);
  delete cur_mem;
  conn.close();  
  Serial.println("MYSQL DISCONNECTED");
  delay(3000);
}
