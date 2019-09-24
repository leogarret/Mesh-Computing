#ifndef _OPEN_FILE_C
#define _OPEN_FILE_C

#include "ui.h"

#endif // !_OPEN_FILE_C

//void ui::callback_open_file_dialog()
//{
//	Mesh mesh;
//	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;
//
//	/*-------- Create Open File Dialog ----------*/
//	GtkWidget *dialog = gtk_file_chooser_dialog_new("Ouvrir un fichier", NULL,
//													GTK_FILE_CHOOSER_ACTION_OPEN,
//													GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
//													GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
//													NULL);
//
//	if (dialog == NULL)
//	{
//		OutputDebugString("Impossible de creer le dialogue d'ouverture de fichier.");
//		exit(-1);
//	}
//
//	/*-----------------------------------------*/
//
//	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
//	{
//		gchar *fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
//
//		if (loader(mesh, fileName, mesh_info) == -1)
//		{
//			OutputDebugString("Impossible de charger le mesh [");OutputDebugString(fileName);OutputDebugString("]\n");
//			gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(data.builder, "meshInfoLbl")), "Please, select a valid obj file.");
//		}
//		else
//		{
//			// Mise a jour du contenu du label MeshInfoLbl
//			ui::MeshDisplayInfo(GTK_LABEL(gtk_builder_get_object(data.builder, "meshInfoLbl")), mesh, mesh_info);
//		}
//	}
//
//	gtk_widget_destroy(dialog);
//}