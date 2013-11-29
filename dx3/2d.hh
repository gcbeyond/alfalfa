#ifndef TWOD_HH
#define TWOD_HH

#include <cassert>
#include <vector>

#include "optional.hh"

/* simple two-dimensional container */
template< class T >
class TwoD
{
private:
  std::vector< std::vector< T > > storage_;

public:
  struct Context
  {
    const unsigned int row, column;
    const Optional< T * > above, left;
  };

  template< typename... Targs >
  TwoD( const unsigned int width, const unsigned int height, Targs&&... Fargs )
    : storage_()
  {
    assert( width > 0 );
    assert( height > 0 );

    /* we need to construct each member separately */
    storage_.reserve( height );
    for ( unsigned int i = 0; i < height; i++ ) {
      std::vector< T > row;
      row.reserve( width );
      for ( unsigned int j = 0; j < width; j++ ) {
	const Optional< T * > above( i > 0 ? &storage_.at( i - 1 ).at( j ) : Optional< T * >() );
	const Optional< T * > left ( j > 0 ? &row.at( j - 1 ) : Optional< T * >() );
	Context c { i, j, above, left };
	row.emplace_back( c, Fargs... );
      }
      storage_.emplace_back( move( row ) );
    }
  }

  T & at( const uint16_t x, const uint16_t y )
  {
    return storage_.at( y ).at( x );
  }

  uint16_t width( void ) const { return storage_.at( 0 ).size(); }
  uint16_t height( void ) const { return storage_.size(); }
};

#endif /* TWOD_HH */
