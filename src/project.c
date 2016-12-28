#include "project.h"

#include "2D_functions.c"
#include "1D_functions.c"
#include "helper_functions.c"


/*
//-------------------------------------------------------------------//
// the garray is trimmed from both sides from min to max 'x' values
//-------------------------------------------------------------------//
void trim_garray( GArray* _g, gdouble _min, gdouble _max)
{
  if(_g==NULL)return;
  if(_g->len==0)return;
  guint _i;
  for(_i=0;_i<_g->len;_i++)
  {
    if( g_array_index( _g, point, _i ).x < _min || g_array_index( _g, point, _i ).x > _max )
                 g_array_remove_index( _g, _i );
  }
}
*/
//-------------------------------------------------------------------//
// Function draws dots on given cairo_t according to values in graph 
//-------------------------------------------------------------------//
void draw_dots( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y )
{
  if( !_g )return;
  if( !_g->g )return;
  if ( _g->g->len == 0 )return;

  GdkRGBA _c1; // ok dots
  GdkRGBA _c2; // not ok dots
  guint _i;
  gdk_rgba_parse( &_c1, "navy" );
  gdk_rgba_parse( &_c2, "red" );

  cairo_set_line_width( _cr, 2 );
  for(_i=0; _i<_g->g->len; _i++ )
  {
    if( g_array_index( _g->g, point, _i ).result == 0 )
                                  gdk_cairo_set_source_rgba (_cr, &_c1);//good
    else if( g_array_index( _g->g, point, _i ).result == 1 ) gdk_cairo_set_source_rgba (_cr, &_c2);//bad
    if( g_array_index( _g->g, point, _i ).result != -1 )
    {
      if( _g->type == GT_CROSSLINE || _g->type == GT_INLINE )
           cairo_arc( _cr, 0.5*_w + g_array_index( _g->g, point, _i ).x * _w*0.8*0.5 / (15.0) + _x,
                      _h*0.9 - g_array_index( _g->g, point, _i ).dose*0.01 * 0.8 * _h + _y, 1., 0., 2 * G_PI);
      else cairo_arc( _cr, 0.1*_w + g_array_index( _g->g, point, _i ).x * _w*0.8*0.5 / (15.0) + _x,
                      _h*0.9 - g_array_index( _g->g, point, _i ).dose*0.01 * 0.8 * _h + _y, 1., 0., 2 * G_PI);
      cairo_stroke ( _cr );

    }
  }

}

//-------------------------------------------------------------------//
// Function draws on given cairo_t given graph 
//-------------------------------------------------------------------//
void draw_graph( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y )
{
  if( !_g )return;
  if( !_g->g )return;
  if ( _g->g->len == 0 )return;
  guint _i;


  // move to first point              // 15 should be on 0.8*_width*0.5
  cairo_set_line_width( _cr, 1 );
  if( _g->type == GT_CROSSLINE || _g->type == GT_INLINE )
       cairo_move_to ( _cr, 0.5*_w + g_array_index( _g->g, point, 0 ).x * _w *0.8*0.5 / (15.0) + _x,
                           _h*0.9 - g_array_index( _g->g, point, 0 ).dose *0.01 * 0.8 * _h + _y );
  else cairo_move_to ( _cr, 0.1*_w + g_array_index( _g->g, point, 0 ).x * _w *0.8*0.5 / (15.0) + _x,
                           _h*0.9 - g_array_index( _g->g, point, 0 ).dose *0.01 * 0.8 * _h + _y );

  for(_i=1; _i<_g->g->len; _i++ )
  {
    if( _g->type == GT_CROSSLINE || _g->type == GT_INLINE )
         cairo_line_to( _cr, 0.5*_w + g_array_index( _g->g, point, _i ).x * _w*0.8*0.5 / (15.0) + _x,
                            _h*0.9 - g_array_index( _g->g, point, _i ).dose *0.01 * 0.8 * _h + _y );
    else cairo_line_to( _cr, 0.1*_w + g_array_index( _g->g, point, _i ).x * _w*0.8*0.5 / (15.0) + _x,
                            _h*0.9 - g_array_index( _g->g, point, _i ).dose *0.01 * 0.8 * _h + _y );
  }

  gdk_cairo_set_source_rgba (_cr, &global_options.color_1 );
  cairo_stroke ( _cr );
}

//-------------------------------------------------------------------//
// Function draws fwhm of given graph on cairo_t 
// if _x, and_y are 0 it will display info in upper right corner
// _x, _y are to adjust it,
// _txt will be printed before FWHM value 
//-------------------------------------------------------------------//
void draw_fwhm_txt ( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y, char*_txt )
{
  if( _g == NULL )return;
  if( _g->type != GT_CROSSLINE && _g->type != GT_INLINE )return;
  GString *_temp_text = NULL;
  _temp_text = g_string_new ("");
  calculate_width( _g, 50 );
  GdkRGBA _c2;
  gdk_rgba_parse( &_c2, "red" );

  gdk_cairo_set_source_rgba (_cr, &_c2 );
  cairo_move_to ( _cr, _w*0.62 + _x, _h *0.1 + _y );
  g_string_printf( _temp_text, "%s %.2fcm", _txt, _g->second_50-_g->first_50, NULL );
  cairo_show_text ( _cr, _temp_text->str );
  cairo_stroke ( _cr );
  g_string_free( _temp_text , TRUE );

}

//-------------------------------------------------------------------//
// Function draws fwhm line of given graph on cairo_t 
//-------------------------------------------------------------------//
void draw_fwhm_line ( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y )
{
  if( _g == NULL )return;
  if( _g->type != GT_CROSSLINE && _g->type != GT_INLINE )return;
  calculate_width( _g, 50 );
  GdkRGBA _c2;
  gdk_rgba_parse( &_c2, "red" );

  gdk_cairo_set_source_rgba (_cr, &_c2 );
  cairo_set_line_width( _cr, 1 );
  cairo_move_to ( _cr, 0.5*_w + _g->first_50 * _w *0.8*0.5 / (15.0) + _x,
                           _h*0.9 - 50 *0.01 * 0.8 * _h + _y );

  cairo_line_to ( _cr, 0.5*_w + _g->second_50 * _w *0.8*0.5 / (15.0) + _x,
                           _h*0.9 - 50 *0.01 * 0.8 * _h + _y );
  cairo_stroke ( _cr );
}


//-------------------------------------------------------------------//
// Function draws on given cairo_t some basics needed to draw graph on
//-------------------------------------------------------------------//
void draw_background( graph*_g, cairo_t *_cr, guint _width, guint _height, guint _x, guint _y )
{
  GdkRGBA _color;
  gdouble _var;
  guint _i;
  GString *_temp_text = NULL;
  _temp_text = g_string_new ("");

  // background
  cairo_rectangle ( _cr, _width * 0.05 + _x, _height * 0.05 + _y, _width * 0.9 + _x, _height * 0.9 + _y );
  gdk_rgba_parse (&_color,"white");
  gdk_cairo_set_source_rgba (_cr, &_color);
  cairo_fill( _cr );

  //grid
  for( _i =0; _i<=12; _i++)
  {
    cairo_move_to ( _cr, _width*0.1 + (0.8*_width*_i)/12 + _x, _height *0.9 + _y ); 
    cairo_line_to ( _cr, _width*0.1 + (0.8*_width*_i)/12 + _x, _height *0.1 + _y );
  }
  for( _i =0; _i<=10; _i++)
  {
    cairo_move_to ( _cr, _width*0.1 + _x, _height *0.1 + (0.8*_height*_i)/10 + _y ); 
    cairo_line_to ( _cr, _width*0.9 + _x, _height *0.1 + (0.8*_height*_i)/10 + _y );
  }
  gdk_rgba_parse (&_color,"grey");
  gdk_cairo_set_source_rgba (_cr, &_color);
  cairo_set_line_width( _cr, 0.5 );
  cairo_stroke ( _cr );
  if( _g == NULL ) return;

  //axis
  if( _g->type == GT_CROSSLINE || _g->type == GT_INLINE )
  {
    cairo_move_to ( _cr, _width * 0.5 + _x, _height *0.9 + _y ); // y
    cairo_line_to ( _cr, _width * 0.5 + _x, _height *0.1 + _y );
  }
  else
  {
    cairo_move_to ( _cr, _width * 0.1 + _x, _height *0.9 + _y ); // y
    cairo_line_to ( _cr, _width * 0.1 + _x, _height *0.1 + _y );
  }

  cairo_move_to ( _cr, _width *0.1 + _x, _height *0.9 + _y ); // x
  cairo_line_to ( _cr, _width *0.9 + _x, _height *0.9 + _y);

  gdk_rgba_parse (&_color,"black");
  cairo_set_line_width( _cr, 0.7 );
  gdk_cairo_set_source_rgba (_cr, &_color);

  cairo_stroke ( _cr );
  
  //labels
  for( _i =0; _i<=6; _i++)
  {
    if( _g->type == GT_CROSSLINE || _g->type == GT_INLINE ) _var = (30*_i)/6.0-15;
    else _var = (30*_i)/6.0;
    g_string_printf( _temp_text, "%d", (gint)_var, NULL );
    cairo_move_to ( _cr, _width*0.1 + (0.8*_width*_i)/6 + _x, _height *0.95 + _y ); 
    cairo_show_text ( _cr,_temp_text->str );
  }
  cairo_move_to ( _cr, _width*0.05 + _x, _height *0.09 + _y ); 
  cairo_show_text ( _cr, "100%" );
  cairo_move_to ( _cr, _width*0.05 + _x, _height *0.49 + _y ); 
  cairo_show_text ( _cr, "50%" );

  gdk_rgba_parse (&_color,"black");
  gdk_cairo_set_source_rgba (_cr, &_color);
  cairo_set_line_width( _cr, 0.5 );
  cairo_stroke ( _cr );
  g_string_free( _temp_text, TRUE );
}

