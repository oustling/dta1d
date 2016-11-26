//-------------------------------------------------------------------//
// Helper functions that operates on graphs
//-------------------------------------------------------------------//

gdouble min_common_x( graph*_g1, graph*_g2 ){
    return MAX( min_x_from_garray( _g1->g ), min_x_from_garray( _g2->g ) );
}


gdouble max_common_x( graph*_g1, graph*_g2 ){
    return MIN( max_x_from_garray( _g1->g ), max_x_from_garray( _g2->g ) );
}

gint round1 (gdouble _a){
    return (gint)(_a < 0 ? (_a - 0.5) : (_a + 0.5));
}

//-------------------------------------------------------------------//
// Function counts number of good points in graph _g
//-------------------------------------------------------------------//
gint get_number_of_good_points_in_graph( graph*_g )
{
  if( _g == 0 )return 0;
  if( _g->g == 0 )return 0;
  if( _g->g->len == 0 )return 0;
  gint _i;
  gint _j = 0;

  for( _i=0; _i<=_g->g->len; _i++ )
  {
    if( g_array_index( _g->g, point, _i ).result == 0 ) _j++;
  }
  return _j;
}

//-------------------------------------------------------------------//
// Function counts number of checked points in graph _g,
// some of points may be not checked -they have result=-1
//-------------------------------------------------------------------//
gint get_number_of_checked_points_in_graph( graph*_g )
{
  if( _g == 0 )return 0;
  if( _g->g == 0 )return 0;
  if( _g->g->len == 0 )return 0;
  gint _i;
  gint _j = 0;

  for( _i=0; _i<=_g->g->len; _i++ )
  {
    if( g_array_index( _g->g, point, _i ).result >= 0 ) _j++;
  }
  return _j;
}

//-------------------------------------------------------------------//
// Function sets calculation result to -1 for all points in graph _g
//-------------------------------------------------------------------//
void reset_graph_calculations( graph*_g )
{
  if( _g == 0 )return;
  if( _g->g == 0 )return;
  if( _g->g->len == 0 )return;
  gint _i;
  for( _i=0; _i<_g->g->len; _i++ )
  {
    g_array_index( _g->g, point, _i ).result = -1;
    if( g_array_index( _g->g, point, _i ).desc )
    {
      g_free(g_array_index( _g->g, point, _i ).desc );
      g_array_index( _g->g, point, _i ).desc = NULL;
    }
  }
  _g->first_50 = 0;
  _g->second_50 = 0;
  _g->type = GT_UNDEFINED;
}

//-------------------------------------------------------------------//
// Inverts garray in graph
//-------------------------------------------------------------------//
void invert_graph( graph*_g )
{
  if( _g == 0 )return;
  if( _g->g == 0 )return;
  if( _g->g->len == 0 )return;
  guint _n = _g->g->len;
  guint _i;
  GArray*_new;
  point _p; 

  _new = g_array_new (TRUE, TRUE, sizeof (point));

  for( _i=0; _i<_n; _i++)
  {
    _p.x = g_array_index( _g->g, point, _i ).x;
    _p.dose = g_array_index( _g->g, point, _i ).dose;
    _p.desc = NULL;
    _p.result = -1;
    g_array_prepend_val( _new, _p );
  }
  reset_graph_calculations( _g );
  g_array_free( _g->g, TRUE );
  _g->g = _new;
}


