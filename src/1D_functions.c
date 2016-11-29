void open_omnipro_accept_clicked(  )
{
  GtkWidget *_dialog;
  gchar *_filename = NULL;
  GString *_uri = NULL;
  gchar *_contents = NULL;
  gsize *_length = NULL;
  GString *_temp_text = NULL;
  guint64 _n_of_rows;

  GFile *_opening_file = NULL;
  GFileInputStream *_file_input = NULL;

  guint64 _i, _n_of_columns;
  gchar **_line_needed_splitted = NULL;
  gdouble _var;

  guint _temp_first_row;
  omnipro_dataset _prev_omnipro_dataset;

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
   
    csv_splitted = g_strsplit ( _contents, "\n", G_MAXINT );
    if( ! g_str_has_prefix ( csv_splitted[0], "ScanColor,ScanType,RadiationType,Energy,Medium" ) )
    {
      msg("Wrong file loaded\nShould begin with \"ScanColor,ScanType,RadiationType,Energy,Medium\"");
      g_strfreev( csv_splitted );
      gtk_widget_destroy( _dialog );
      return;
    }

    n_of_csv_lines = g_strv_length( csv_splitted );
    n_of_csv_sets = 0;
    gtk_combo_box_text_remove_all( GTK_COMBO_BOX_TEXT(omnipro_combo_box_text) );
    _temp_text = g_string_new ("");

    gtk_widget_show( omnipro_la_1 );
    gtk_widget_show( omnipro_combo_box_text );
    gtk_widget_show( omnipro_button );
    gtk_widget_show( omnipro_la_2 );
    gtk_widget_show( omnipro_la_3 );
    gtk_widget_show( omnipro_la_4 );
    reset_graph_calculations( &omnipro_graph );
    g_array_set_size( omnipro_graph.g, 0 );
    are_calculations_current = FALSE;

    g_array_set_size ( omnipro_sets_garray, 0 ); //now we need to identify sets
    gtk_widget_queue_draw ( omnipro_da );

    for( _i=0; _i<n_of_csv_lines; _i++ )
    {
      if( g_str_has_prefix ( csv_splitted[_i], ",Crossline(cm),Inline(cm),Depth(cm),Dose(%)" ) )
      {
        if( n_of_csv_sets > 0 ) //we have second , 3rd ... set 
        {
          _prev_omnipro_dataset.first_row = _temp_first_row;
          _prev_omnipro_dataset.last_row = _i-1;
          g_array_append_val ( omnipro_sets_garray, _prev_omnipro_dataset );
        }
        _temp_first_row = _i+2; //always for 1st 2nd 3rd ...
        n_of_csv_sets++;

        g_string_printf ( _temp_text, "%d. %s", n_of_csv_sets, csv_splitted[_i-1], NULL );
        gtk_combo_box_text_insert_text( GTK_COMBO_BOX_TEXT(omnipro_combo_box_text), -1, _temp_text->str );
      }

    }
    // we reached the end of a file and the last set must be added
    _prev_omnipro_dataset.first_row = _temp_first_row;
    _prev_omnipro_dataset.last_row = _i-1;
    g_array_append_val ( omnipro_sets_garray, _prev_omnipro_dataset );

    _n_of_rows = n_of_csv_lines - 1 - 2*n_of_csv_sets;

    for ( _i=0; _i<n_of_csv_sets; _i++ )
    {
      _prev_omnipro_dataset = g_array_index ( omnipro_sets_garray, omnipro_dataset, _i );
    }

    g_string_printf ( _temp_text, "N of pts: %" G_GUINT64_FORMAT"; N of sets: %u;", _n_of_rows, n_of_csv_sets, NULL);
    gtk_label_set_text( GTK_LABEL(omnipro_la_1), _temp_text->str );

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
// end of open_omnipro_accept_clicked


//-------------------------------------------------------------------//
// this function loads 1D data from csv file
// the file must be: x, dose_value formatted 
// each line is read, distance between x should be 0.1 cm
// the file must be symmetric
//-------------------------------------------------------------------//
void open_1d_from_csv_clicked(  )
{
  GtkWidget*_dialog;
  gchar*_filename = NULL;
  GString*_uri = NULL;
  gchar*_contents = NULL;
  gsize*_length = NULL;
  guint _n_of_rows;
  gchar**_splitted_file;
  gchar**_splitted_row;

  GFile *_opening_file = NULL;
  GFileInputStream *_file_input = NULL;

  guint _i;
  point _tp;

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
   
    _splitted_file = g_strsplit ( _contents, "\n", G_MAXINT );
    _n_of_rows = g_strv_length( _splitted_file );
    reset_graph_calculations( &omnipro_graph );
    g_array_set_size( omnipro_graph.g, 0 );
    omnipro_graph.type = GT_CROSSLINE;
    for( _i=0;_i<_n_of_rows-1;_i++ )
    {
      _splitted_row = g_strsplit ( _splitted_file[_i], ",", G_MAXINT );
      if( g_strv_length( _splitted_row ) < 1 )
      { 
        g_strfreev( _splitted_row );
        g_strfreev( _splitted_file );
        msg("Something is wrong with the length of some row...");
        g_free( _filename );
        g_free( _contents );
        g_free( _length );
        gtk_widget_destroy( _dialog );
        g_array_set_size( omnipro_graph.g, 0 );
        return;
      }
      _tp.x = g_ascii_strtod ( _splitted_row[0], NULL );
      _tp.dose = g_ascii_strtod ( _splitted_row[1], NULL );
      _tp.result = -1;
      _tp.desc = NULL;
      g_strfreev( _splitted_row );
      g_array_append_val( omnipro_graph.g, _tp );
    }
    normalize_graph( omnipro_graph, NORM_TO_CENTER );
    calculate_fwhm( &omnipro_graph );
    are_calculations_current = FALSE;
    gtk_widget_queue_draw ( omnipro_da );
    gtk_widget_hide( omnipro_la_1 );
    gtk_widget_hide( omnipro_combo_box_text );
    gtk_widget_hide( omnipro_button );
    gtk_widget_hide( omnipro_la_2 );
    gtk_widget_hide( omnipro_la_3 );
    gtk_widget_hide( omnipro_la_4 );

    g_strfreev( _splitted_file );
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
// end of open_1d_from_csv_clicked

//-------------------------------------------------------------------//
// Function operates on csv_splitted array, the garray should be created first
// function creates omnipro_garray filled with point structures.
//-------------------------------------------------------------------//
void get_omnipro_dataset_clicked()
{
  GString *_temp_text = NULL;
  guint _n_of_rows;

  guint _i, _n_of_columns, _n_series;
  gchar **_line_splitted = NULL;
  gdouble _var;

  guint _temp_first_row;
  omnipro_dataset _set;
  point _temp_point;

  _temp_text = g_string_new("");
  g_string_append( _temp_text, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(omnipro_combo_box_text)) );
  
  if( _temp_text->len > 3 )
  {
    _line_splitted = g_strsplit( _temp_text->str, ".", -1 );
    _n_series = g_ascii_strtoull( _line_splitted[0], NULL, 10 );
    if( g_strrstr( _temp_text->str, "Crossline" ) != NULL )omnipro_graph.type = GT_CROSSLINE;
    if( g_strrstr( _temp_text->str, "Inline" ) != NULL )omnipro_graph.type = GT_INLINE;
    if( g_strrstr( _temp_text->str, "Depth Dose" ) != NULL )omnipro_graph.type = GT_DEPTH;
    g_strfreev( _line_splitted );
    // now is time to get appropriate data
    if( omnipro_graph.type == GT_UNDEFINED)
    {
      msg("Something is wrong with the type of dataset.\nIt is neither inline, crossline or depth dose.");
      g_string_free( _temp_text, TRUE );
      return;
    }    
    _set = g_array_index( omnipro_sets_garray, omnipro_dataset, _n_series-1 );
    g_array_set_size ( omnipro_graph.g, 0 ); 

    for( _i=_set.first_row; _i<=_set.last_row; _i++ )
    {
      _line_splitted = g_strsplit( csv_splitted[_i-1], ",", -1 );
      _temp_point.x = (gdouble)round1( g_ascii_strtod ( _line_splitted[omnipro_graph.type], NULL ) *10.0) *0.1;
        //round*10/10 rounds one digit after comma
      _temp_point.dose = g_ascii_strtod ( _line_splitted[4], NULL );
      _temp_point.result = -1; // not checked yet
      _temp_point.desc = NULL;
      g_array_append_val( omnipro_graph.g, _temp_point );
    }
    g_string_printf ( _temp_text, "Currently loaded set: %d" ,_n_series );
    gtk_label_set_text( GTK_LABEL(omnipro_la_2), _temp_text->str );
    g_string_printf ( _temp_text, "N of points in set: %d", omnipro_graph.g->len );
    gtk_label_set_text( GTK_LABEL(omnipro_la_3), _temp_text->str );
    g_string_printf ( _temp_text, "Step: %.2f", get_step_of_garray(omnipro_graph.g) );
    gtk_label_set_text( GTK_LABEL(omnipro_la_4), _temp_text->str );
  }

  if( (g_array_index( omnipro_graph.g, point, 1 ).x - g_array_index( omnipro_graph.g, point, 0 ).x ) <= 0 )
         invert_graph( &omnipro_graph );

  if((omnipro_graph.type==GT_CROSSLINE) || (omnipro_graph.type == GT_INLINE)) //we have xy column
  {
    normalize_graph( omnipro_graph, NORM_TO_CENTER );
  }
  else if( (omnipro_graph.type==GT_DEPTH) ) //we are in some depth plane - column is depth dose
  {
    normalize_graph( omnipro_graph, NORM_TO_MAX );
  }

  g_string_free( _temp_text, TRUE );

  calculate_fwhm( &omnipro_graph );
  gtk_widget_queue_draw ( omnipro_da );
  gtk_widget_queue_draw ( compare_da );
  are_calculations_current = FALSE;
}
//end of get_omnipro_dataset_clicked