//-------------------------------------------------------------------//
// Callback function for printing
//-------------------------------------------------------------------//
static void begin_print (GtkPrintOperation *operation,
             GtkPrintContext   *context,
             gpointer           user_data)
{
  gtk_print_operation_set_n_pages (operation, 1);
}

//-------------------------------------------------------------------//
// Callback function for printing
//-------------------------------------------------------------------//
static void draw_page (GtkPrintOperation *operation,
           GtkPrintContext   *context,
           gint               page_nr,
           gpointer           user_data)
{
  cairo_t *_cr;
  PangoLayout *layout;
  gint text_width, text_height;
  gdouble _w, _h;
  gint line;
  PangoFontDescription *desc;
  gchar *page_str;

  _cr = gtk_print_context_get_cairo_context (context);
  _w = gtk_print_context_get_width (context);
  _h = gtk_print_context_get_height (context);
  
  cairo_rectangle( _cr, 0, 0, _w, 30 );

  cairo_set_source_rgb (_cr, 0.8, 0.8, 0.8);
  cairo_fill_preserve (_cr);

  cairo_set_source_rgb (_cr, 0, 0, 0);
  cairo_set_line_width (_cr, 1);
  cairo_stroke (_cr);

  graph*_tested_g=NULL;
  graph*_other_g=NULL;

  draw_background( NULL , _cr, _w, _h*0.5, 0, 20 );
  if( are_calculations_current )
  {


    if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(compare_rb_1)) )
    {
      _tested_g = &monaco_graph;
      _other_g  = &omnipro_graph;
    }
    else
    {
      _tested_g = &omnipro_graph;
      _other_g  = &monaco_graph;
    }

    draw_graph( &omnipro_graph, _cr, _w, _h*0.5, 0, 20 );
    draw_graph( &monaco_graph, _cr, _w, _h*0.5, 0, 20 );
    draw_dots( _tested_g, _cr, _w, _h*0.5, 0, 20 );
    draw_fwhm_txt( &omnipro_graph, _cr, _w, _h*0.5, 0, 20, "1D FWHM" );
    draw_fwhm_line( &omnipro_graph, _cr, _w, _h*0.5, 0, 20 );
    draw_fwhm_txt( &monaco_graph, _cr, _w, _h*0.5, 0, 35, "2D FWHM" );
    draw_fwhm_line( &monaco_graph, _cr, _w, _h*0.5, 0, 20 );
  }

  cairo_set_source_rgb (_cr, 0, 0, 0);
  cairo_set_line_width (_cr, 1);

  layout = gtk_print_context_create_pango_layout (context);

  desc = pango_font_description_from_string ("sans 14");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  pango_layout_set_text (layout, "Report", -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  if (text_width > _w)
    {
      pango_layout_set_width (layout, _w);
      pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_START);
      pango_layout_get_pixel_size (layout, &text_width, &text_height);
    }
  cairo_move_to (_cr, (_w - text_width) / 2,  (30 - text_height) / 2);
  pango_cairo_show_layout (_cr, layout);

  page_str = g_strdup_printf ("Checking options: dose difference[%%]: %.3f, dta[mm]: %.3f",
                              global_options.dose_difference, global_options.dta );
  desc = pango_font_description_from_string ("sans 12");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  pango_layout_set_text (layout, page_str, -1);
  g_free (page_str);
  pango_layout_set_width (layout, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  cairo_move_to (_cr, 30, _h *0.5 + 40);
  pango_cairo_show_layout (_cr, layout);

  page_str = g_strdup_printf ("Number of checked points: %d", get_number_of_checked_points_in_graph( _tested_g ) );
  desc = pango_font_description_from_string ("sans 12");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  pango_layout_set_text (layout, page_str, -1);
  g_free (page_str);
  pango_layout_set_width (layout, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  cairo_move_to (_cr, 30, _h *0.5 + 40 + text_height);
  pango_cairo_show_layout (_cr, layout);

  page_str = g_strdup_printf ("Number of points that meets criteria: %d (%.1f%%)", get_number_of_good_points_in_graph( _tested_g ),
              100*((gfloat)get_number_of_good_points_in_graph( _tested_g )/(gfloat)get_number_of_checked_points_in_graph( _tested_g )) );
  desc = pango_font_description_from_string ("sans 12");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  pango_layout_set_text (layout, page_str, -1);
  g_free (page_str);
  pango_layout_set_width (layout, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  cairo_move_to (_cr, 30, _h *0.5 + 40 + 2*text_height );
  pango_cairo_show_layout (_cr, layout);

  g_object_unref (layout);
}

//-------------------------------------------------------------------//
// Callback function for printing
//-------------------------------------------------------------------//
static void end_print (GtkPrintOperation *operation,
           GtkPrintContext   *context,
           gpointer           user_data)
{
}

//-------------------------------------------------------------------//
// Callback function activated after activate signal
// is emmited from menu after choosing 'generate report'
//-------------------------------------------------------------------//
void report_dialog( GtkMenuItem *_mi, gpointer _d )
{
  GtkWidget *_dialog, *_content_area;

  GtkPrintOperation *operation;
  GtkPrintSettings *settings;
  GError *error = NULL;

  operation = gtk_print_operation_new ();

  g_signal_connect (G_OBJECT (operation), "begin-print", G_CALLBACK (begin_print), NULL );
  g_signal_connect (G_OBJECT (operation), "draw-page",   G_CALLBACK (draw_page), NULL );
  g_signal_connect (G_OBJECT (operation), "end-print",   G_CALLBACK (end_print), NULL );

  gtk_print_operation_set_use_full_page (operation, FALSE);
  gtk_print_operation_set_unit (operation, GTK_UNIT_POINTS);
  gtk_print_operation_set_embed_page_setup (operation, TRUE);

  settings = gtk_print_settings_new ();

  gtk_print_settings_set (settings, GTK_PRINT_SETTINGS_OUTPUT_BASENAME, "print");
  gtk_print_operation_set_print_settings (operation, settings);

  gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (main_window), &error);

  g_object_unref (operation);
  g_object_unref (settings);

  if (error)
    {
      GtkWidget *dialog;

      dialog = gtk_message_dialog_new (GTK_WINDOW (main_window),
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_CLOSE,
                                       "%s", error->message);
      g_error_free (error);

      g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);

      gtk_widget_show (dialog);
    }
}

//-------------------------------------------------------------------//
// About dialog
//-------------------------------------------------------------------//
void about_dialog( GtkMenuItem *_mi, gpointer _d )
{
//  gchar *authors[] = { "Arkadiusz Kowalski and others", NULL };
  gtk_show_about_dialog ( GTK_WINDOW( main_window),
                       "program-name", "dta1d",
                       "license-type", GTK_LICENSE_GPL_2_0,
                       "version", VERSION, 
                       "comments", "Program can be used in radiotherapy departments by physicists to perform some QA procedures. Program compares two dose profiles. Calculated in treatment planning system Monaco from Elekta and measured in water phantom by IBA OmniPro Accept.",
//                       "authors", authors,
                       "website", "https://github.com/oustling/dta1d", NULL );
}

