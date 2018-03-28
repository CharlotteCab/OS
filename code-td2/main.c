#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "se_fichier.h"

// Affichage du contenu d'un fichier
// \param	chemin			Nom/chemin du fichier à afficher
// \return					-1 si échec à l'ouverture, 0 sinon
int affichage (const char * chemin)
{
	SE_FICHIER Fi;
	char c_lire;
	int B_Ret;

	Fi = SE_ouverture(chemin, O_RDONLY);
	B_Ret = SE_lectureCaractere(Fi, &c_lire);

	printf("%c", c_lire);
	while( (B_Ret = SE_lectureCaractere(Fi, &c_lire)) > 0)
	{
		printf("%c", c_lire);
	}
	if(B_Ret < 0)
	{
		return -1;
	}

	SE_fermeture(Fi);

	return 0;
}
// Copie de fichier
// \param	chemin1			Nom/chemin du fichier source
// \param	chemin2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int copie (const char * chemin1, const char * chemin2)
{
		SE_FICHIER fd, ff;
		fd=SE_ouverture(chemin1, O_RDONLY);
		ff=SE_ouverture(chemin2,O_WRONLY);

		if(fd.descripteur<0 || ff.descripteur<0) return -1;

		char c_lire;
		int B_ret;
		B_ret=SE_lectureCaractere(fd,&c_lire);
		while(B_ret > 0)
		{
			if(SE_ecritureCaractere(ff,c_lire)<0)
			{
				return -2;
			}
			B_ret=SE_lectureCaractere(fd,&c_lire);
		}
		if(B_ret<0) return -2;

		SE_fermeture(fd);
		SE_fermeture(ff);

		return 0;
}

// Déplacement de fichier
// \param	chemin1			Nom/chemin du fichier source
// \param	chemin2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int deplacement (const char * chemin1, const char * chemin2)
{
	if(copie(chemin1,chemin2)<0) return -1;
	if(SE_suppression(chemin1)<0) return -1;

	return 0;
}
// Comparaison entre deux fichiers
// \param	chemin1			Nom/chemin du premier fichier
// \param	chemin2			Nom/chemin du second fichier
// \return					-1 si échec à l'ouverture,
//							1 si fichiers identiques, 0 sinon
int sontIdentiques (const char * chemin1, const char * chemin2)
{
	SE_FICHIER f1,f2;
	int B_ret1,B_ret2;
	char c_lire1,c_lire2;
	f1=SE_ouverture(chemin1,O_RDONLY);
	f2=SE_ouverture(chemin2,O_RDONLY);

	if(f1.descripteur<0 || f2.descripteur<0) return -1;

	B_ret1=SE_lectureCaractere(f1,&c_lire1);
	B_ret2=SE_lectureCaractere(f2,&c_lire2);

	while(B_ret1>0 || B_ret2>0)
	{
		if((c_lire1!=c_lire2) || (B_ret1!=B_ret2)) return 0;
		B_ret1=SE_lectureCaractere(f1,&c_lire1);
		B_ret2=SE_lectureCaractere(f2,&c_lire2);
	}
	SE_fermeture(f1);
	SE_fermeture(f2);
	return 1;
}

void verifAffichage () {
	int nstdout = creat ("test2.txt", 0644);
	int tmp, tmp2;

	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	tmp = dup (fileno (stdout) );

	// Vider (entre autre) le buffer de stdout
	fflush (NULL);
	dup2 (nstdout, fileno (stdout) );

	affichage ("test1.txt");

	// Vider (entre autre) le buffer de stdout
	fflush (NULL);
	dup2 (tmp, fileno (stdout) );

	close (nstdout); close (tmp);

	if (! system ("diff test1.txt test2.txt >diff.log") )
		printf ("'affichage()' test has succeeded.\n");
	else
		printf ("'affichage()' test has failed.\n");

	system ("rm -f diff.log test1.txt test2.txt");
}

void verifCopie () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	copie ("test1.txt", "test2.txt");

	if (! system ("diff test1.txt test2.txt >diff.log") )
		printf ("'copie()' test has succeeded.\n");
	else
		printf ("'copie()' test has failed.\n");

	system ("rm -f diff.log test1.txt test2.txt");
}

void verifDeplacement () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system ("cat test1.txt >test2.txt");

	deplacement ("test2.txt", "test3.txt");

	if (! system ("test -e test2.txt") )
		printf ("'deplacement() test has failed.\n");
	else if (! system ("diff test1.txt test3.txt >diff.log") )
		printf ("'deplacement()' test has succeeded.\n");
	else
		printf ("'deplacement()' test has failed.\n");

	system ("rm -f diff.log test1.txt test3.txt");
}

void verifSontIdentiques () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system ("echo 'This file\nis\n\nGREAT!\n' >test2.txt");
	system ("echo 'This file\nis\n\nGREAT!\n\n' >test3.txt");
	system ("echo 'This fil\nis\n\nGREAT!\n' >test4.txt");
	system ("echo 'This file\nis\nGREAT!\n' >test5.txt");

	if (! sontIdentiques ("test1.txt", "test2.txt") )
		printf ("'sontIdentiques()' test#1 has failed.\n");
	else if (sontIdentiques ("test1.txt", "test3.txt") )
		printf ("'sontIdentiques()' test#2 has failed.\n");
	else if (sontIdentiques ("test2.txt", "test4.txt") )
		printf ("'sontIdentiques()' test#3 has failed.\n");
	else if (sontIdentiques ("test3.txt", "test5.txt") )
		printf ("'sontIdentiques()' test#4 has failed.\n");
	else
		printf ("'sontIdentiques()' test has succeeded.\n");

	system ("rm -f test1.txt test2.txt test3.txt test4.txt test5.txt");
}

int main (int argc, char ** argv) {
	verifAffichage ();
	verifCopie ();
	verifDeplacement ();
	verifSontIdentiques ();

	return 0;
}
