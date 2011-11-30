// This file belongs to the OpenGL xyzw Dataviewer located at: 
//     https://github.com/moeeeep/opengl_dataviewer 
// Copyright (C) 2011  Pascal Spörri <pascal.spoerri@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <QDebug>
#include "opengl.h"
#include "datatypes.h"
class Timestep { // VERY BAD CODE
public:
  float4* pos;
  float* w;
  int elements;
  float maxW;
  explicit Timestep(int elements) {
    pos = (float4*) malloc(elements*sizeof(float4));
    w = (float*) malloc(elements*sizeof(float));
    this->elements = elements;
    maxW = 1.0;
  }

  void put(int i,float x, float y, float z, float w) {
    if (i>elements) {
      qDebug() << "Index "<< i << " is not in range";
      exit(EXIT_FAILURE);
    }
    pos[i].x = x;
    pos[i].y = y;
    pos[i].z = z;
    pos[i].w = w;
    this->w[i] = w;
    if (w > maxW)
      maxW = w;
  }
};

#endif
