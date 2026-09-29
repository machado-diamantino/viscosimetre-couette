# Viscosimètre de Couette — Mesure de vitesse par détection optique

Projet académique en équipe de 3 · Licence 2 SPI, Capteurs et Mesures · 2025–2026 · Université Paris Nanterre (UFR SITEC)

Mesure sans contact de la vitesse de rotation du cylindre intérieur d'un viscosimètre de Couette, par coupure d'un faisceau laser sur une photorésistance (LDR), avec une carte Arduino Uno R3. La transposition sur Raspberry Pi 4 a été étudiée mais n'est pas terminée.

## Contexte

Dans un viscosimètre de Couette, un cylindre intérieur (rotor) tourne dans un fluide contenu par un cylindre extérieur fixe. La viscosité dépend du couple mesuré et de la vitesse angulaire :

`η = C · e / (2π · Ri³ · L · Ω)`

Il faut donc connaître la vitesse de rotation avec précision.

## Principe de la mesure

Un disque perforé (ou une marque) est fixé sur l'arbre du rotor. À chaque tour, il coupe le faisceau laser qui éclaire la LDR. Le signal est lu sur une entrée analogique de l'Arduino, converti en état logique avec un seuil (120 sur 1023, soit ≈ 0,59 V), puis chaque front montant donne une période :

`N (tr/min) = 60 / Δt`

Avantages : mesure sans contact, résolution temporelle de 1 ms (`millis()`), faible coût.

## Matériel

| Composant | Rôle |
|---|---|
| Arduino Uno R3 (ATmega328P) | Acquisition analogique, calcul des tr/min |
| Raspberry Pi 4 B | Traitement et interface (transposition étudiée) |
| Diode laser rouge 1 mW + LDR (1 kΩ à 1 MΩ) | Détection optique |
| Résistance 220 Ω, condensateur 1 µF | Pont diviseur et montage RC |
| 2 boutons poussoirs | Interface +1 / −1 (D2 et D3) |

## Programme Arduino

`arduino/viscosimetre_analogique/` : lecture de A0, détection de front montant, calcul de la vitesse, gestion des boutons (INPUT_PULLUP, anti-rebond logiciel). Sortie série à 9600 bauds au format CSV : `temps(ms),RPM,V`.

## Résultats et limites

- Preuve de concept validée sur Arduino : détection de fronts fonctionnelle et calcul des tr/min.
- Vitesse maximale mesurable d'environ 2500 tr/min, limitée par `delay(20)` dans la boucle.
- Seuil fixe sensible à la lumière ambiante (risque de faux positifs).

## Partie Raspberry Pi 4

Le Raspberry Pi 4 n'a **aucune entrée analogique** : ses GPIO sont numériques (0 / 3,3 V). Faute de convertisseur externe disponible, un montage RC (LDR + 220 Ω + 1 µF) a été testé (`arduino/test_montage_rc/`) pour convertir la résistance de la LDR en temps de charge.

Résultat : la tension du condensateur ne franchit pas le seuil logique de la broche numérique, la détection directe n'est pas fiable. La transposition sur Raspberry Pi n'a donc pas abouti.

Pistes identifiées :
1. CAN externe : MCP3008 (SPI) ou ADS1115 (I2C).
2. Arduino en esclave : acquisition temps réel sur l'Arduino, envoi des données au Raspberry Pi par USB/UART.
3. Comparateur à seuil avec hystérésis (LM339 / LM393) pour obtenir un signal numérique propre.

## Utilisation

1. Câbler la LDR et la résistance de 220 Ω en pont diviseur sur A0, les boutons sur D2 et D3.
2. Ouvrir `arduino/viscosimetre_analogique/viscosimetre_analogique.ino` dans l'IDE Arduino et le téléverser.
3. Ouvrir le moniteur ou le traceur série à 9600 bauds.

## Contenu du dépôt

```
arduino/viscosimetre_analogique/   Programme final (mesure des tr/min)
arduino/test_montage_rc/           Test du montage RC
docs/                              Rapport et poster du projet
```

## Équipe

Diamantino Machado et deux autres étudiants de L2 SPI.
