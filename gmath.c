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

  color ambient = calculate_ambient(alight, areflect);
  color diffuse = calculate_diffuse(light, dreflect, normal);
  color specular = calculate_specular(light, sreflect, view, normal);

  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;

  limit_color(&i);
  //????
  //shit
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = areflect[RED] * alight.red;
  a.green = areflect[GREEN] * alight.green;
  a.blue = areflect[BLUE] * alight.blue;

  limit_color(&a);

  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;

  double cosine = dot_product(normal, light[LOCATION]);

  d.red = cosine * dreflect[RED] * light[COLOR][RED];
  d.green = cosine * dreflect[GREEN] * light[COLOR][GREEN];
  d.blue = cosine * dreflect[BLUE] * light[COLOR][BLUE];

  limit_color(&d);

  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {

  color s;
  s.red = 0;
  s.green = 0;
  s.blue = 0;

  double cosine = dot_product(normal, light[LOCATION]);
  double hold[3];
  
  hold[0] = 2 * cosine * normal[0] - light[LOCATION][0];
  hold[1] = 2 * cosine * normal[1] - light[LOCATION][1];
  hold[2] = 2 * cosine * normal[2] - light[LOCATION][2];

  double argument = dot_product(hold, view);
  
  if(argument <= 0){
    return s;
  }
  double resultant = pow(argument, 16);
  
  s.red = resultant * light[COLOR][RED] * sreflect[RED];
  s.green = resultant * light[COLOR][GREEN] * sreflect[GREEN];
  s.blue = resultant * light[COLOR][BLUE] * sreflect[BLUE];

  limit_color(&s);

  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255: c-> blue;

  //im so happy i used ternary operator

}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude = 0.0;
  int i;
  
  for(i = 0; i < 3; i++){
    magnitude += vector[i] * vector[i];
  }
  magnitude = sqrt(magnitude);
  
  int j;
  for(j = 0; j < 3; j++){
    vector[j] = vector[j] / magnitude;
  }

}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  int i;
  double dot = 0.0;

  for(i = 0; i < 3; i++){
    dot += a[i] * b[i];
  }
  return dot;
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





















