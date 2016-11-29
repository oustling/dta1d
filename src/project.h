#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#define OPTIONS_FILE "dta1d.options"

#include <gtk/gtk.h>
#include <locale.h>

#include "../config.h"

typedef struct
{
 gdouble dose_difference; //in %
 gdouble dta; // in mm
 GdkRGBA color_1;
 GdkRGBA color_2;
} options;

options global_options;


typedef struct
{
  guint first_row;
  guint last_row;
}omnipro_dataset;

typedef struct
{
  gdouble x;
  gdouble dose;
  gdouble result; //filled after comparing two arrays  1-ok
  gchar* desc; //filled while evaluating an algorithm, for debugging or showing how it works
} point;

typedef struct
{
  GArray* g;
  guint type;
  gdouble first_50; //the points where dose is 50 percent
  gdouble second_50;
} graph;


//graph type
#define  GT_UNDEFINED 0
#define  GT_CROSSLINE 1
#define  GT_INLINE 2
#define  GT_DEPTH 3

//normalization type
#define NORM_TO_MAX 0
#define NORM_TO_CENTER 1

//algorithms
#define ALG_SIMPLE_DTA 0
#define ALG_COMPLEX_DTA 1
#define ALG_GAMMA 2

gint algorithm = ALG_COMPLEX_DTA;//for now - this should be able to choose in options window

GtkWidget *main_window;
GtkWidget *main_box; //menu + main_hbox
GtkWidget *main_hbox;

//menu
GtkWidget *main_menu_bar;
GtkWidget *menu_item_file;
GtkWidget *menu_item_monaco;
GtkWidget *menu_item_omnipro;

GtkWidget *menu_file;
GtkWidget *menu_item_options;
GtkWidget *menu_item_report;
GtkWidget *menu_item_about;
GtkWidget *menu_item_exit;

GtkWidget *menu_monaco;
GtkWidget *menu_item_open_monaco_plane;
GtkWidget *menu_item_save_monaco_graph;
GtkWidget *menu_item_open_omnipro_imrt_plane;
GtkWidget *menu_item_2d_normalize_to_max;
GtkWidget *menu_item_2d_normalize_to_center;
GtkWidget *menu_item_2d_clear;

GtkWidget *menu_omnipro;
GtkWidget *menu_item_open_omnipro_accept;
GtkWidget *menu_item_open_1d_from_csv;
GtkWidget *menu_item_1d_normalize_to_max;
GtkWidget *menu_item_1d_normalize_to_center;
GtkWidget *menu_item_1d_clear;
//end of menu

GtkWidget *hhbox;


GtkWidget *monaco_frame;
GtkWidget *monaco_vbox_menu;


GtkWidget *m_2d_box;
  GtkWidget *m_2d_la;
  GtkWidget *m_2d_da;
GtkWidget *m_grid;
  GtkWidget *monaco_la_1;
  GtkWidget *monaco_ed_1;
  GtkWidget *monaco_bu_1;
  GtkWidget *monaco_la_2;
  GtkWidget *monaco_ed_2;
  GtkWidget *monaco_bu_2;
GtkWidget *monaco_la_3;
GtkWidget *monaco_la_4;
GtkWidget *monaco_la_5;
GtkWidget *monaco_rb_1;
GtkWidget *monaco_rb_2;
GtkWidget *monaco_rb_3;
GtkWidget *monaco_da;


GtkWidget *omnipro_frame;
GtkWidget *omnipro_hbox;
GtkWidget *omnipro_vbox_menu;

GtkWidget *omnipro_la_1;
GtkWidget *omnipro_combo_box_text;
GtkWidget *omnipro_button;
GtkWidget *omnipro_la_2;
GtkWidget *omnipro_la_3;
GtkWidget *omnipro_la_4;
GtkWidget *omnipro_da;

GtkWidget *width_frame;
GtkWidget *width_grid;
  GtkWidget *width_la_1;
  GtkWidget *width_ed_1;
  GtkWidget *width_la_2;
  GtkWidget *width_la_3;
  GtkWidget *width_la_4;
  GtkWidget *width_ed_2;
  GtkWidget *width_ed_3;
  GtkWidget *width_bu_1;
  GtkWidget *width_la_5;
  GtkWidget *width_ed_4;
  GtkWidget *width_ed_5;


GtkWidget *compare_vbox; //compare_hbox + compare_da
GtkWidget *compare_hbox; //compare_button + compare_rb_1 + compare_rb_2
GtkWidget *compare_button;
GtkWidget *compare_rb_1;//1vs2
GtkWidget *compare_rb_2;//2vs1

