#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color amb = calculate_ambient(alight, areflect);
  color diff = calculate_diffuse(light, dreflect, normal);
  color spec = calculate_specular(light, sreflect, view, normal);
  i.red = amb.red + diff.red + spec.red;
  i.green = amb.green + diff.green + spec.green;
  i.blue = amb.blue + diff.blue + spec.blue;
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * *areflect;
  a.green = alight.green * *areflect;
  a.blue = alight.blue * *areflect;
  limit_color(&a);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  // double x = light[0][0];
  // double y = light[0][1];
  // double z = light[0][2];
  d.red = light[1][0] * *dreflect * dot_product(normal, light[0]);
  d.green = light[1][1] * *dreflect * dot_product(normal, light[0]);
  d.blue = light[1][2] * *dreflect * dot_product(normal, light[0]);;
  limit_color(&d);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  s.red = 0;
  s.green = 0;
  s.blue = 0;
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255 : c->blue;
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
