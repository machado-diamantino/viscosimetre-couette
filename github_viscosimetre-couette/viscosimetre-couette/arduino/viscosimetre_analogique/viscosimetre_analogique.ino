// Mesure de vitesse de rotation par coupure de faisceau laser / LDR
// Arduino Uno R3 - version analogique
// Sortie série (9600 bauds) : temps(ms),RPM,V

const int capteur  = A0;   // photorésistance (pont diviseur avec R = 220 ohms)
const int btnPlus  = 2;    // bouton +1
const int btnMoins = 3;    // bouton -1
int seuil = 120;           // seuil de détection sur la lecture analogique (0-1023)

int etat = 0;
int ancienEtat = 0;

unsigned long tempsDernierPassage = 0;  // pour le calcul des RPM
int V = 0;                              // variable contrôlée par les boutons

void setup() {
  Serial.begin(9600);
  pinMode(btnPlus, INPUT_PULLUP);   // bouton relié à GND -> INPUT_PULLUP
  pinMode(btnMoins, INPUT_PULLUP);
  tempsDernierPassage = millis();
}

void loop() {
  // Lecture de la photorésistance
  int valeur = analogRead(capteur);
  unsigned long temps = millis();
  etat = (valeur < seuil) ? 1 : 0;

  // Front montant : passage détecté
  if (etat == 1 && ancienEtat == 0) {
    float rpmInstant = 0;
    if (tempsDernierPassage > 0) {
      float deltaTemps = (temps - tempsDernierPassage) / 1000.0;  // ms -> s
      rpmInstant = (1.0 / deltaTemps) * 60.0;                     // RPM instantané
    }
    tempsDernierPassage = temps;

    // Sortie pour le traceur série : temps (ms), RPM, V
    Serial.print(temps);
    Serial.print(",");
    Serial.print(rpmInstant);
    Serial.print(",");
    Serial.println(V);
  }

  ancienEtat = etat;

  // Lecture des boutons pour V
  if (digitalRead(btnPlus) == LOW) {
    V += 1;
    delay(200);   // anti-rebond simple
  }
  if (digitalRead(btnMoins) == LOW) {
    V -= 1;
    delay(200);
  }

  delay(20);      // lecture stable
}
