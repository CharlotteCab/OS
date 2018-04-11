#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Enumération des différents opérateurs possibles
typedef enum {
	OCD_SOMME,
	OCD_MOYENNE,
	OCD_MAX,
	OCD_MIN,
	OCD_TOTAL
} opcode_t;

// Structure contenant les arguments du programme
typedef struct {
	int nbThreads;			//< Nombre de threads à utiliser
	int tailleTableau;		//< Taille du tableau utilisé pour la réduction
	opcode_t code;			//< Opération à réaliser
} arg_t;

// Structure contenant les informations nécessaires au thread
typedef struct {
	int* t; 					//< Tableau d'entiers à traiter
	int start;						//< Indice de début de traitement
	int end;						//< Indice de fin de traitement (non compris)
	int res;						//< Résultat local
} message_t;

// Alias de pointeurs de fonction
typedef int (* ptrVerif) (int *, int, int);

// Fonction thread -- calcule la somme locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * sommeTableau (void * arg)
{
	message_t* msg=(message_t*) arg;
	msg->res=0;
	for(int i=msg->start;i<=msg->end;i++)
	{
		msg->res+=msg->t[i];
	}
	pthread_exit(NULL);
}

// Fin de la réduction -- calcule la somme globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducSomme (message_t * msg, int nbThreads)
{
	int res=0;
	for(int i=0; i<nbThreads;i++)
	{
		res+=msg[i].res;
	}
	return res;
}

// Fonction thread -- calcule la moyenne locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * moyenneTableau (void * arg) {  }

// Fin de la réduction -- calcule la moyenne globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMoyenne (message_t * msg, int nbThreads) {  }

// Fonction thread -- calcule le maximum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * maxTableau (void * arg) {  }

// Fin de la réduction -- calcule le maximum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMax (message_t * msg, int nbThreads) {  }

// Fonction thread -- calcule le minimum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * minTableau (void * arg) {  }

// Fin de la réduction -- calcule le minimum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMin (message_t * msg, int nbThreads) {  }

// NE PAS TOUCHER
// Fonction de vérification -- somme des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifSomme (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- moyenne des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMoyenne (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	if (tailleTableau == 0)
		return resultat == 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == (tmp / tailleTableau);
}

// NE PAS TOUCHER
// Fonction de vérification -- maximum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMax (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp < tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- minimum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMin (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 101;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp > tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// Analyse des arguments du programme. Quitte si le nombre d'arguments est
// différent de 4. Remplit la structure de données des arguments.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Structure de données des arguments
arg_t analyseArguments (int argc, char ** argv)
{
	if(argc!=4)
	{
		printf("Problème dans le nombre d'arguments\n");
		exit(-1);
	}
	arg_t a;
	a.nbThreads=atoi(argv[1]);
	a.tailleTableau=atoi(argv[2]);
	switch (argv[3][0])
	{
		case '+': {a.code=OCD_SOMME; break;}
		case '/': {a.code=OCD_MOYENNE; break;}
		case 'M': {a.code=OCD_MAX; break;}
		case 'm': {a.code=OCD_MIN; break;}
		default: exit(-1);
	}

	return a;
}

// NE PAS TOUCHER
// Récupération de la fonction de vérification à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return					Pointeur de la fonction de vérification
ptrVerif decodeOpcodeVerif (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return verifSomme;
		case OCD_MOYENNE:	return verifMoyenne;
		case OCD_MAX:		return verifMax;
		case OCD_MIN:		return verifMin;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Génération du tableau avec des entiers compris entre 1 et 100.
// \param	tailleTableau	Taille du tableau d'entiers
// \return					Tableau d'entiers
int * genereTableau (int tailleTableau)
{
	int i=0;
	int *t=malloc(tailleTableau*sizeof(int));
	for(i=0;i<tailleTableau;i++)
	{
		t[i]=rand()%100+1;
	}
	return t;
}

// Fonction chargée de la réduction multi-threadé, elle va initialiser les
// différentes variables utilisées par les threads (tableau d'entier, messages,
// etc.) puis créer les threads. Une fois l'exécution des threads terminée et
// la réduction opérée, la vérification du résultat est faite.
// \param	arg 			Arguments du programme décodés
void programmePrincipal (const arg_t arg) {
	// Déclaration des variables
	int * tab, res,taille;
	message_t *msg=malloc(arg.nbThreads*sizeof(message_t));
	pthread_t *t=malloc(arg.nbThreads*sizeof(pthread_t));

	// Allocation de la mémoire
	tab=genereTableau(arg.tailleTableau);
	if(t==NULL)
	{
		printf("Problème allocation mémoire\n");
		exit(-1);
	}

	// Initialisation des variables et création des threads
	taille=arg.tailleTableau/arg.nbThreads;
	for(int i=0;i<arg.nbThreads-1;i++)
	{
		msg[i].t=tab;
		msg[i].start=taille*i;
		msg[i].end=taille*(i+1);

		switch (arg.code)
		{
			case OCD_SOMME: pthread_create(&t[i],NULL,sommeTableau,&msg[i]);
			case OCD_MOYENNE: pthread_create(&t[i],NULL,moyenneTableau,&msg[i]);
			case OCD_MIN: pthread_create(&t[i],NULL,minTableau,&msg[i]);
			case OCD_MAX: pthread_create(&t[i],NULL,maxTableau,&msg[i]);
		}
	}
	msg[arg.nbThreads-1].t=tab;
	msg[arg.nbThreads-1].start=arg.tailleTableau%arg.nbThreads*(arg.nbThreads-1);
	msg[arg.nbThreads-1].end=arg.tailleTableau;

	switch (arg.code)
	{
		case OCD_SOMME: pthread_create(&t[arg.nbThreads-1],NULL,sommeTableau,&msg[arg.nbThreads-1]);
		case OCD_MOYENNE: pthread_create(&t[arg.nbThreads-1],NULL,moyenneTableau,&msg[arg.nbThreads-1]);
		case OCD_MIN: pthread_create(&t[arg.nbThreads-1],NULL,minTableau,&msg[arg.nbThreads-1]);
		case OCD_MAX: pthread_create(&t[arg.nbThreads-1],NULL,maxTableau,&msg[arg.nbThreads-1]);
	}

	// Jointure
	int i=0;
	for(i=0;i<arg.nbThreads;i++)
	{
		pthread_join(t[i],NULL);
	}

	// Réduction et affichage du résultat
	switch (arg.code)
	{
		case OCD_SOMME: res=reducSomme(msg,arg.nbThreads);
		case OCD_MOYENNE: res=reducMoyenne(msg,arg.nbThreads);
		case OCD_MIN: res=reducMin(msg,arg.nbThreads);
		case OCD_MAX: res=reducMax(msg,arg.nbThreads);
	}
	printf("Le résultat est: %d\n", res);

	// NE PAS TOUCHER
	if ( (* (decodeOpcodeVerif (arg.code) ) ) (tab, arg.tailleTableau, res) )
		printf ("Le resultat est juste.\n");
	else printf ("Le resultat est faux.\n");
	// FIN

	// Libération de la mémoire
	free(msg);
	free(t);
	free(tab);
}

// NE PAS TOUCHER
// Fonction main -- analyse les arguments donnés au
// programme, puis exécute le programme principal, chargé de la réduction
// multi-threadée.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Code de retour du programme (0 en cas de réussite)
int main (int argc, char ** argv) {
	arg_t a;

	srand (time (NULL) );

	a = analyseArguments (argc, argv);
	programmePrincipal (a);

	return 0;
}