GtkWidget *compare_da;
GtkWidget *msg_bar;
GtkWidget *msg_label;

guint64 n_of_x, n_of_y;
guint64 monaco_row_val, monaco_column_val;
gdouble monaco_step = 0; // in cm
gdouble omnipro_step = 0;

gdouble* plane = NULL; //table is organized as text in the book (don't have any better idea about it)
gchar **lines_splitted = NULL;
gchar **csv_splitted = NULL;

guint n_of_lines;
guint n_of_csv_lines;
guint n_of_csv_sets;

// global graphs
graph monaco_graph;
graph omnipro_graph;
gboolean are_calculations_current = FALSE; // this indicates if we should draw compare_da

// Arrays with point structures (most of them)
//GArray *monaco_garray = NULL; 
//GArray *omnipro_points_garray = NULL; // with points
GArray *omnipro_sets_garray = NULL; // with omnipro_datasets
//GArray *checked_garray_trimmed = NULL; // this it the array we want to compare, filled with points
//GArray *the_other_garray = NULL; // this is the array we want to compare to, filled with points

// FUNCTIONS //

/* 2D manipulating functions defined in monaco_functions.c */
void open_monaco_plane_clicked(  );
void save_monaco_graph_clicked(  );
void get_monaco_row_clicked(  );
void get_monaco_column_clicked(  );
void open_omnipro_imrt_plane_clicked(  );

void draw_background( graph*_g, cairo_t *_cr, guint _width, guint _height, guint _x, guint _y );
void draw_graph( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y );
void draw_dots ( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y );
void draw_fwhm_txt ( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y, char*_txt );
void draw_fwhm_line ( graph*_g, cairo_t *_cr, guint _w, guint _h, guint _x, guint _y );

static void begin_print( GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data );
static void draw_page( GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, gpointer user_data );
static void end_print (GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data );
void report_dialog( GtkMenuItem *_mi, gpointer _data );

gboolean load_options();
void options_dialog( GtkMenuItem *_mi, gpointer _data );
void about_dialog( GtkMenuItem *_mi, gpointer _data );
void dialog_response (GtkDialog *_dialog, gint _id, gpointer user_data);
gboolean difference_changed( GtkWidget *_widget, GdkEvent  *_event, gpointer   user_data );
gboolean dta_changed( GtkWidget *_widget, GdkEvent  *_event, gpointer   user_data );
void color1_changed( GtkColorButton *_widget, gpointer   user_data );
void color2_changed( GtkColorButton *_widget, gpointer   user_data );

gboolean compare_doses( gdouble _d1, gdouble _d2, gdouble _sensitivity );
gdouble abs1( gdouble );
void trim_garray( GArray* _g, gdouble _min, gdouble _max);
gint get_number_of_good_points_in_graph( graph*_g );
gint get_number_of_checked_points_in_graph( graph*_g );
void reset_graph_calculations( graph*_g );
void invert_graph( graph*_g );
point* find_point_in_garray( GArray* _g, gdouble _x, gdouble _s );//we give x value and function
                                                          // returns dose for given x from _g, _s is sensitivity
gdouble check_dta( GArray* , point* ); //we check the point 
                                                   //from _checked_garray_trimmed against _second garray points,
                                                   //according to defined agreements
gdouble min_common_x( graph*_g1, graph*_g2 );
gdouble max_common_x( graph*_g1, graph*_g2 );
void calculate_fwhm( graph*_g );
gdouble get_step_of_garray( GArray* );
void normalize_graph(  graph, guint _norm_type );
gdouble max_x_from_garray( GArray* );
gdouble max_dose_from_garray( GArray* );
gdouble min_x_from_garray( GArray* );
static void msg( const gchar * );
gboolean is_it_number( const gchar * );

void open_omnipro_accept_clicked(  );
void open_1d_from_csv_clicked(  );
void get_omnipro_dataset_clicked(  );
void compare_button_clicked(  );
void menu_item_2d_clear_clicked();
void menu_item_1d_clear_clicked();
void calculate_width_clicked();

void monaco_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data );
void omnipro_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data );
void compare_da_draw( GtkWidget *_widget, cairo_t *_cr, gpointer _data );

gboolean compare_da_press( GtkWidget *_widget, GdkEvent *_event, gpointer user_data );

void main_quit(  );

#endif /* MAIN_WINDOW_H */
