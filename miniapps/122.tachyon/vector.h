/*
 * vector.h - This file contains declarations of vector functions
 *
 *  $Id: vector.h 499 2006-10-06 19:58:28Z keeper $
 */

flt VDot(const vector *, const vector *);
void VCross(const vector *, const vector *, vector *);
flt VLength(const vector *);
void VNorm(vector *);
void VAdd(const vector *, const vector *, vector *);
void VSub(const vector *, const vector *, vector *);
void VAddS(flt, const vector *, const vector *, vector *);
vector Raypnt(const ray *, flt);
void VScale(vector * a, flt s); 

void ColorAddS(color * a, const color * b, flt s); 
void ColorAccum(color * a, const color * b); 
void ColorScale(color * a, flt s); 

