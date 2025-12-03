# IOT_Agriculture — Système de Capteurs et Communication LoRaWAN

**Projet IoT — Semestre 1, 2025**
**École Nationale Supérieure Polytechnique de Yaoundé — Département Informatique**

---

## Aperçu du Projet

**IOT_Agriculture** est un système de surveillance et de contrôle pour parcelles agricoles, utilisant la technologie **LoRaWAN** pour assurer une communication fiable sur de longues distances.

Le projet est divisé en deux plateformes matérielles distinctes mais complémentaires :

1.  **LoRaWAN Gateway (RIOT OS) :** Gestion de la connectivité réseau et de l'envoi/réception des données vers un Network Server
2.  **Interface Locale (Arduino) :** Gestion de l'interaction utilisateur sur site (affichage, saisie des commandes).

---

##  Objectifs Techniques

* Maîtriser l'**initialisation de la pile LoRaMAC** (OTAA/ABP).
* Implémenter la **communication bi-directionnelle** (Uplink/Downlink).
* Gérer les **périphériques d'interface** (Clavier, Écran LCD I2C) sur plateforme Arduino.
* Séparer les logiques d'interface et de réseau en **modules distincts**.
* Déboguer et valider les **clés OTAA** et les **adresses I2C**.

---

## Technologies Clés

| Composant | Technologie | Rôle |
|----|---------|--------------|
| **Réseau** | LoRaWAN, Semtech LoRaMAC | Communication LPWAN longue portée |
| **OS Embarqué** | RIOT OS | Gestion des threads et du hardware pour le LoRaWAN |
| **Interface** | Arduino (Langage C/C++) | Affichage local et saisie utilisateur |
| **I/O** | Clavier matriciel 4x4, LCD I2C (0x27) | Interaction physique |
| **Débogage** | I2C Scanner, Moniteur Série | Outils de validation matérielle |

---

## Structure du Dépôt

```bash
IOT_Agriculture/
├── LoRaWAN_Gateway/                 # Code RIOT OS (C) pour la connectivité
│   └── main.c                       # Gestion OTAA, Send/Receive, Threads
│   └── Makefile                     # Fichier de compilation RIOT OS
|
└── Local_Interface_Arduino/         # Code Arduino (.ino) pour l'interface utilisateur
    ├── LCD_Keypad_UI/               # Code d'interface final
    │   └── lcd_i2c.ino              # Logique de saisie de parcelle et d'affichage
    |
    └── Tools/                       # Outils de validation matérielle
        ├── keyboard.ino             # Test du clavier
        └── i2c_address_find.ino     # Scanner d'adresse I2C
|
├── .gitignore
├── README.md
└── [docs/ ]
