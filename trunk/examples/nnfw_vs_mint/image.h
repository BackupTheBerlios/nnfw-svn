/* $Id: image.h,v 1.3 2005/02/19 09:47:17 stefano Exp $ */

#ifndef MINT_IMAGE_H
#define MINT_IMAGE_H

struct mint_image;

enum mint_image_setting { 
  side = 0, 
  fontsize = 1, 
  border = 2, 
  indices = 3,
  scale = 4,
  normalize = 8
};

struct mint_image *mint_image_new( void );

void mint_image_del( struct mint_image * );

void mint_image_set( struct mint_image *, 
		     enum mint_image_setting, int );

void mint_image_set_font( struct mint_image *, 
			  const char *font );

void mint_image_eps1( const struct mint_image *, const float *ary, 
		      int r, int c, const char *filename );

void mint_image_eps2( const struct mint_image *, const float **ary, 
		      int r, int c, const char *filename );

#endif
