/*Κωδικας Μαθητων Β' Λυκείου 3ο Γενικό Λύκειο Βεροιας 2019-2020 
 *Για το project i bin προκειμένου να χρησιμοποιηθεί για τον Διαγωνισμό Ανοιχτών Τεχνολογιών (ΕΛΛΑΚ)
 *Ιστοσελίδα Σχολείου http://3lyk-veroias.ima.sch.gr/autosch/joomla25/
 *The code requires the library:https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
*/
//Εναλλακτικές δηλώσεις βιβλιοθηκών για τη χρηση ESP32 αντί ESP8266
//  #ifdef ESP32
//  #include <WiFi.h>
//  #include <HTTPClient.h>
//#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
//#endif

#include <Wire.h>

// Στοιχεία σύνδεση στο ασύρματο δίκτυο
const char* ssid     = "HUAWEI-E5330-7138";
const char* password = "rbh0329t";

// Domain name και URL path προς το αρχείο php που εισάγει τα δεδομένα μας στη βάση mysql.
// Αν δεν γνωρίζουμε το domain name, μπορούμε να βάλουμε την IP διεύθυνση με το path
// π.χ. 192.168.1.4/dimlytsios/test/post-esp-data_modified.php
const char* serverName = "http://users.sch.gr/dimlytsios/test/post-esp-data_modified.php";

// Ορισμός ενός API key για λόγους ασφφαλείας στη μεταφφόρτωση των δεδομένων. 
// Αν το αλλάξουμε, πρέπει να το αλλάξουμε και στο αρχείο /post-esp-data.php 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "BME280";
String sensorLocation = "Office";




void setup() {
  Serial.begin(9600);
  /*while (!Serial){
    ;
  }*/

  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  //Έλεγχος της κατάστασης σύνδεσης στο ασύρματο δίκτυο
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Το Domain name με το URL path που ορίσαμε παραπάνω ή η IP διεύθυνση με το path
    http.begin(serverName);
    
    // Καθορισμός του τύπου των δεδομένων αποστολής
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Παράδειγμα αποστολής δεδομένων όταν γνωρίζουμε απευθείας τις τιμές (HTTP POST request data)
    // Ένωση των τιμών σε ένα string και αποστολή.
    /*String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(bme.readTemperature())
                          + "&value2=" + String(bme.readHumidity()) + "&value3=" + String(bme.readPressure()/100.0F) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);*/

    
    
    String httpRequestData = "";
    char character;

    //Παραλαβή ενός ενός byte δεδομένων από τη serial του nano, μετατροπή σε char και επαναληπτική ένωση για να σχηματιστεί ένα string αποστολής.
    //Το delay είναι ενδεικτικό για να μην "μπουκώνει" η σειριακή επικοινωνία μεταξύ ESP8266 και nano και διακόπτεται η επικοινωνία
    while(Serial.available()) {
      character = Serial.read();
      httpRequestData.concat(character);
      delay(10);
      }
      
    if (httpRequestData != "") {
      Serial.println(httpRequestData);
      }

    //Εκτύπωση στην οθόνη των στοιχείων πριν τα μεταφορτώσουμε στο διαδίκτυο
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Μεταφόρτωση των δεδομένων Send HTTP POST 
    int httpResponseCode = http.POST(httpRequestData);
     
    // Παράδειγμα αποστολής δεδομένων τύπου: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // Παράδειγμα αποστολής δεδομένων τύπου: application/json
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // Άδειασμα των πόρων μεταφόρτωσης
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
   
}
/*Ευχαριστούμε τον  Rui Santos απο τον οποίο προήλθε ο αρχικός κώδικας χωρίς τις δικές μας μετατροπές 
 *Ιστοσελιδα προέλευσης: https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
*/