//-------------------------------------------------------------------//
// Function activated after activate signal is emmited from menu 
// after choosing options, creates options window.
//-------------------------------------------------------------------//
void options_dialog( GtkMenuItem *_mi, gpointer _d )
{
  GtkWidget *_dialog, *_content_area;
  GtkDialogFlags _flags;
  GtkWidget *_difference_hbox;
  GtkWidget *_difference_la_1;
  GtkWidget *_difference_ed_1;
  GtkWidget *_dta_hbox;
  GtkWidget *_dta_la_1;
  GtkWidget *_dta_ed_1;
  GtkWidget *_color1_hbox;
  GtkWidget *_color1_la_1;
  GtkWidget *_color1_bu_1;
  GtkWidget *_color2_hbox;
  GtkWidget *_color2_la_1;
  GtkWidget *_color2_bu_1;

  GString *_temp_text = NULL;

  _flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  _dialog = gtk_dialog_new_with_buttons ("Options",
                                       GTK_WINDOW( main_window ),
                                       _flags,
                                       "_SAVE",   GTK_RESPONSE_ACCEPT,
                                       "_RECALCULATE",   GTK_RESPONSE_APPLY,
                                       "_CANCEL", GTK_RESPONSE_REJECT,
                                       NULL);
  _content_area = gtk_dialog_get_content_area (GTK_DIALOG (_dialog));

  load_options();

  _temp_text = g_string_new ("");

  _difference_hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 1 );
    _difference_la_1 = gtk_label_new ("Difference [%]:");
    gtk_box_pack_start( GTK_BOX(_difference_hbox), _difference_la_1, FALSE, FALSE, 1 );
    _difference_ed_1 = gtk_entry_new ();
    g_string_printf ( _temp_text, "%.3f", global_options.dose_difference );
    gtk_entry_set_text ( GTK_ENTRY(_difference_ed_1), _temp_text->str);
    gtk_box_pack_start( GTK_BOX(_difference_hbox), _difference_ed_1, FALSE, FALSE, 1 );
  gtk_box_pack_start( GTK_BOX(_content_area), _difference_hbox, FALSE, FALSE, 1 );

  _dta_hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 1 );
    _dta_la_1 = gtk_label_new ("Dta [mm]:");
    gtk_box_pack_start( GTK_BOX(_dta_hbox), _dta_la_1, FALSE, FALSE, 1 );
    _dta_ed_1 = gtk_entry_new ();
    g_string_printf ( _temp_text, "%.3f", global_options.dta );
    gtk_entry_set_text ( GTK_ENTRY(_dta_ed_1), _temp_text->str);
    gtk_box_pack_start( GTK_BOX(_dta_hbox), _dta_ed_1, FALSE, FALSE, 1 );
  gtk_box_pack_start( GTK_BOX(_content_area), _dta_hbox, FALSE, FALSE, 1 );

  _color1_hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 1 );
    _color1_la_1 = gtk_label_new ("Color 1:");
    gtk_box_pack_start( GTK_BOX(_color1_hbox), _color1_la_1, FALSE, FALSE, 1 );
    _color1_bu_1 = gtk_color_button_new_with_rgba ( &global_options.color_1);
    gtk_box_pack_start( GTK_BOX(_color1_hbox), _color1_bu_1, FALSE, FALSE, 1 );
  gtk_box_pack_start( GTK_BOX(_content_area), _color1_hbox, FALSE, FALSE, 1 );

  _color2_hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 1 );
    _color2_la_1 = gtk_label_new ("Color 2:");
    gtk_box_pack_start( GTK_BOX(_color2_hbox), _color2_la_1, FALSE, FALSE, 1 );
    _color2_bu_1 = gtk_color_button_new_with_rgba ( &global_options.color_2);
    gtk_box_pack_start( GTK_BOX(_color2_hbox), _color2_bu_1, FALSE, FALSE, 1 );
  gtk_box_pack_start( GTK_BOX(_content_area), _color2_hbox, FALSE, FALSE, 1 );

  g_string_free( _temp_text, TRUE );

  g_signal_connect ( _dialog,                      "response", G_CALLBACK (dialog_response), NULL );
  g_signal_connect ( GTK_WIDGET(_difference_ed_1), "focus-out-event", G_CALLBACK (difference_changed), NULL );
  g_signal_connect ( GTK_WIDGET(_dta_ed_1),        "focus-out-event", G_CALLBACK (dta_changed), NULL );
  g_signal_connect ( GTK_WIDGET(_color1_bu_1),     "color-set", G_CALLBACK (color1_changed), NULL );
  g_signal_connect ( GTK_WIDGET(_color2_bu_1),     "color-set", G_CALLBACK (color2_changed), NULL );

  gtk_widget_show_all( _dialog );
}

//-------------------------------------------------------------------//
// Callback function for options dialog
//-------------------------------------------------------------------//
gboolean difference_changed( GtkWidget *_widget, GdkEvent  *_event, gpointer   user_data )
{
  global_options.dose_difference = g_ascii_strtod( gtk_entry_get_text (GTK_ENTRY(_widget) ), NULL);
  return FALSE;
}

//-------------------------------------------------------------------//
// Callback function for options dialog
//-------------------------------------------------------------------//
gboolean dta_changed( GtkWidget *_widget, GdkEvent  *_event, gpointer   user_data )
{
  global_options.dta = g_ascii_strtod( gtk_entry_get_text (GTK_ENTRY(_widget) ), NULL);
  return FALSE;
}

//-------------------------------------------------------------------//
// Callback function for options dialog
//-------------------------------------------------------------------//
void color1_changed( GtkColorButton *_widget, gpointer   user_data )
{
  gtk_color_chooser_get_rgba( GTK_COLOR_CHOOSER(_widget), &global_options.color_1);
}

//-------------------------------------------------------------------//
// Callback function for options dialog
//-------------------------------------------------------------------//
void color2_changed( GtkColorButton *_widget, gpointer   user_data )
{
  gtk_color_chooser_get_rgba( GTK_COLOR_CHOOSER(_widget), &global_options.color_2);
}



//-------------------------------------------------------------------//
// Function activated after respponse signal is emmited from options 
// dialog if response is "save" file is written, otherwise options
// prewiously saved in file are restored as defaults.
//-------------------------------------------------------------------//
void dialog_response (GtkDialog *_dialog, gint _id, gpointer _user_data)
{
  if( _id == GTK_RESPONSE_ACCEPT )
  {
    GString *_temp_text = NULL;
    _temp_text = g_string_new ("");
    g_string_printf ( _temp_text, "dose_difference[%%]: %.3f\ndta[mm]: %.3f\n%s\n%s",
                                   global_options.dose_difference, global_options.dta,
                                   gdk_rgba_to_string (&global_options.color_1),
                                   gdk_rgba_to_string (&global_options.color_2),
                                   NULL );
    if( !g_file_set_contents( OPTIONS_FILE, _temp_text->str, -1, NULL ) )msg("Error while saving file"); 
    g_string_free( _temp_text, TRUE );
  }
  else if( _id == GTK_RESPONSE_APPLY )
  {
    compare_button_clicked();
  }
  else
  {
    load_options();
    gtk_widget_destroy( GTK_WIDGET(_dialog) );
  }
}

//-------------------------------------------------------------------//
// Function try to read options file and replace option values
// if there is no file or something is wrong default vals are set
//-------------------------------------------------------------------//
gboolean load_options()
{
  global_options.dose_difference = 2.0;
  global_options.dta = 2.0;
  gdk_rgba_parse( &global_options.color_1, "red" );
  gdk_rgba_parse( &global_options.color_2, "blue" );
  if(g_file_test ( OPTIONS_FILE, G_FILE_TEST_IS_REGULAR))
  {
    gchar *_contents = NULL;
    gsize *_length = NULL;
    guint _np;
    gchar **_temp_line_splitted = NULL;
    gchar **_sp = NULL;

    g_file_get_contents( OPTIONS_FILE, &_contents, _length, NULL );
   
    _sp = g_strsplit ( _contents, "\n", G_MAXINT );
    _np = g_strv_length( _sp );
    if( _np < 4 )
    {
      msg("File should have more lines");
      g_strfreev( _sp );
      return FALSE;
    }
   
    if( ! g_str_has_prefix ( _sp[0], "dose_difference[%]:" ) ) //first line
    {
      msg("First line should be i.e.:\n\"dose_difference[%]: 2.0\" setting default value");
    }
    else
    {
      _temp_line_splitted = g_strsplit ( _sp[0], " ", G_MAXINT );
      global_options.dose_difference = g_ascii_strtod ( _temp_line_splitted[1], NULL);
      g_strfreev( _temp_line_splitted );
    }
    if( ! g_str_has_prefix ( _sp[1], "dta[mm]:" ) ) //second line
    {
      msg("Second line should be i.e.:\n\"dta[mm]: 2.0\" setting default value");
    }
    else
    {
      _temp_line_splitted = g_strsplit ( _sp[1], " ", G_MAXINT );
      global_options.dta = g_ascii_strtod ( _temp_line_splitted[1], NULL);
      g_strfreev( _temp_line_splitted );
    }
    if( !gdk_rgba_parse( &global_options.color_1, _sp[2] ) ) msg("Third line should represent a color");
    if( !gdk_rgba_parse( &global_options.color_2, _sp[3] ) ) msg("Fourth line should represent a color");
    g_strfreev( _sp );
    return TRUE;
  }
  else return FALSE;
}

