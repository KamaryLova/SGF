- [Introduction](#Introduction)
- [Diagramme de communication](#Diagramme_de_communication)
- [Cahier des charges](#Cahier_des_charges)
- [Fat 32 Explanation](#Fat32_Explanation)
- [Code review](#Code_Review)

## Introduction

Ce projet est réalisé dans le cadre du cours NSY103, où il s'agirait d'implémenter un système de gestion de fichiers FAT32 qui permettrait de lire le disque.

## Pré-requis

Pour le bon fonctionnement du programme il est nécessaire d'avoir **un image disk** que l'on créera avec un Makefile formater sour format Fat32. 
Etre sous un environnement linux est aussi primordiale car certaines commandes du Make ne s'execute que sur un système Unix


## Diagramme de communication





## Cahier des charges



## Fat32 Explanation

FAT est un système de gestion de fichiers développés par Microsoft principalement utilisés pour les petits disque (floppy disk et les cartes SD). \
Il a connu des évolutions au fil des années notamment sur la taille des fichiers qu'elle peut supporter. \
FAT de son abbréviation utilise une File Allocation Table qui pour garder en mémoire les clusters composés de secteurs de où sont associés les fichiers. \
Le nombre associés à chaque Fat correspond au nombre de bits pour entrées  dans la table.


Un système de fichier FAT doit au moins contenir ces 4 sections pour pouvoir lire un fichier dans le dique.

* BootSector : 
* RootDirectory :
* FAT table region : 
* Data Region :



## Code Review





