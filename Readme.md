- [Introduction](#Introduction)
- [Diagramme de communication](#Diagramme_de_communication)
- [Cahier des charges](#Cahier_des_charges)
- [Explication brève FAT32](#Explication_brève_FAT32)
- [Code review](#Code_Review)

## Introduction

Ce projet est réalisé dans le cadre du cours NSY103, où il s'agirait d'implémenter un système de gestion de fichiers FAT32 qui permettrait de lire le disque.

## Pré-requis

Pour le bon fonctionnement du programme il est nécessaire d'avoir **un image disk** que l'on créera avec un Makefile formater sour format Fat32. 
Etre sous un environnement linux est aussi primordiale car certaines commandes du Make ne s'execute que sur un système Unix. 
Pour des démos et pouvoir débugger, il est aussi nécessaire d'avoir l'utilitaire **mtools** sur Linux pour voir le contenu du disque. 


## Diagramme de communication



## Cahier des charges



## Explication brève FAT32

FAT est un système de gestion de fichiers développés par Microsoft principalement utilisés pour les petits disque (floppy disk et les cartes SD). \
Il a connu des évolutions au fil des années notamment sur la taille des fichiers qu'elle peut supporter. \
FAT de son abbréviation utilise une File Allocation Table qui pour garder en mémoire les clusters composés de secteurs de où sont associés les fichiers. \
Le nombre associés à chaque Fat correspond au nombre de bits pour entrées  dans la table.


Un système de fichier FAT doit au moins contenir ces 4 sections pour pouvoir lire un fichier dans le dique.

* BootSector : Se situe au début du disque et contient les informations necessaires pour le système de fichier  (exemple: les informations de format, le nombre de secteur par cluster etc ..)
* RootDirectory : Se situe après le booSector et contient les informations contenant le fichier recherché tel que le nom du fichier, son volume et le pointeur d'adresse qui contient l'adresse du premier clusster ou se situe le contenu du fichier.  
* FAT table region : Une table d'entrée de cluster qui référence l'adresse du pointeur suivant ou se situe le contenu d'un fichier. 
* Data Region : partition où se situe les données. 

## Code Review







