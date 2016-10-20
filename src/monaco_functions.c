//-------------------------------------------------------------------//
// Monaco opening and manipulating functions 
//-------------------------------------------------------------------//


//-------------------------------------------------------------------//
// function sets the following global values:
// n_of_x, n_of_y, monaco_step, n_of_lines, lines_splitted, monaco_row_val, monaco_column_val
// We assume that the plane is symmetric (0,0 is in the middle)
// so the size of plane will be from -((n_of_x-1)*0.5) to +((n_of_x-1)*0.5)
//-------------------------------------------------------------------//
void open_monaco_plane_clicked(  )
{
  GtkWidget *_dialog;
  gchar *_filename = NULL;
  GString *_uri = NULL;
  gchar *_contents = NULL;
  gsize *_length = NULL;
  GString *_temp_text = NULL;

  GFile *_opening_file = NULL;
  GFileInputStream *_file_input = NULL;

  gchar **_temp_line_splitted = NULL;

  _dialog = gtk_file_chooser_dialog_new ("Choose file to open",
				      GTK_WINDOW(main_window),
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      "_Cancel", GTK_RESPONSE_CANCEL,
				      "_Open", GTK_RESPONSE_ACCEPT,
				      NULL);
  gtk_file_chooser_set_action ( GTK_FILE_CHOOSER(_dialog), GTK_FILE_CHOOSER_ACTION_OPEN);
  _uri = g_string_new( g_get_current_dir() );
  g_string_append_printf( _uri, "/data" );
  gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(_dialog), _uri->str );
  g_string_free( _uri, TRUE );

  if (gtk_dialog_run (GTK_DIALOG (_dialog)) == GTK_RESPONSE_ACCEPT)
  {
     _filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (_dialog));
     g_file_get_contents( (const gchar*) _filename, &_contents, _length, NULL );
   
    lines_splitted = g_strsplit ( _contents, "\n", G_MAXINT );

    if( ! g_str_has_prefix ( lines_splitted[0], "0001108e" ) )
    {
      msg("Wrong file loaded\nShould begin with \"0001108e\"");
      g_strfreev( lines_splitted );
      gtk_widget_destroy( _dialog );
      return;
    }

    n_of_lines = g_strv_length( lines_splitted );

    _temp_line_splitted = g_strsplit ( lines_splitted[14], ",", G_MAXINT );

    n_of_x = g_ascii_strtoull ( _temp_line_splitted[1], NULL, 10);
    n_of_y = g_ascii_strtoull ( _temp_line_splitted[2], NULL, 10);
    g_strfreev( _temp_line_splitted );
   
    _temp_line_splitted = g_strsplit ( lines_splitted[15], ",", G_MAXINT );
    monaco_step = g_ascii_strtod ( _temp_line_splitted[1], NULL );
    g_strfreev( _temp_line_splitted );

    monaco_column_val = ((n_of_x-1)/2)+1;
    monaco_row_val = ((n_of_y-1)/2)+1;

    //we assume that if monaco_column_var & monaco_row_val are different the plane is sagital
    if( monaco_column_val == monaco_row_val ) gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(monaco_rb_1), TRUE );
    else gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(monaco_rb_2), TRUE );

    _temp_text = g_string_new ("");

    g_string_printf ( _temp_text, "Step: %.3f", monaco_step );
    gtk_label_set_text( GTK_LABEL(monaco_la_5), _temp_text->str );

    g_string_printf ( _temp_text, "%" G_GUINT64_FORMAT, monaco_row_val, NULL);
    gtk_entry_set_text( GTK_ENTRY(monaco_ed_1), _temp_text->str );
    g_string_printf ( _temp_text, "%" G_GUINT64_FORMAT, monaco_column_val, NULL);
    gtk_entry_set_text( GTK_ENTRY(monaco_ed_2), _temp_text->str );

    g_array_set_size( monaco_graph.g, 0 );
    gtk_widget_queue_draw ( monaco_da );

    g_string_free( _temp_text, TRUE );
    g_free( _filename );
    g_free( _contents );
    g_free( _length );
    gtk_widget_destroy( _dialog );
  }
  else
  {
    gtk_widget_destroy( _dialog );
    return;
  }
}
// end of open_monaco_plane_clicked