//-------------------------------------------------------------------//
// Utility function abs1
//-------------------------------------------------------------------//
gdouble abs1(gdouble _x)
{
  return _x<0?-_x:_x; 
}


//-------------------------------------------------------------------//
// Function returns dose value for given _x value, Dose(x),
// _g is array filled with points, _sensitivity should be set
// to 0.5*step_of_array; 
//-------------------------------------------------------------------//
point* find_point_in_garray( GArray* _g, gdouble _x, gdouble _sensitivity )
{
  gint _i;
  for( _i=0; _i<=_g->len; _i++ )
  {
    if( abs1( g_array_index( _g, point, _i ).x - _x) < _sensitivity ) return &g_array_index( _g, point, _i );
  }
//  return NULL;
}


//-------------------------------------------------------------------//
// Base function to check if two poins are the same or different
// according to settings defined. 
// We check the point located at _x
// from checked_garray_trimmed against all _second garray points,
// these arrays are monaco or omnipro arrays - depending on 1vs2 or 2vs1 option,
//
// According to defined dose diff and dta values;
// returns 0 if we meet criteria, if not returns 1 //like gamma method
// -1 on error
//-------------------------------------------------------------------//
gdouble check_dta(  GArray*_second, point*_p )
{
  gdouble _difference = global_options.dose_difference;
  gdouble _dta = global_options.dta;

  if( _dta <= 0.0 ){msg("Dta should be a number > 0.0"); return -1;}
  _dta = _dta * 0.1; //conversion from mm to cm (in arrays metrics are in cm)

  gdouble _first_dose, _second_dose, _diff_0, _diff_m1, _diff_p1 ; //plus 1, minus 1
  gdouble _x = _p->x;
  gdouble _x2 = _x;
  gdouble _delta_x = 100;
  gint _i, _j;

  GString* _s;
  GArray* _temp_garray; 

  _first_dose = _p->dose;
  _second_dose = find_point_in_garray( _second, _x, 0.5*get_step_of_garray( _second ) )->dose;
  _s = g_string_new("");
  /**/  g_string_append_printf( _s, "FD: %2.3f, SD: %2.3f; ", _first_dose, _second_dose, NULL );
  
  /**/  g_string_append_printf( _s, "DIFF : is: %2.2f%%, should be lt: %2.1f%% ", abs1( _first_dose - _second_dose ), _difference, NULL);
  if( abs1( _first_dose - _second_dose ) < _difference )
  // percentage difference criteria meet
  {
    _p->desc = g_strdup_printf("%s", _s->str, NULL );
    g_string_free( _s, TRUE );
    return 0;
  } //good
  /**/  g_string_append_printf( _s, " [ ", NULL);
  // otherwise we check the nearest point
  if( algorithm == ALG_SIMPLE_DTA )
  {
  // in that algorithm we simply go left or right on the garray: the_other_garray, beginning from the place (x)
  // we want to check. We check the difference between dose values. |D1 - D2| If we meet the local minimum 
  // (the next dose difference is greater), we check the distance x1-x2.
  //_new_garray_min/ If it is less than defined dta the point is ok, otherwise not.
  // This is simple way for theoretic curves, but for real measurement or real calculations, wchich are not smooth,
  // someteimes is gives false results. Because of that other algorithms have been implemented.
    for(;;)
    {
      _second_dose = find_point_in_garray( _second, _x2, 0.5*get_step_of_garray( _second ) )->dose;
      _diff_0 = abs1( _first_dose - _second_dose );
    /**/  g_string_append_printf( _s, "D: %2.2f, DD: %2.2f ", _second_dose, _diff_0, NULL);
      _second_dose = find_point_in_garray( _second, _x2+get_step_of_garray( _second ), 0.5*get_step_of_garray( _second ) )->dose;
      _diff_p1 = abs1( _first_dose - _second_dose );
      _second_dose = find_point_in_garray( _second, _x2-get_step_of_garray( _second ), 0.5*get_step_of_garray( _second ) )->dose;
      _diff_m1 = abs1( _first_dose - _second_dose );
  
      //we check if the smallest from plus1 and minus1 is even smaller than original dose diff _diff_0
      if((_diff_p1 < _diff_m1) && (_diff_p1 < _diff_0) ) _x2 = _x2 + get_step_of_garray( _second );else
      if( _diff_m1 < _diff_0 ) _x2 = _x2 - get_step_of_garray( _second );else break;
    }
    _delta_x = abs1( _x - _x2 ); // in cm
  }
  else if( algorithm == ALG_COMPLEX_DTA )
  {
	  point* _pm;
	  point* _pp;
	  gdouble _temp_x;
	  point _temp_point;
	  _temp_garray = NULL;
  // we need to find every point in _second array that has the same dose value that checking point has,
  // but there is no exact the same dose points, we have at least one point that has sligtly less dose value
  // and the next that has sligtly bigger dose. From those two points we need to calculate x value where dose
  // is the same. This values (x, dose) we store in point structure.
  // The amount of that kind of points can be several or quite more - if the graph is sharpen.
  //
  // Next step is to find one closest point and check its distance from the point we are evaluating
  // If the distance is less than dta, checked point meets criteria, otherwise not.
    _temp_garray = g_array_new( TRUE, TRUE, sizeof(point));
    for( _i=0; _i<_second->len - 1; _i++)
    {
          _pm = &g_array_index( _second, point, _i );

	  _pp = &g_array_index( _second, point, _i+1 );

	  if( ((_first_dose >= _pm->dose) && (_first_dose < _pp->dose))
	        || ((_first_dose <= _pm->dose) && (_first_dose > _pp->dose))  )
	  { // _first_dose is between _pm->dose and _pp->dose
            // now we have to estimate x
		  _temp_point.x = ((_pp->x-_pm->x)*(_first_dose-_pm->dose))/(_pp->dose - _pm->dose) + _pm->x; 
		  //from simple proportion
		  _temp_point.dose = _first_dose; // ought to be the same
		  _temp_point.result = 0;
		  _temp_point.desc = NULL;
		  g_array_append_val( _temp_garray, _temp_point );
	  }

    }//now we need to find the closest from these points
    for( _i=0; _i<_temp_garray->len; _i++)
    {
	   _x2 = abs1(_x - g_array_index( _temp_garray, point, _i ).x );
	   if( _x2 < _delta_x ) _delta_x = _x2;
    }
    g_array_free( _temp_garray, TRUE );  
  }
  /**/  g_string_append_printf( _s, " _delta_x: %2.1fcm ] ", _delta_x, NULL);
  _p->desc = g_strdup_printf("%s", _s->str, NULL );
  g_string_free( _s, TRUE );
  if( _delta_x < _dta ) return 0; // in mm //good

  return 1; //bad
}

//-------------------------------------------------------------------//
// Utility function get_step_of_garray
//-------------------------------------------------------------------//
gdouble get_step_of_garray( GArray* _g )
{
  if ( _g ) 
  {
    if ( _g->len >= 2 ) 
    {
      gdouble _temp = 0;
      _temp = g_array_index( _g, point, 1 ).x - g_array_index( _g, point, 0 ).x;
      if( _temp < 0 ) _temp = -_temp;
      return _temp;
    }
  }
  return 0;
}


//-------------------------------------------------------------------//
// Function changes values in garray according to specified normalization 
//-------------------------------------------------------------------//
void normalize_graph( graph _g, guint _norm_type )
{
  guint _i;
  gdouble _temp = 0;
  point _point, _point_2;

  if( _g.type == GT_DEPTH)
  {
    for(_i=0; _i<_g.g->len; _i++) if( g_array_index( _g.g, point, _i ).dose > _temp )
                                   _temp = g_array_index( _g.g, point, _i ).dose;
  }
  if( (_g.type == GT_CROSSLINE) || (_g.type == GT_INLINE) )
  {
    if( _norm_type == NORM_TO_CENTER )
      {
      if( _g.g->len % 2 == 1 )
      {
        _point = g_array_index( _g.g, point, (guint)(_g.g->len * 0.5 + 0.5) );
        _temp = _point.dose;
      } 
      else 
      {
        _point = g_array_index( _g.g, point, (guint)(_g.g->len * 0.5) );
        _point_2 = g_array_index( _g.g, point, (guint)(_g.g->len * 0.5 + 1) );
        _temp = ( _point.dose + _point_2.dose ) * 0.5;
      }
    }
    else //NORM_TO_MAX
    {
      _temp = max_dose_from_garray( _g.g );
    }
  }
 
  _temp = 100/_temp;

  for(_i=0;_i<_g.g->len;_i++)
  {
    g_array_index( _g.g, point, _i ).dose = g_array_index( _g.g, point, _i ).dose*_temp;
  }
}


