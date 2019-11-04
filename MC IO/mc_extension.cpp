#ifndef MC_EXTENSION_C
#define MC_EXTENSION_C

#include "mc_utils.hpp"
#include <string>
#include <logger.hpp>

#endif // MC_EXTENSION_C

/*
** Cette fonction compare deux chaînes de caractères sans prendre en compte la casse.
** Exemple:
** "Bonjour" et "bonjour" sont identiques.
*/
int strcicmp(char const *a, char const *b)
{
	for (;; a++, b++) {
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
		if (d != 0 || !*a)
			return d;
	}
}

/*
** Cette fonction vérifie si un nom de fichier contient une extension.
*/
bool mc::utils::haveExtension(const char *fileName)
{
	int len = strlen(fileName);

	for (int i = 0; i <= len || fileName[i] != '\0'; ++i)
	{
		if (fileName[i] == '.') return true;
	}

	return false;
}


/*
** Cette fonction compare une extension avec l'extension d'un fichier.
** Exemple:
** extensionCompare("mon-fichier.txt", "txt") retourne 'true', car l'extension de
** 'mon-fichier.txt' est bien 'txt'.
*/
bool mc::utils::extensionCompare(const char *fileName, char *extension)
{
	int len = std::strlen(fileName);

	if (haveExtension(fileName) == false)
	{
		logger::mcLog(logger::errorStream, "%s doesn't have extension.\n");
		return false;
	}

	int size = 1;
	for (int i = len; i > 0; --i, ++size)
	{
		if (fileName[i] == '.')
			break;
	}

	size -= 2;
	char *buff = new char[size + 1];
	for (int i = len - size, idx = 0; idx <= size; ++i)
	{
		buff[idx++] = fileName[i];
	}


	if (strcmp(_strlwr(buff), _strlwr(extension)) == 0)
		return true;

	return false;
}