//-------------------------------------------------------------------//
// Exporting row as csv 
//-------------------------------------------------------------------//
void save_monaco_row_clicked(  )
{
  GString *_temp_text = NULL;
  GtkWidget *_dialog;
  gchar *_filename = NULL;
  GString *_uri = NULL;
  GtkFileChooser *_chooser;
  GtkFileChooserAction _action = GTK_FILE_CHOOSER_ACTION_SAVE;

  guint64 _i;

  _temp_text = g_string_new ("");

  get_monaco_row_clicked();

  for( _i=0; _i < monaco_graph.g->len; _i++ )
  {
    g_string_append_printf ( _temp_text, "%" G_GUINT64_FORMAT ", %5.5f\n", _i+1, (g_array_index(monaco_graph.g,point,_i)).dose, NULL);
  }


  _dialog = gtk_file_chooser_dialog_new ("Choose file to open",
				      GTK_WINDOW(main_window),
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      "_Cancel", GTK_RESPONSE_CANCEL,
				      "_Save", GTK_RESPONSE_ACCEPT,
				      NULL);

  gtk_file_chooser_set_action ( GTK_FILE_CHOOSER(_dialog), GTK_FILE_CHOOSER_ACTION_SAVE);
  _chooser = GTK_FILE_CHOOSER (_dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (_chooser, TRUE);
  gtk_file_chooser_set_current_name (_chooser, "Exported Row.csv");
  _uri = g_string_new( g_get_current_dir() );
  g_string_append_printf( _uri, "/data" );
  gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(_dialog), _uri->str );
  g_string_free( _uri, TRUE );


  if (gtk_dialog_run (GTK_DIALOG (_dialog)) == GTK_RESPONSE_ACCEPT)
    {
      _filename = (gchar*)gtk_file_chooser_get_filename (_chooser);
      g_file_set_contents (_filename, _temp_text->str, -1, NULL );
    }

  gtk_widget_destroy (_dialog);

  if(_filename)g_free(_filename);
}


//-------------------------------------------------------------------//
// Function retrieves row given in GtkEntry and creates monaco_garray 
//-------------------------------------------------------------------//
void get_monaco_row_clicked(  ) // get row given as y
{
  // lines: from 0 to 16 (17 lines) they are some bla bla texts
  // line 17 = lines_splitted[16] is the first row
  // we need to write monaco_row_val'th row
  // so this will be monaco_row_val + 16 line (counting from 1)
  // we call it by monaco_row_val + 15 !!! (counting from 0)
  GString *_temp_text = NULL;
  guint64 _n_of_pts, _i;
  gchar **_line_needed_splitted = NULL;

  gdouble _dose;
  gdouble _x;

  point _point;

  _temp_text = g_string_new ("");

  if(lines_splitted == NULL)
  {
    msg( "File not loaded" );
    return;
  }

  if( ! is_it_number(gtk_entry_get_text(GTK_ENTRY(monaco_ed_1))) ){msg("Not a number"); return;}

  monaco_row_val = g_ascii_strtoull( gtk_entry_get_text(GTK_ENTRY(monaco_ed_1)), NULL, 0 );

  if( (monaco_row_val > n_of_y) || (monaco_row_val == 0))
  {
    msg( "Wrong row number" );
    return;
  }
  _line_needed_splitted = g_strsplit ( lines_splitted[monaco_row_val+15], ",", G_MAXINT );
  _n_of_pts = g_strv_length( _line_needed_splitted );

  g_array_set_size ( monaco_graph.g, 0 );


  _x = -((n_of_x - 1)*0.05); // beggining of x axis in cm (not mm)
  for( _i=0; _i<_n_of_pts; _i++ )
  {
    _dose = g_ascii_strtod ( _line_needed_splitted[_i], NULL );
    _point.x = _x + monaco_step*0.1 * _i; //in cm
    _point.dose = _dose;
    g_array_append_val( monaco_graph.g, _point );
  }

  if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_1)) )monaco_graph.type = GT_CROSSLINE;//row is cross
  else if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_2)) )monaco_graph.type = GT_DEPTH;
  else if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_3)) )monaco_graph.type = GT_DEPTH;

  normalize_graph( monaco_graph, NORM_TO_CENTER );

  g_strfreev( _line_needed_splitted );

  g_string_append_printf ( _temp_text, "Currently loaded: %" G_GUINT64_FORMAT ". row", monaco_row_val );
  gtk_label_set_text( GTK_LABEL(monaco_la_3), _temp_text->str );
  g_string_printf ( _temp_text, "N of points: %" G_GUINT64_FORMAT, _n_of_pts );
  gtk_label_set_text( GTK_LABEL(monaco_la_4), _temp_text->str );
  g_string_printf ( _temp_text, "Step: %.3f", get_step_of_garray(monaco_graph.g) );
  gtk_label_set_text( GTK_LABEL(monaco_la_5), _temp_text->str );

  g_string_free( _temp_text, TRUE );

  gtk_widget_queue_draw ( monaco_da );

}
// end of get_monaco_row_clicked

