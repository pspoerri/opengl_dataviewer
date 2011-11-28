#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <QDebug>
#include "opengl.h"
class Timestep { // VERY BAD CODE
  float4* data;
  int elements;
  explicit Timestep(size elements) {
    data = (float4*) malloc(elements*sizeof(float4));
    this->elements = elements;
  }

  void put(int i,float x, float y, float z, float w) {
    if (i>elements) {
      qDebug() << "Index "<< i << " is not in range";
      exit(EXIT_FAILURE);
    }
    data[i].x = x;
    data[i].y = y;
    data[i].z = z;
    data[i].w = w;
  }
};

#endif
