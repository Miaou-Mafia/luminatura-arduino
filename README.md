# Luminatura Arduino et Pure data

## Arduino 

Le code Arduino mesure la capacitance de l’utilisateur, qui correspond à la capacité du corps à stocker une charge électrique. Cette valeur est limitée à un maximum de 1000 pour assurer une calibration cohérente. Une plaque en acier sert de capteur et détecte les variations de capacitance lorsque l’utilisateur la touche la plaque en acier. Les données captées sont transmises via le port 8001, 8002 et 8003, utilisant une connexion réseau Ethernet pour la communication. PureData reçoit ces informations et les utilise pour générer des interactions sonores et visuelles en temps réel.

| Atom M5 Stack   | Ports    |
| ---------- | -------- |
| M5 Stack 1 | 8001 |
| M5 Stack 2 | 8002 |
| M5 Stack 3 | 8003 |

### Les différents ports utilisés
| Port  | Fonction                              |
| ----- | ------------------------------------- |
| 10001 | Qlc+                                  |
| 10002 | TouchDesigner - projection sur le sol |
| 10003 | Reaper incluant le fichier Plugdata (Plaque 01)   |
| 10005 | Reaper incluant le fichier Plugdata (Plaque 02)   |
| 10007 | Reaper incluant le fichier Plugdata (Plaque 03)   |
| 10004 | TouchDesigner - projection sur le mur |

## PureData
Le code PureData collecte les données brutes d'Arduino en fonction de la capacitance de l'utilisateur. Ensuite, un effet de transition fluide est appliqué pour lisser les valeurs. Puis, il envoie deux types de données : les données brutes et les données booléennes. Ces données sont transmises à des ports spécifiques pour chaque logiciel sur le PC.

