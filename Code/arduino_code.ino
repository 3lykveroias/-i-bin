/**
 * @brief This code controls an Arduino-based system that uses sensors to detect the level of trash in a bin and alert the user when it is full. 
 * It uses two HC-SR04 ultrasonic sensors to measure the distance of the trash level from the top of the bin, an MQ-2 smoke sensor to detect smoke, 
 * and four LEDs to indicate the level of the trash. It also includes a servo motor to open and close the lid of the bin. 
 * 
 * The code initializes the pins for the sensors, LEDs, and servo motor, and defines the threshold for the smoke sensor. 
 * It then enters a loop where it reads the sensor values and performs the necessary actions based on the readings. 
 * If the distance measured by the outer HC-SR04 sensor is less than or equal to 5 cm, the lid of the bin is opened for 3 seconds. 
 * If the distance measured by the inner HC-SR04 sensor is less than or equal to 6 cm, the appropriate LED is turned on to indicate the trash level. 
 * If the smoke sensor detects smoke above the defined threshold, the buzzer is turned on. 
 * 
 * @note This code requires the Servo library to be installed in the Arduino IDE.
 */
// #include <stdio.h>      //βιβλιοθήκες C για διαχωρισμό των string. Δεν χρειάστηκε καθώς ο διαχωρισμός έγινε στο ESP8266.
// #include <string.h>     //βιβλιοθήκες C για διαχωρισμό των string. Δεν χρειάστηκε καθώς ο διαχωρισμός έγινε στο ESP8266.
#include <Servo.h>

int trigPin_outer = 2; // Ψηφιακή έξοδος Trigger εξωτερικού αισθητήρα hc-sr04. Το Trigger είναι πάντα Digital PIN ενώ το Echo μπορεεί να είναι PWM η οποιoδήποτε Digital PIN.
int echoPin_outer = 3; // Ψηφιακή είσοδος Echo εξωτερικού αισθητήρα hc-sr04

int trigPin_inner = 4; // Ψηφιακή έξοδος Trigger εσωτερικού αισθητήρα hc-sr04
int echoPin_inner = 5; // Ψηφιακή είσοδος Echo εσωτερικού αισθητήρα hc-sr04

long duration_outer; // Διάρκεια παλμού εξωτερικού αισθητήρα hc-sr04
long cm_outer;       // Απόσταση εξωτερικού αισθητήρα hc-sr04

long duration_inner; // Διάρκεια παλμού εσωτερικού αισθητήρα hc-sr04
long cm_inner;       // Απόσταση εσωτερικού αισθητήρα hc-sr04

int smoke = A0;       // Αναλογική είσοδος αισθητήρα καπνού mq-2
int smokeThres = 130; // Όριο συναγερμού αισθητήρα καπνού mq-2

int buzzer = 6; // Έξοδος του active buzzer

int white = A1;  // Έξοδος λευκού LED
int green = A2;  // Έξοδος λευκού LED
int yellow = A3; // Έξοδος λευκού LED
int red = A4;    // Έξοδος λευκού LED

int servopin = 11; // Pin που είναι συνδεδεμένος ο servo κινητήρας. Μάλλον χρειάζεται pwm

int send_time = 0;

Servo lidservo; // Δημιουργία ενός αντικειμένου με όνομα myservo για τον έλεγχο του κινητήρα ανοίγματος. PWM pins: 3, 5, 6, 9, 11

