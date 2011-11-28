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