//-------------------------------------------------------------------//
// Exporting column as csv 
//-------------------------------------------------------------------//
void save_monaco_column_clicked(  )
{
  GString *_temp_text = NULL;
  GtkWidget *_dialog;
  gchar *_filename = NULL;
  GString *_uri = NULL;
  GtkFileChooser *_chooser;
  GtkFileChooserAction _action = GTK_FILE_CHOOSER_ACTION_SAVE;

  guint64 _i;

  _temp_text = g_string_new ("");

  get_monaco_column_clicked();

  for( _i=0; _i < monaco_graph.g->len; _i++ )
  {
    g_string_append_printf ( _temp_text, "%" G_GUINT64_FORMAT ", %.1f\n", _i+1, (g_array_index(monaco_graph.g,point,_i)).dose, NULL);
  }

  _dialog = gtk_file_chooser_dialog_new ("Choose file to open",
				      GTK_WINDOW(main_window),
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      "_Cancel", GTK_RESPONSE_CANCEL,
				      "_Save", GTK_RESPONSE_ACCEPT,
				      NULL);

  gtk_file_chooser_set_action ( GTK_FILE_CHOOSER(_dialog), GTK_FILE_CHOOSER_ACTION_SAVE);
  _chooser = GTK_FILE_CHOOSER (_dialog);

  gtk_file_chooser_set_do_overwrite_confirmation (_chooser, TRUE);
  gtk_file_chooser_set_current_name (_chooser, "Exported Column.csv");
  _uri = g_string_new( g_get_current_dir() );
  g_string_append_printf( _uri, "/data" );
  gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER(_dialog), _uri->str );
  g_string_free( _uri, TRUE );


  if (gtk_dialog_run (GTK_DIALOG (_dialog)) == GTK_RESPONSE_ACCEPT)
    {
      _filename = (gchar*)gtk_file_chooser_get_filename (_chooser);
      g_file_set_contents (_filename, _temp_text->str, -1, NULL );
    }

  gtk_widget_destroy (_dialog);

  if(_filename)g_free(_filename);
  g_string_free( _temp_text, TRUE );

}


//-------------------------------------------------------------------//
// Function retrieves column given in GtkEntry as x
// and creates monaco_garray from it
//-------------------------------------------------------------------//
void get_monaco_column_clicked()
{
  // lines: from 0 to 16 (17 lines) they are some bla bla texts
  // line 17 = lines_splitted[16] is the first row
  // we need to write monaco_column_val'th column
  // we need to split every row from 17 [16] and take monaco_column_val'th value from this row
  // then put it in the array
  GString *_temp_text = NULL;
  guint64 _n_of_rows = n_of_lines-17; //there is 17 "bla bla" lines but we need the first row was the 0th
  guint64 _i, _n_of_columns;
  gchar **_line_needed_splitted = NULL;

  gdouble _x;
  gdouble _dose;

  point _point;

  _temp_text = g_string_new ("");

  if(lines_splitted == NULL){msg("File not loaded");return;}

  if( ! is_it_number(gtk_entry_get_text(GTK_ENTRY(monaco_ed_2))) ){msg("Not a number"); return;}

  monaco_column_val = g_ascii_strtoull( gtk_entry_get_text(GTK_ENTRY(monaco_ed_2)), NULL, 0 );

  if( (monaco_column_val > n_of_x) || (monaco_column_val == 0))
  {
    msg( "Wrong column number" );
    return;
  }

  g_array_set_size ( monaco_graph.g, 0 );

  if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(monaco_rb_1))) //we have xy column
  {
    _x = -((n_of_y - 1)*0.05); // beggining ox y axis in cm (not mm)
  }
  else //we are in some depth plane - column is depth dose
  {
    _x = -10;
  }
  for( _i=0; _i<_n_of_rows; _i++ )
  {
      _line_needed_splitted = g_strsplit ( lines_splitted[_i+16], ",", G_MAXINT );
      _n_of_columns = g_strv_length( _line_needed_splitted );
      _dose = g_ascii_strtod ( _line_needed_splitted[monaco_column_val-1], NULL );
      _point.x = _x + monaco_step*0.1 * _i;//in cm
      _point.dose = _dose;
      g_array_append_val( monaco_graph.g, _point );

      g_strfreev( _line_needed_splitted );
      _line_needed_splitted = NULL;
  }

  if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_1)) )monaco_graph.type = GT_CROSSLINE;//row is cross
  else if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_2)) )monaco_graph.type = GT_DEPTH;
  else if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(monaco_rb_3)) )monaco_graph.type = GT_DEPTH;

  if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(monaco_rb_1))) //we have xy column
  {
    normalize_graph( monaco_graph, NORM_TO_CENTER );
  }
  else //we are in some depth plane - column is depth dose
  {
    normalize_graph( monaco_graph, NORM_TO_MAX );
  }

  g_string_printf ( _temp_text, "Currently loaded: %" G_GUINT64_FORMAT ". column", monaco_column_val );
  gtk_label_set_text( GTK_LABEL(monaco_la_3), _temp_text->str );
  g_string_printf ( _temp_text, "N of points: %d", monaco_graph.g->len );
  gtk_label_set_text( GTK_LABEL(monaco_la_4), _temp_text->str );
  g_string_printf ( _temp_text, "Step: %.3f", get_step_of_garray(monaco_graph.g) );
  gtk_label_set_text( GTK_LABEL(monaco_la_5), _temp_text->str );
  g_string_free( _temp_text, TRUE );

  gtk_widget_queue_draw ( monaco_da );
}
// end of get_monaco_column_clicked