//max x from garray
gdouble max_x_from_garray( GArray* _g )
{
  guint _i;
  gdouble _temp = -10e10;
  for(_i=0; _i<_g->len; _i++)
      if( g_array_index( _g, point, _i ).x > _temp ) _temp = g_array_index( _g, point, _i ).x;
  return _temp;
}

//max dose from garray
gdouble max_dose_from_garray( GArray* _g )
{
  guint _i;
  gdouble _temp = -10e10;
  for(_i=0; _i<_g->len; _i++)
      if( g_array_index( _g, point, _i ).dose > _temp ) _temp = g_array_index( _g, point, _i ).dose;
  return _temp;
}

//min x from garray
gdouble min_x_from_garray( GArray* _g )
{
  guint _i;
  gdouble _temp = 10e10;
  for(_i=0; _i<_g->len; _i++)
      if( g_array_index( _g, point, _i ).x < _temp ) _temp = g_array_index( _g, point, _i ).x;
  return _temp;
}

// Function displays simple message on screen
void msg( const gchar *_m)
{
  GtkWidget *_dialog;

  _dialog = gtk_message_dialog_new (GTK_WINDOW( main_window ),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_INFO, GTK_BUTTONS_OK, _m, NULL );
  gtk_dialog_run(GTK_DIALOG (_dialog));
  gtk_widget_destroy ( _dialog );
}

// Function checks if string is a number
gboolean is_it_number( const gchar *_c )
{
  gint _i = 0;
  
  if( !_c ) return FALSE; //nothing there
  if( !_c[0] ) return FALSE; //not sure 'bout that
  for( ;; )
  { 
    if( !_c[_i] )return TRUE; //every one before must be a digit and this one is null
    if( g_ascii_isdigit ( _c[_i] ) ){ _i++; }
    else return FALSE;
  }
  return FALSE; //just in case
}

// Function returns number of rows in given GtkTreeView
guint get_number_of_rows( GtkTreeView* _t )
{
  guint _i = 0;
  GtkTreePath* _p = gtk_tree_path_new_first();
  GtkTreeIter _iter;
  while( gtk_tree_model_get_iter( gtk_tree_view_get_model( _t ), &_iter, _p ) )
  {
    gtk_tree_path_next( _p );
    _i++;
  }
  return _i;
}


//-------------------------------------------------------------------//
// The next four are normalization functions from menu 
//-------------------------------------------------------------------//
void menu_item_2d_normalize_to_max_clicked()
{
  if(monaco_graph.g->len == 0){msg("No data loaded");return;}
  normalize_graph( monaco_graph, NORM_TO_MAX );
  are_calculations_current = FALSE;
  gtk_widget_queue_draw ( monaco_da );
  gtk_widget_queue_draw ( compare_da );
}

void menu_item_2d_normalize_to_center_clicked()
{
  if(monaco_graph.g->len == 0){msg("No data loaded");return;}
  normalize_graph( monaco_graph, NORM_TO_CENTER );
  are_calculations_current = FALSE;
  gtk_widget_queue_draw ( monaco_da );
  gtk_widget_queue_draw ( compare_da );
}

void menu_item_1d_normalize_to_max_clicked()
{
  if(omnipro_graph.g->len == 0){msg("No data loaded");return;}
  normalize_graph( omnipro_graph, NORM_TO_MAX );
  are_calculations_current = FALSE;
  gtk_widget_queue_draw ( omnipro_da );
  gtk_widget_queue_draw ( compare_da );
}

void menu_item_1d_normalize_to_center_clicked()
{
  if(omnipro_graph.g->len == 0){msg("No data loaded");return;}
  normalize_graph( omnipro_graph, NORM_TO_CENTER );
  are_calculations_current = FALSE;
  gtk_widget_queue_draw ( omnipro_da );
  gtk_widget_queue_draw ( compare_da );
}

void menu_item_2d_clear_clicked()
{
   reset_graph_calculations( &monaco_graph );
   g_array_set_size( monaco_graph.g, 0 );
   gtk_widget_queue_draw ( monaco_da );
   gtk_widget_queue_draw ( compare_da );
}

void menu_item_1d_clear_clicked()
{
   reset_graph_calculations( &omnipro_graph );
   g_array_set_size( omnipro_graph.g, 0 );
   gtk_widget_queue_draw ( omnipro_da );
   gtk_widget_queue_draw ( compare_da );
}


void main_quit()
{
  if( lines_splitted)  g_strfreev( lines_splitted );
  if( monaco_graph.g )g_array_free ( monaco_graph.g, TRUE );
  if( omnipro_graph.g )g_array_free ( omnipro_graph.g, TRUE );
  if( omnipro_sets_garray )g_array_free ( omnipro_sets_garray, TRUE );
  if( plane ) g_free( plane );
  gtk_main_quit();
}


// preview of imported 2d plane
// not implemented yet
void m_2d_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data )
{
  GdkRGBA _color;
  gint _width  = gtk_widget_get_allocated_width (_widget);
  gint _height = gtk_widget_get_allocated_height (_widget);

  // background
  cairo_rectangle ( _cr, _width * 0.05, _height * 0.05, _width * 0.9, _height * 0.9 );
  gdk_rgba_parse (&_color,"white");
  gdk_cairo_set_source_rgba (_cr, &_color);
  cairo_fill( _cr );

  //axis
  cairo_move_to ( _cr, _width *0.5, _height *0.9 );
  cairo_line_to ( _cr, _width *0.5, _height *0.1 );

  cairo_move_to ( _cr, _width *0.1, _height *0.5 );
  cairo_line_to ( _cr, _width *0.9, _height *0.5 );

  gdk_rgba_parse (&_color,"black");
  cairo_set_line_width( _cr, 0.7 );
  gdk_cairo_set_source_rgba (_cr, &_color);
  cairo_stroke ( _cr );
  

}


void monaco_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data )
{
  gint _w  = gtk_widget_get_allocated_width (_widget);
  gint _h = gtk_widget_get_allocated_height (_widget);
  draw_background( &monaco_graph, _cr, _w, _h, 0, 0 );
  draw_graph( &monaco_graph, _cr, _w, _h, 0, 0 );
  draw_fwhm_txt( &monaco_graph, _cr, _w, _h, 0, 0, "2D FWHM" );
  draw_fwhm_line( &monaco_graph, _cr, _w, _h, 0, 0 );

}

void omnipro_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data )
{
  gint _w  = gtk_widget_get_allocated_width (_widget);
  gint _h = gtk_widget_get_allocated_height (_widget);
  draw_background( &omnipro_graph, _cr, _w, _h, 0, 0 );
  draw_graph( &omnipro_graph, _cr, _w, _h, 0, 0 );
  draw_fwhm_txt( &omnipro_graph, _cr, _w, _h, 0, 0, "1D FWHM" );
  draw_fwhm_line( &omnipro_graph, _cr, _w, _h, 0, 0 );

}

void compare_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data )
{
  gint _w  = gtk_widget_get_allocated_width (_widget);
  gint _h = gtk_widget_get_allocated_height (_widget);
  draw_background( &monaco_graph, _cr, _w, _h, 0, 0 ); 
  if( are_calculations_current )
  {
    graph*_tested_g=NULL;
    graph*_other_g=NULL;

    if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(compare_rb_1)) )
    {
      _tested_g = &monaco_graph;
      _other_g  = &omnipro_graph;
    }
    else
    {
      _tested_g = &omnipro_graph;
      _other_g  = &monaco_graph;
    }
    draw_background( _tested_g, _cr, _w, _h, 0, 0 ); 

    draw_graph( &omnipro_graph, _cr, _w, _h, 0, 0 );
    draw_graph( &monaco_graph, _cr, _w, _h, 0, 0 );
    draw_dots( _tested_g, _cr, _w, _h, 0, 0 );
    draw_fwhm_txt( &omnipro_graph, _cr, _w, _h, 0, 0, "1D FWHM" );
    draw_fwhm_line( &omnipro_graph, _cr, _w, _h, 0, 0 );
    draw_fwhm_txt( &monaco_graph, _cr, _w, _h, 0, 15, "2D FWHM" );
    draw_fwhm_line( &monaco_graph, _cr, _w, _h, 0, 0 );
  }
}


