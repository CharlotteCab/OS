#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "se_fichier.h"
#include "unistd.h"


SE_FICHIER SE_ouverture (const char * chemin,	int acces)
{
	SE_FICHIER Fi;
	Fi.descripteur = open(chemin, acces|O_CREAT, 0660);
	if(Fi.descripteur < 0) return Fi;

	/*Fi.chemin = chemin;
	Fi.acces = acces;*/

	return Fi;
}

int SE_fermeture (SE_FICHIER fichier)
{
	return close(fichier.descripteur);
}

int SE_suppression (const char * chemin)
{
	return unlink(chemin);
}

int SE_lectureCaractere (SE_FICHIER fichier, char * caractere)
{
	return read(fichier.descripteur, caractere, 1);
}

int SE_ecritureCaractere(SE_FICHIER fichier,const char caractere)
{
	return write(fichier.descripteur, &caractere, sizeof(char));
}
