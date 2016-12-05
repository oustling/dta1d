//-------------------------------------------------------------------//
// we assume that _g is dose profile graph = there are two points that
// have dose _height dose, these points are saved to _g->first_50 and
// second_50, 
//-------------------------------------------------------------------//
gboolean calculate_width( graph*_g, gdouble _height )
{
  if( _g == 0 )return FALSE;
  if( _g->g == 0 )return FALSE;
  if( _g->g->len == 0 )return FALSE;
  if( _g->type == GT_DEPTH )return FALSE;
  point _p1,_p2;//1, _p2, _p3, _p4;
  point _p;
  guint _i = 1; // we start checking from second point
 // guint _counter = 0;

  GArray*_tg;
  _tg = g_array_new (TRUE, TRUE, sizeof (point));

  for( ;; )
  {
    if( g_array_index( _g->g, point, _i ).dose >= _height && g_array_index( _g->g, point, _i-1 ).dose < _height )
    {
      _p2.x    = g_array_index( _g->g, point, _i ).x;
      _p2.dose = g_array_index( _g->g, point, _i ).dose;
      _p1.x    = g_array_index( _g->g, point, _i-1 ).x;
      _p1.dose = g_array_index( _g->g, point, _i-1 ).dose;
      _p.x = ( _height - _p1.dose )*( _p2.x - _p1.x )/( _p2.dose - _p1.dose ) + _p1.x;
      _p.dose = _height;
      g_array_append_val( _tg, _p );

    }
    else if( g_array_index( _g->g, point, _i ).dose < _height && g_array_index( _g->g, point, _i-1 ).dose >= _height )
    {
        _p2.x    = g_array_index( _g->g, point, _i ).x;
        _p2.dose = g_array_index( _g->g, point, _i ).dose;
        _p1.x    = g_array_index( _g->g, point, _i-1 ).x;
        _p1.dose = g_array_index( _g->g, point, _i-1 ).dose;
        
        _p.x = ( _p2.dose - _height )*( _p2.x - _p1.x )/( _p1.dose - _p2.dose ) + _p2.x;
        _p.dose = _height;
      g_array_append_val( _tg, _p );
    }
    if( _g->g->len == _i )break; //end of _g->g
    _i++;
  }
  //    printf("%d ", _tg->len);
  if( _tg->len != 2 )
  {
    _g->first_50  = 0;
    _g->second_50 = 0; 
    g_array_free ( _tg, TRUE );
    return FALSE;
  }
//    printf("x %f, dose %f\n", g_array_index( _tg, point, 0 ).x,g_array_index( _tg, point, 0 ).dose);
//    printf("x %f, dose %f\n", g_array_index( _tg, point, 1 ).x,g_array_index( _tg, point, 1 ).dose);
  _g->first_50  =  g_array_index( _tg, point, 0 ).x;
  _g->second_50 = g_array_index( _tg, point, 1 ).x; 
  g_array_free ( _tg, TRUE );
  return TRUE;
}





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
// it cannot change graph.type
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