void compare_button_clicked()
{
  if( omnipro_graph.g->len == 0 || monaco_graph.g->len == 0 )
  { // if one of array is empty, draw result anyway
    gtk_widget_queue_draw( compare_da );
    return;
  }

  if( !( (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(monaco_rb_1)) &&
                     ((omnipro_graph.type == GT_CROSSLINE) || (omnipro_graph.type == GT_INLINE)) )
          ||( ( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(monaco_rb_2)) || gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(monaco_rb_3)) ) &&
              (omnipro_graph.type == GT_DEPTH)) ) ){msg("Cannot compare different types of data");return;};
  
  if( abs1((get_step_of_garray(monaco_graph.g) / get_step_of_garray(omnipro_graph.g)) - 1.0) >= 0.001 )
             { msg("For now step size of each array should be the same.\nThis will be changed in the future.");return; }

  gdouble _step = MIN( get_step_of_garray(monaco_graph.g), get_step_of_garray(omnipro_graph.g) );
  if( _step == 0 ) {msg("Something is wrong with the step of an array.");return;}
  gdouble _d;
  gdouble _min_tested_x = min_common_x( &monaco_graph, &omnipro_graph );
  gdouble _max_tested_x = max_common_x( &monaco_graph, &omnipro_graph );
  gdouble _temp_dta;

  graph*_tested_g=NULL;
  graph*_other_g=NULL;

  if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(compare_rb_1)) )
  {
    _tested_g = &monaco_graph;
    _other_g  = &omnipro_graph;
  }
  else
  {
    _tested_g = &omnipro_graph;
    _other_g  = &monaco_graph;
  }

  reset_graph_calculations( _tested_g );
  for( _d=_min_tested_x; _d<=_max_tested_x; _d=_d+get_step_of_garray(_tested_g->g) )
  { 
    _temp_dta = check_dta( _other_g->g, find_point_in_garray( _tested_g->g, _d, 0.05 ) );
    find_point_in_garray( _tested_g->g, _d, 0.05 )->result = _temp_dta;
    if( _temp_dta == -1 ){ msg("Something went wrong while dta calculations.\n"); break; }
  }
  are_calculations_current = TRUE;
  gtk_widget_queue_draw ( compare_da );
}

void calculate_width_clicked()
{
  if( ! is_it_number(gtk_entry_get_text(GTK_ENTRY(width_ed_1))) ){msg("Given height is not a number, decimal signs are not allowed as well"); return;}

  GString*_t = g_string_new("");
  gdouble _h = g_strtod( gtk_entry_get_text(GTK_ENTRY(width_ed_1)), NULL );

  if( &omnipro_graph )calculate_width( &omnipro_graph, _h );
  g_string_printf( _t, "%.2f", omnipro_graph.first_50 );
  gtk_entry_set_text(GTK_ENTRY(width_ed_2), _t->str );
  g_string_printf( _t, "%.2f", omnipro_graph.second_50 );
  gtk_entry_set_text(GTK_ENTRY(width_ed_3), _t->str );

  if( &monaco_graph ) calculate_width( &monaco_graph, _h );
  g_string_printf( _t, "%.2f", monaco_graph.first_50 );
  gtk_entry_set_text(GTK_ENTRY(width_ed_4), _t->str );
  g_string_printf( _t, "%.2f", monaco_graph.second_50 );
  gtk_entry_set_text(GTK_ENTRY(width_ed_5), _t->str );
  g_string_free( _t, TRUE );
}

void menu_item_1d_print_x_clicked()
{
  if( ! is_it_number(gtk_entry_get_text(GTK_ENTRY(width_ed_1))) ){msg("Given height is not a number, decimal signs are not allowed as well"); return;}

  GString*_t = g_string_new("");
  gdouble _h = g_strtod( gtk_entry_get_text(GTK_ENTRY(width_ed_1)), NULL );
  if( &omnipro_graph )print_x_of_given_width( &omnipro_graph, _h );
  g_string_free( _t, TRUE );
}

void menu_item_2d_print_x_clicked()
{
  if( ! is_it_number(gtk_entry_get_text(GTK_ENTRY(width_ed_1))) ){msg("Given height is not a number, decimal signs are not allowed as well"); return;}

  GString*_t = g_string_new("");
  gdouble _h = g_strtod( gtk_entry_get_text(GTK_ENTRY(width_ed_1)), NULL );
  if( &monaco_graph ) print_x_of_given_width( &monaco_graph, _h );
  g_string_free( _t, TRUE );
}

gboolean compare_doses( gdouble _d1, gdouble _d2, gdouble _sensitivity )
{
  if( abs1( _d1 - _d2 ) < _sensitivity ) return TRUE;
  return FALSE;
}


gboolean compare_da_press (GtkWidget *_widget, GdkEvent  *_event, gpointer user_data)
{
  graph*_tested_g=NULL;
  graph*_other_g=NULL;

  // do we want to check 1vs2 or 2vs1 ?
  if( gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(compare_rb_1)) )
  {
    _tested_g = &monaco_graph;
    _other_g  = &omnipro_graph;
  }
  else
  {
    _tested_g = &omnipro_graph;
    _other_g  = &monaco_graph;
  }
  if( _tested_g->g->len==0 || _other_g->g->len==0 ) return TRUE;
  GString* _text;
  gint _w = gtk_widget_get_allocated_width  (_widget);
  gint _h = gtk_widget_get_allocated_height (_widget);
  point*_p;

  gdouble _dose_from_garray, _dose_from_da;
  gdouble _x_cm = ((gdouble)((GdkEventButton*)_event)->x/(gdouble)_w)*37.5 -18.75;
  _p = find_point_in_garray( _tested_g->g, _x_cm, 0.5*get_step_of_garray( _tested_g->g ) );
  _dose_from_garray = _p->dose;
  _dose_from_da = 112.5 - (((GdkEventButton*)_event)->y * 125)/(gdouble)_h;


  if( compare_doses( _dose_from_garray, _dose_from_da, 0.7) )
  {
    _text = g_string_new("");
    if(_p->desc) g_string_append_printf( _text, "x: %.1fcm; dose: %.2f%% %s", _x_cm, _dose_from_da, _p->desc, NULL );
    gtk_label_set_text( GTK_LABEL(msg_label), _text->str );
    g_string_free ( _text, TRUE );
    return TRUE; 
  }
  else gtk_label_set_text( GTK_LABEL(msg_label), "Choose point" );
  return TRUE;
}

