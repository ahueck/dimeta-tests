/*
 * render.h - This file contains the defines for the top level functions 
 *
 *  $Id: render.h 499 2006-10-06 19:58:28Z keeper $
 */

void create_render_threads(scenedef * scene);
void destroy_render_threads(scenedef * scene);
void renderscene(scenedef *); 