void setup()
{
  // Έναρξη Serial Port
  Serial.begin(9600);

  // Ορισμός εισόδων inputs και εξόδων outputs
  pinMode(trigPin_outer, OUTPUT);
  pinMode(echoPin_outer, INPUT);

  pinMode(trigPin_inner, OUTPUT);
  pinMode(echoPin_inner, INPUT);

  pinMode(smoke, INPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(white, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);

  lidservo.attach(servopin); // Προσάρτηση (attach) του servo κινητήρα στο pin 11, στο αντικείμενο servo
}

// Συνάρτηση που καλείται για να σβήνουν όλα τα led.
void LEDS_out()
{
  digitalWrite(white, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
}

void loop()
{
  int smokeSensor = analogRead(smoke);
  digitalWrite(buzzer, HIGH);

  // Εξωτερικός αισθητήρας hc-sr04
  //  Ο αισθητήρας στέλνει ένα σήμα με τη μορφή ήχου υψηλής συχνότητας. Μέσω του trig pin αποστέλλεται ένας HIGH παλμός 10 ή περισσότερων δευτερολέπτων.
  //  Στην αρχή δίνουμε έναν LOW παλμό για να εξασφαλίσουμε έναν καθαρό (χωρίς θόρυβο) HIGH παλμό.
  digitalWrite(trigPin_outer, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_outer, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_outer, LOW);

  // Εξωτερικός αισθητήρας hc-sr04
  //  Διαβάζουμε το σήμα από τον αισθητήρα. Έναν HIGH παλμό του οποίου η χρονική διάρκεια (σε microsecond) ισούται
  //  με τον χρόνο που μεσολαβεί από την αποστολή του παλμού από το trig pin μέχρι την επιστροφή στο echo pin από το αντικείμενο που συνάντησε.
  pinMode(echoPin_outer, INPUT);
  duration_outer = pulseIn(echoPin_outer, HIGH);

  // Εξωτερικός αισθητήρας hc-sr04
  //  Μετατροπή του χρόνου σε απόσταση (εκατοστά). Διαιρούμε με το 29.1
  cm_outer = (duration_outer / 2) / 29.1;

  // Εσωτερικός αισθητήρας hc-sr04
  //  Ο αισθητήρας στέλνει ένα σήμα με τη μορφή ήχου υψηλής συχνότητας. Μέσω του trig pin αποστέλλεται ένας HIGH παλμός 10 ή περισσότερων δευτερολέπτων.
  //  Στην αρχή δίνουμε έναν LOW παλμό για να εξασφαλίσουμε έναν καθαρό (χωρίς θόρυβο) HIGH παλμό.
  digitalWrite(trigPin_inner, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_inner, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_inner, LOW);

  // Εσωτερικός αισθητήρας hc-sr04
  //  Ο αισθητήρας στέλνει ένα σήμα με τη μορφή ήχου υψηλής συχνότητας. Μέσω του trig pin αποστέλλεται ένας HIGH παλμός 10 ή περισσότερων δευτερολέπτων.
  //  Στην αρχή δίνουμε έναν LOW παλμό για να εξασφαλίσουμε έναν καθαρό (χωρίς θόρυβο) HIGH παλμό.
  pinMode(echoPin_inner, INPUT);
  duration_inner = pulseIn(echoPin_inner, HIGH);

  // Εσωτερικός αισθητήρας hc-sr04
  //  Μετατροπή του χρόνου σε απόσταση (εκατοστά). Διαιρούμε με το 29.1
  cm_inner = (duration_inner / 2) / 29.1;

  //--------------Δοκιμαστικές εντολές για τον έλεγχο των μεταβλητών--------------
  // Serial.print("Εξωτερική απόσταση ");
  // Serial.print(cm_outer);
  // Serial.print("cm");
  // Serial.println();
  //--------------------------Τέλος δοκιμαστικών εντολών--------------------------

  // Άνοιγμα και κλείσιμο του κάδου. Όταν ο κάδος ανοίγει, βεβαιονόμαστε ότι όλα τα LED είναι σβηστά,
  // για να μη δίνεται λανθασμένη ένδειξη από τον αισθητήρα στάθμης που μετακινείται διαγώνια πάνω στο καπάκι του κάδου.
  if (cm_outer <= 5)
  {
    LEDS_out();          // Σβήσιμο των LED
    lidservo.write(90);  // Άνοιγμα του κάδου
    delay(3000);         // για 3 δευτερόλεπτα
    lidservo.write(-90); // Κλείσιμο του κάδου
  }

  //--------------Δοκιμαστικές εντολές για τον έλεγχο των μεταβλητών--------------
  // Serial.print("Εσωτερική απόσταση ");
  // Serial.print(cm_inner);
  // Serial.print("cm");
  // Serial.println();
  //--------------------------Τέλος δοκιμαστικών εντολών--------------------------

  // Έλεγχος της στάθμης τουυ κάδου και άναμα του αντίστοιχου LED.
  // Βεβαιονόμαστε ότι είναι όλα σβηστά και κατόπιν ανάβει το κατάλληλο ανάλογα με τη στάθμη.
  if (cm_inner <= 6)
  {
    LEDS_out();
    digitalWrite(red, HIGH);
  }
  else if (cm_inner <= 12)
  {
    LEDS_out();
    digitalWrite(yellow, HIGH);
  }
  else if (cm_inner <= 18)
  {
    LEDS_out();
    digitalWrite(green, HIGH);
  }
  else if (cm_inner <= 25)
  {
    LEDS_out();
    digitalWrite(white, HIGH);
  }

  //--------------Δοκιμαστικές εντολές για τον έλεγχο των μεταβλητών--------------
  // Serial.print("Pin A0: ");
  // Serial.print(smokeSensor);
  //--------------------------Τέλος δοκιμαστικών εντολών--------------------------

  // Έλεγχος του αισθθητήρα καπνού ανάλογα με το όριο που έχουμε θέση. Αν εντοπιστεί καπνός, χτυπάει το buzzer, αλλιώς σταματάει.
  if (smokeSensor > smokeThres)
  {
    digitalWrite(buzzer, LOW);
    // Serial.println("Φωτιά");
  }
  else
  {
    digitalWrite(buzzer, HIGH);
    // Serial.println("Δεν ανιχνεύθηκε καπνός");
  }
  // Serial.println();

  // Ορισμός και αρχικοποίηση του string httpRequestData το οποίο αποτελεί τα δεδομένα μας από τους αισθητήρες
  // και θα αποσταλεί στο ESP8266 για να μεταφορτωθεί στην php μας βάση στο διαδίκτυο.
  String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=" + String(cm_inner) + "&value2=" + String(smokeSensor) + "&value3=1005.14";

  //--------------Δοκιμαστικές εντολές για τον έλεγχο των μεταβλητών--------------
  // Serial.println(httpRequestData);
  // Serial.println(cm_outer);
  //--------------------------Τέλος δοκιμαστικών εντολών--------------------------

  // Αποστολή δεδομένων στο ESP8266 μεσω σειριακής επικοινωνίας, όταν πάρει φωτιά ή γεμίσει ο κάδος.
  // Το μικρό delay υπάρχει για να μην "μπουκώσει" η επικοινωνία με το ESP8266 λόγω της συχνής αποστολής
  if (smokeSensor > smokeThres || cm_inner <= 6)
  {
    Serial.println(httpRequestData);
    delay(500);
  }

  // Αποστολή δεδομένων ανα 5 λεπτά: 150 φορές ανα 2 δευτερόλεπτα = 300 δευτερόλεπτα, δηλαδή 5 λεπτά.
  // Αυτή είναι η τακτική αποστολή δεδομένων, όταν ο κάδος δεν είναι γεμάτος ή δεν κινδυνεύει.
  // Ο χρόνος τωων πέντε λεπτών είναι ενδεικτικός και μπορεί να αλλάξει ανάλογα με το πόσο συχνά χρησιμοποιείται ο κάδος.
  if (send_time == 0)
  {
    Serial.println(httpRequestData);
    delay(500);
  }
  else
  {
    if (send_time >= 150)
    { // αποστολή δεδομένων ανα 5 λεπτά: 150 φορές ανα 2 δευτερόλεπτα = 300 δευτερόλεπτα, δηλαδή 5 λεπτά
      Serial.println(httpRequestData);
      delay(500);
      send_time = 0;
    }
  }

  // Η μεταβλητή send_time χρησιμοποιείται για να μετρηθεί το χρονικό διάστημα των πέντε λεπτών.
  send_time = send_time + 1;

  delay(2000);
}