gint main( gint argc, gchar **argv )
{
  gtk_disable_setlocale();
  gtk_init( &argc, &argv ); // initializes locale again if not disabled before

  monaco_graph.g = g_array_new (TRUE, TRUE, sizeof (point));
  monaco_graph.type = GT_UNDEFINED;
  omnipro_graph.g = g_array_new (TRUE, TRUE, sizeof (point));
  omnipro_graph.type = GT_UNDEFINED;

  omnipro_sets_garray = g_array_new (TRUE, TRUE, sizeof (omnipro_dataset));
  load_options();

  // Creating window with box in it
  // box contains menu_bar and so on
  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size( GTK_WINDOW(main_window), 800, 670 );
  gtk_window_set_title (GTK_WINDOW(main_window), PACKAGE_STRING );
  gtk_window_set_icon_name (GTK_WINDOW (main_window), "document-open");

  main_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 4 );//menu + hhpaned
  main_menu_bar = gtk_menu_bar_new();

  main_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0); //hhbox + comp_vbox

  hhbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2); //monaco_vbox + omnipro_vbox + compare_hbox

  omnipro_frame = gtk_frame_new("One dimmentional data: "); //omnipro_hbox
  gtk_container_set_border_width( GTK_CONTAINER(omnipro_frame), 2 );

    omnipro_vbox_menu = gtk_box_new( GTK_ORIENTATION_VERTICAL, 2 );  //omnipro_la_1 + ...

      omnipro_la_1 = gtk_label_new ("N of pts: ;N of sets: ;");
      omnipro_combo_box_text = gtk_combo_box_text_new_with_entry ();
      omnipro_button = gtk_button_new_with_label("Get selected dataset");
      g_signal_connect( omnipro_button, "clicked", G_CALLBACK( get_omnipro_dataset_clicked ), NULL ); 
      omnipro_la_2 = gtk_label_new ("Currently loaded set: ");
      omnipro_la_3 = gtk_label_new ("N of points in set: ");
      omnipro_la_4 = gtk_label_new ("Step: ");
      omnipro_da = gtk_drawing_area_new ();
      gtk_widget_set_size_request (omnipro_da, 100, 100);

      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_la_1, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_combo_box_text, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_button, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_la_2, FALSE, FALSE, 0 );//currenty loaded
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_la_3, FALSE, FALSE, 0 );//N of points
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_la_4, TRUE, FALSE, 0 );//Step
      gtk_box_pack_start( GTK_BOX(omnipro_vbox_menu), omnipro_da, FALSE, FALSE, 0 );

  gtk_container_add( GTK_CONTAINER(omnipro_frame), omnipro_vbox_menu );
  gtk_box_pack_start( GTK_BOX(hhbox), omnipro_frame, FALSE, FALSE, 2 );




  monaco_frame = gtk_frame_new("Two dimmentional plane: "); 
  gtk_container_set_border_width( GTK_CONTAINER(monaco_frame), 2 );
    monaco_vbox_menu = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );  //monaco_la_1 + monaco_ed_1 + monaco_la_2 + monaco_ed_2
      m_2d_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
        m_2d_la = gtk_label_new ("2d view:");
        m_2d_da = gtk_drawing_area_new ();
        gtk_widget_set_size_request (m_2d_da, 100, 100);
      gtk_box_pack_start( GTK_BOX(m_2d_box), m_2d_la, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(m_2d_box), m_2d_da, FALSE, FALSE, 0 );
      m_grid = gtk_grid_new();
      gtk_grid_insert_row( GTK_GRID(m_grid),1 );
      gtk_grid_insert_column( GTK_GRID(m_grid),1 );
      gtk_grid_insert_column( GTK_GRID(m_grid),2 );
        monaco_la_1 = gtk_label_new ("Row to export/get:");
        monaco_ed_1 = gtk_entry_new ();
        gtk_entry_set_max_length (GTK_ENTRY(monaco_ed_1), 5);
        gtk_entry_set_max_width_chars (GTK_ENTRY(monaco_ed_1), 5);
        gtk_entry_set_width_chars (GTK_ENTRY(monaco_ed_1), 5);

        monaco_bu_1 = gtk_button_new_with_label("Get it");
        g_signal_connect( monaco_bu_1, "clicked", G_CALLBACK( get_monaco_row_clicked ), NULL ); 
       gtk_grid_attach (GTK_GRID(m_grid), monaco_la_1, 0,0, 1,1 );
       gtk_grid_attach (GTK_GRID(m_grid), monaco_ed_1, 1,0, 1,1 );
       gtk_grid_attach (GTK_GRID(m_grid), monaco_bu_1, 2,0, 1,1 );

        monaco_la_2 = gtk_label_new ("Column to export/get:");
        monaco_ed_2 = gtk_entry_new ();
        gtk_entry_set_max_length (GTK_ENTRY(monaco_ed_2), 5);
        gtk_entry_set_max_width_chars (GTK_ENTRY(monaco_ed_2), 5);
        gtk_entry_set_width_chars (GTK_ENTRY(monaco_ed_2), 5);
        monaco_bu_2 = gtk_button_new_with_label("Get it");
        g_signal_connect( monaco_bu_2, "clicked", G_CALLBACK( get_monaco_column_clicked ), NULL ); 
       gtk_grid_attach (GTK_GRID(m_grid), monaco_la_2, 0,1, 1,1 );
       gtk_grid_attach (GTK_GRID(m_grid), monaco_ed_2, 1,1, 1,1 );
       gtk_grid_attach (GTK_GRID(m_grid), monaco_bu_2, 2,1, 1,1 );

      monaco_la_3 = gtk_label_new ("Currently loaded: ");
      monaco_la_4 = gtk_label_new ("N of points: ");
      monaco_la_5 = gtk_label_new ("Step: ");

      monaco_rb_1 = gtk_radio_button_new_with_label ( NULL, "XY plane (in, cross)");
      monaco_rb_2 = gtk_radio_button_new_with_label_from_widget ( GTK_RADIO_BUTTON (monaco_rb_1), "XZ plane (PDD)");
      monaco_rb_3 = gtk_radio_button_new_with_label_from_widget ( GTK_RADIO_BUTTON (monaco_rb_1), "YZ plane (PDD)");

      monaco_da = gtk_drawing_area_new ();
      gtk_widget_set_size_request (monaco_da, 100, 100);

      // gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), m_2d_box, FALSE, FALSE, 0 );
      // 2d box can be added is some future.
      // For now 2d data is stored as text in gchar**, not as gfloat values or points
      // Only 1d graphs are represented as g_arrays of points.
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), m_grid, FALSE, FALSE, 0 );//text + editable + button
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_la_3, FALSE, FALSE, 0 );//currenty loaded
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_la_4, FALSE, FALSE, 0 );//N of points
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_la_5, FALSE, FALSE, 0 );//Step
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_rb_1, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_rb_2, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_rb_3, FALSE, FALSE, 0 );
      gtk_box_pack_start( GTK_BOX(monaco_vbox_menu), monaco_da, FALSE, TRUE, 0 );
  gtk_container_add( GTK_CONTAINER(monaco_frame), monaco_vbox_menu );
  gtk_box_pack_start( GTK_BOX(hhbox), monaco_frame, FALSE, FALSE, 2 );


  width_frame = gtk_frame_new("Width of profiles");
  gtk_container_set_border_width( GTK_CONTAINER(width_frame), 2 );
    width_grid = gtk_grid_new();
    gtk_grid_set_row_spacing( GTK_GRID(width_grid), 1 );
    gtk_grid_set_column_spacing( GTK_GRID(width_grid), 1 );
    gtk_grid_set_column_homogeneous( GTK_GRID(width_grid), TRUE );
      gtk_grid_insert_row( GTK_GRID(width_grid),1 );
      gtk_grid_insert_row( GTK_GRID(width_grid),2 );
      gtk_grid_insert_column( GTK_GRID(width_grid),1 );
      gtk_grid_insert_column( GTK_GRID(width_grid),2 );
      gtk_grid_insert_column( GTK_GRID(width_grid),3 );
        width_la_1 = gtk_label_new ("Height [%]:"); // first row
        width_ed_1 = gtk_entry_new ();
        gtk_entry_set_max_length (GTK_ENTRY(width_ed_1), 3);
        gtk_entry_set_max_width_chars (GTK_ENTRY(width_ed_1), 3);
        gtk_entry_set_width_chars (GTK_ENTRY(width_ed_1), 3);
        width_la_2 = gtk_label_new ("L(-) [cm]:");
        width_la_3 = gtk_label_new ("R(+) [cm]:");
      gtk_grid_attach (GTK_GRID(width_grid), width_la_1, 0,0, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_ed_1, 1,0, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_la_2, 2,0, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_la_3, 3,0, 1,1 );

        width_la_4 = gtk_label_new ("1D:");
        width_ed_2 = gtk_entry_new (); // second row
//        gtk_entry_set_max_length (GTK_ENTRY(width_ed_2), 4);
        gtk_entry_set_max_width_chars (GTK_ENTRY(width_ed_2), 4);
        gtk_entry_set_width_chars (GTK_ENTRY(width_ed_2), 4);
        gtk_widget_set_can_focus( width_ed_2, FALSE );

        width_ed_3 = gtk_entry_new ();
//        gtk_entry_set_max_length (GTK_ENTRY(width_ed_3), 4);
        gtk_entry_set_max_width_chars (GTK_ENTRY(width_ed_3), 4);
        gtk_entry_set_width_chars (GTK_ENTRY(width_ed_3), 4);
        gtk_widget_set_can_focus( width_ed_3, FALSE );
      gtk_grid_attach (GTK_GRID(width_grid), width_la_4, 1,1, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_ed_2, 2,1, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_ed_3, 3,1, 1,1 );

        width_bu_1 = gtk_button_new_with_label("Calc"); // third row
        g_signal_connect( width_bu_1, "clicked", G_CALLBACK( calculate_width_clicked ), NULL ); 

        width_la_5 = gtk_label_new ("2D:");
        width_ed_4 = gtk_entry_new ();
//        gtk_entry_set_max_length (GTK_ENTRY(width_ed_4), 4);
        gtk_entry_set_max_width_chars (GTK_ENTRY(width_ed_4), 4);
        gtk_entry_set_width_chars (GTK_ENTRY(width_ed_4), 4);
        gtk_widget_set_can_focus( width_ed_4, FALSE );

        width_ed_5 = gtk_entry_new ();
