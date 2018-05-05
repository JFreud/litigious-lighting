#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);
  color amb = calculate_ambient(alight, areflect);
  color diff = calculate_diffuse(light, dreflect, normal);
  color spec = calculate_specular(light, sreflect, view, normal);
  i.red = amb.red + diff.red + spec.red;
  i.green = amb.green + diff.green + spec.green;
  i.blue = amb.blue + diff.blue + spec.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  limit_color(&a);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  // double x = light[0][0];
  // double y = light[0][1];
  // double z = light[0][2];
  double dotp = dot_product(normal, light[LOCATION]);
  d.red = light[COLOR][RED] * dreflect[RED] * dotp;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * dotp;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dotp;
  limit_color(&d);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  // PKs([2(N • L)N - L] • V)^x
  color s;
  s.red = 0;
  s.green = 0;
  s.blue = 0;
  //2(N • L):
  double nl = 2 * dot_product(normal, light[LOCATION]);
  double arr[3];
  arr[0] = nl * normal[0] - light[LOCATION][0];
  arr[1] = nl * normal[1] - light[LOCATION][1];
  arr[2] = nl * normal[2] - light[LOCATION][2];
  double dotp = dot_product(arr, view);
  if (dotp <= 0) {
    return s;
  }
  double result = pow(dotp, 16);
  s.red = light[COLOR][RED] * sreflect[RED] * result;
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * result;
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * result;
  limit_color(&s);
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255 : c->blue;
  c->red = c->red < 0 ? 0 : c->red;
  c->green = c->green < 0 ? 0 : c->green;
  c->blue = c->blue < 0 ? 0 : c->blue;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double mag = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  vector[0] /= mag;
  vector[1] /= mag;
  vector[2] /= mag;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  double dp = 0;
  int i = 0;
  for (; i < 3; i++) {
    dp += a[i] * b[i];
  }
  return dp;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
