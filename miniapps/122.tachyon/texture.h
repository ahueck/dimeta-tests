/*
 * texture.h This file contains all of the includes and defines for the texture 
 * mapping part of the shader.
 *
 *  $Id: texture.h 499 2006-10-06 19:58:28Z keeper $
 */

void InitTextures(void);
color     constant_texture(const vector *, const texture *, const ray *);
color    image_cyl_texture(const vector *, const texture *, const ray *);
color image_sphere_texture(const vector *, const texture *, const ray *);
color  image_plane_texture(const vector *, const texture *, const ray *);
color      checker_texture(const vector *, const texture *, const ray *);
color  cyl_checker_texture(const vector *, const texture *, const ray *);
color         grit_texture(const vector *, const texture *, const ray *);
color         wood_texture(const vector *, const texture *, const ray *);
color       marble_texture(const vector *, const texture *, const ray *);
color       gnoise_texture(const vector *, const texture *, const ray *);
int Noise(flt, flt, flt);
void InitTextures(void);
void FreeTextures(void);

texture * new_texture(void);
texture * new_standard_texture(void);
texture * new_vcstri_texture(void);
void free_standard_texture(void * voidtex);

