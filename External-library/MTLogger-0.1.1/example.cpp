/*
**	Ce fichier permet d'avoir un petit aperçut sur l'utilisation de MTLogger.
**	Le multi-threading est géré automatiquement.
*/

#include "mtlogger.hpp"

void fun()
{
	mt::tlog(mt::tmttrace, "Je suis dans un autre thread\n"); // Il déctecte automatiquement le thread actuel.
	mt::tlog(mt::tmterror, "Je suis une erreur\n");
}

int main()
{
	mt::tlog(mt::tmttrace, "Je suis un test\n"); // Thread principal
	mt::tlog(mt::tmttrace, "Je suis un autre test\n");

	std::thread other_thread(fun); // On appelle fun() dans un autre thread.
	other_thread.join();

	mt::log(mt::mttrace, "Général"); // Ce texte sera écrit dans un fichier commun. Pas de gestion de thread.

	std::getchar();
	return 0;
}