//        gtk_entry_set_max_length (GTK_ENTRY(width_ed_5), 4);
        gtk_entry_set_max_width_chars (GTK_ENTRY(width_ed_5), 4);
        gtk_entry_set_width_chars (GTK_ENTRY(width_ed_5), 4);
        gtk_widget_set_can_focus( width_ed_5, FALSE );
      gtk_grid_attach (GTK_GRID(width_grid), width_bu_1, 0,2, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_la_5, 1,2, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_ed_4, 2,2, 1,1 );
      gtk_grid_attach (GTK_GRID(width_grid), width_ed_5, 3,2, 1,1 );

    gtk_container_add( GTK_CONTAINER(width_frame), width_grid );
  gtk_box_pack_start( GTK_BOX(hhbox), width_frame, FALSE, FALSE, 2 );


  compare_hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 4 );
      compare_button = gtk_button_new_with_label("Compare");
      gtk_box_pack_start( GTK_BOX(compare_hbox), compare_button, TRUE, TRUE, 2 );
      g_signal_connect( compare_button, "clicked", G_CALLBACK( compare_button_clicked ), NULL );
      compare_rb_1 = gtk_radio_button_new_with_label ( NULL, "1vs2");
      gtk_box_pack_start( GTK_BOX(compare_hbox), compare_rb_1, FALSE, FALSE, 2 );
      compare_rb_2 = gtk_radio_button_new_with_label_from_widget ( GTK_RADIO_BUTTON (compare_rb_1), "2vs1");
      gtk_box_pack_start( GTK_BOX(compare_hbox), compare_rb_2, FALSE, FALSE, 2 );
  gtk_box_pack_start( GTK_BOX(hhbox), compare_hbox, TRUE, TRUE, 2 );


  compare_vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 4 );

    compare_da = gtk_drawing_area_new ();
      gtk_widget_set_size_request (compare_da, 400, 300);
      g_signal_connect ( compare_da, "button-press-event", G_CALLBACK( compare_da_press ), NULL );
      gtk_widget_set_events  (compare_da, gtk_widget_get_events (compare_da)
                             | GDK_BUTTON_PRESS_MASK );
    gtk_box_pack_start( GTK_BOX(compare_vbox), compare_da, TRUE, TRUE, 4 );
    
    msg_bar = gtk_info_bar_new ();
      gtk_info_bar_set_message_type (GTK_INFO_BAR (msg_bar), GTK_MESSAGE_INFO);
      msg_label = gtk_label_new ("");
      gtk_label_set_line_wrap ( GTK_LABEL(msg_label), TRUE );
      gtk_box_pack_start (GTK_BOX (gtk_info_bar_get_content_area (GTK_INFO_BAR (msg_bar))), msg_label, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (compare_vbox), msg_bar, FALSE, FALSE, 0);
  
  gtk_box_pack_start( GTK_BOX(main_hbox), hhbox, FALSE, FALSE, 4 );
  gtk_box_pack_start( GTK_BOX(main_hbox), compare_vbox, TRUE, TRUE, 4 );
  gtk_box_pack_start( GTK_BOX(main_box), main_menu_bar, FALSE, FALSE, 0 );
  gtk_box_pack_start( GTK_BOX(main_box), main_hbox, TRUE, TRUE, 0 );
  gtk_container_add( GTK_CONTAINER(main_window), main_box );
   

  // Menu:
  menu_item_file = gtk_menu_item_new_with_label( "File" );
  gtk_menu_shell_append( GTK_MENU_SHELL( main_menu_bar ), menu_item_file );
  menu_item_omnipro = gtk_menu_item_new_with_label( "1D data" );
  gtk_menu_shell_append( GTK_MENU_SHELL( main_menu_bar ), menu_item_omnipro );
  menu_item_monaco = gtk_menu_item_new_with_label( "2D data" );
  gtk_menu_shell_append( GTK_MENU_SHELL( main_menu_bar ), menu_item_monaco );

  menu_file = gtk_menu_new();
    menu_item_options = gtk_menu_item_new_with_label( "Options" );
    menu_item_report = gtk_menu_item_new_with_label( "Print/view report" );
    menu_item_about = gtk_menu_item_new_with_label( "About" );
    menu_item_exit = gtk_menu_item_new_with_label( "Exit" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_file ), menu_item_options );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_file ), menu_item_report );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_file ), menu_item_about );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_file ), menu_item_exit );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(menu_item_file), menu_file );

  menu_omnipro = gtk_menu_new();
    menu_item_open_omnipro_accept = gtk_menu_item_new_with_label( "Open OmniproAccept csv file" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_open_omnipro_accept );
    menu_item_open_1d_from_csv = gtk_menu_item_new_with_label( "Load 1d data from csv file" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_open_1d_from_csv );
    menu_item_save_omnipro_graph = gtk_menu_item_new_with_label( "Export given 1D graph" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_save_omnipro_graph );
    menu_item_1d_normalize_to_max = gtk_menu_item_new_with_label( "Normalize to Dmax" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_1d_normalize_to_max );
    menu_item_1d_normalize_to_center = gtk_menu_item_new_with_label( "Normalize to center" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_1d_normalize_to_center );
    menu_item_1d_print_x = gtk_menu_item_new_with_label( "Print all x of given height" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_1d_print_x );
    menu_item_1d_clear = gtk_menu_item_new_with_label( "Clear" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_omnipro ), menu_item_1d_clear );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(menu_item_omnipro), menu_omnipro );
 
  menu_monaco = gtk_menu_new();
    menu_item_open_monaco_plane = gtk_menu_item_new_with_label( "Open Monaco plane file" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_open_monaco_plane );
    menu_item_save_monaco_graph = gtk_menu_item_new_with_label( "Export given Monaco graph" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_save_monaco_graph );
    menu_item_open_omnipro_imrt_plane = gtk_menu_item_new_with_label( "Open OmniPro I'mRT plane file" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_open_omnipro_imrt_plane );
    menu_item_2d_normalize_to_max = gtk_menu_item_new_with_label( "Normalize to Dmax" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_2d_normalize_to_max );
    menu_item_2d_normalize_to_center = gtk_menu_item_new_with_label( "Normalize to center" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_2d_normalize_to_center );
    menu_item_2d_print_x = gtk_menu_item_new_with_label( "Print all x of given height" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_2d_print_x );
    menu_item_2d_clear = gtk_menu_item_new_with_label( "Clear" );
    gtk_menu_shell_append( GTK_MENU_SHELL( menu_monaco ), menu_item_2d_clear );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(menu_item_monaco), menu_monaco );

  gtk_widget_show_all( main_window );

  // SIGNALS 
  g_signal_connect( menu_item_exit, "activate",    G_CALLBACK( main_quit ), NULL ); 
  g_signal_connect( menu_item_options, "activate", G_CALLBACK( options_dialog ), NULL ); 
  g_signal_connect( menu_item_about, "activate", G_CALLBACK( about_dialog ), NULL ); 
  g_signal_connect( menu_item_report, "activate", G_CALLBACK( report_dialog ), NULL ); 


  g_signal_connect( main_window, "delete_event",  G_CALLBACK(main_quit), NULL );
  g_signal_connect( menu_item_open_monaco_plane, "activate", G_CALLBACK(open_monaco_plane_clicked), NULL ); 
  g_signal_connect( menu_item_save_monaco_graph, "activate", G_CALLBACK(save_monaco_graph_clicked), NULL ); 
  g_signal_connect( menu_item_open_omnipro_imrt_plane, "activate", G_CALLBACK(open_omnipro_imrt_plane_clicked), NULL ); 
  g_signal_connect( menu_item_2d_normalize_to_max, "activate", G_CALLBACK(menu_item_2d_normalize_to_max_clicked), NULL ); 
  g_signal_connect( menu_item_2d_normalize_to_center, "activate", G_CALLBACK(menu_item_2d_normalize_to_center_clicked), NULL ); 
  g_signal_connect( menu_item_2d_print_x, "activate", G_CALLBACK(menu_item_2d_print_x_clicked), NULL ); 
  g_signal_connect( menu_item_2d_clear, "activate", G_CALLBACK(menu_item_2d_clear_clicked), NULL ); 

  g_signal_connect( menu_item_open_omnipro_accept, "activate", G_CALLBACK(open_omnipro_accept_clicked), NULL ); 
  g_signal_connect( menu_item_open_1d_from_csv, "activate", G_CALLBACK(open_1d_from_csv_clicked), NULL ); 
  g_signal_connect( menu_item_save_omnipro_graph, "activate", G_CALLBACK(save_omnipro_graph_clicked), NULL ); 
  g_signal_connect( menu_item_1d_normalize_to_max, "activate", G_CALLBACK(menu_item_1d_normalize_to_max_clicked), NULL ); 
  g_signal_connect( menu_item_1d_normalize_to_center, "activate", G_CALLBACK(menu_item_1d_normalize_to_center_clicked), NULL ); 
  g_signal_connect( menu_item_1d_print_x, "activate", G_CALLBACK(menu_item_1d_print_x_clicked), NULL ); 
  g_signal_connect( menu_item_1d_clear, "activate", G_CALLBACK(menu_item_1d_clear_clicked), NULL ); 


  g_signal_connect (G_OBJECT (m_2d_da), "draw",            G_CALLBACK (m_2d_da_draw), NULL);
  g_signal_connect (G_OBJECT (monaco_da), "draw",            G_CALLBACK (monaco_da_draw), NULL);
  g_signal_connect (G_OBJECT (omnipro_da), "draw",            G_CALLBACK (omnipro_da_draw), NULL);
  g_signal_connect (G_OBJECT (compare_da), "draw",            G_CALLBACK (compare_da_draw), NULL);

  gtk_main();
  return 0;
}

  

