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

#ifndef DATA_H
#define DATA_H
#include "opengl.h"
#include "timestep.h"
#include <QString>
#include <stdlib.h>
#include <QByteArray>
#include <QFile>

std::vector<Timestep> timesteps;
int max_frame_number = 0;

void readFile(QString filename) {
  QFile file(filename);
  QByteArray line;
  int elements_timestep;
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Couldn't open file";
    exit(EXIT_FAILURE);
  }


  float x,y,z,w;
  // Count the number of lines
  while (!file.atEnd()) {
    line = file.readLine();
    if (sscanf(line.data(),"%d",&elements_timestep)== -1) {
      qDebug() << "File has wrong format";
      exit(EXIT_FAILURE);
    }
    if (!file.atEnd())
      line = file.readLine(); // Ignore this line
    if (!file.atEnd())
      line = file.readLine(); // Ignore this line
    timesteps.push_back(Timestep(elements_timestep));
    for (int i=0;i<elements_timestep;i++) {
      if (!file.atEnd()) {
        line = file.readLine();
        if(sscanf(line.data(),"%f %f %f %f", &x, &y, &z, &w)==-1) {
          qDebug() << "Error: " << line;
          exit(EXIT_FAILURE);
        }
        timesteps.back().put(i,x,y,z,w);
      } else {
        qDebug() << "File has wrong format...";
      }

    }
    max_frame_number++;
  }
}
#endif

