// Test du montage RC (LDR + 220 ohms + 1 uF) sur Arduino Uno R3
// Objectif : observer la charge / décharge du condensateur et tester
// une détection numérique directe (pin 7).

const int capteur    = A0;  // point commun RC (entre LDR et C)
const int pinOut     = 12;  // commande de charge / décharge
const int pinDigital = 7;   // entrée numérique de test

void setup() {
  Serial.begin(9600);
  pinMode(pinOut, OUTPUT);
  pinMode(pinDigital, INPUT);
}

void loop() {
  int etat = digitalRead(pinDigital);

  // PHASE 1 : CHARGE
  Serial.println("CHARGE");
  digitalWrite(pinOut, HIGH);
  for (int i = 0; i < 20; i++) {
    int valeur = analogRead(capteur);
    Serial.println(valeur);
    if (etat == HIGH) {
      Serial.println("DIGITAL = HIGH (lumiere detectee)");
    } else {
      Serial.println("DIGITAL = LOW (pas de lumiere)");
    }
    delay(5);
  }

  // PHASE 2 : DÉCHARGE
  Serial.println("DECHARGE");
  digitalWrite(pinOut, LOW);
  for (int i = 0; i < 20; i++) {
    int valeur = analogRead(capteur);
    Serial.println(valeur);
    delay(5);
  }
}
