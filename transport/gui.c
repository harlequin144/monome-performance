
#include <gtk/gtk.h>



void start_x_gui()
{
	GtkWidget *window;
	GtkWidget *textbox;
	GtkWidget *table;
	int i = 0;
	gtk_init (&i, NULL);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
				
	gtk_window_set_title (GTK_WINDOW (window), "Window");
				
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	 /* Create a 1x2 table */
	   table = gtk_table_new (1, 2, TRUE);
		    gtk_container_add (GTK_CONTAINER (window), table);


	textbox = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (textbox),0);
	gtk_table_attach_defaults (GTK_TABLE (table), textbox, 0, 1, 0, 1);


				
	gtk_widget_show (window);
				
	gtk_main ();
				
}
