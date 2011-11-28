#ifndef DATA_H
#define DATA_H
#include "opengl.h"
#include "timestep.h"
#include <QString>
#include <stdlib.h>
#include <QByteArray>
#include <QFile>

std::vector<Timestep> timesteps;
void readTimestep(int i, QFile file) {

}

void readFile(QString filename) {
  QFile file(filename);
  QByteArray line;
  int elements_timestep;
  int timesteps;
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return -1;

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
        if(sscanf(line.data(),"%f %f %f %f", &tx, &ty, &tz, &tmass)==-1) {
          qDebug() << "Error: " << line;
          exit(EXIT_FAILURE);
        }
        timestamps.back().put(i,x,y,z,w);
      } else {
        qDebug() << "File has wrong format...";
      }

    }
  }
  // Go to beginning again
  file.seek(0);
  if (nlines<=3) {
    qDebug() << "File only has"<< nlines << "lines";
    exit(EXIT_FAILURE);
  }
  if (!file.atEnd()) {
    line = file.readLine();
    if (sscanf(line.data(),"%d",&elements_timestep)== -1) {
      qDebug() << "File has wrong format";
      exit(EXIT_FAILURE);
    }
    timesteps = nlines/(elements_timestep+3);
    steppings = (time)
  }
}
#endif

