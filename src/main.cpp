#include <Arduino.h>
#include "Graph.h"
void setup() {
  // put your setup code here, to run once:
   M5.begin();
  auto k = Graph();
  k.plot(10,10,RED,"hoge");       //Add Plot-legend-name is hoge. 
  k.plot(0,0,RED,"huu");          //Write line: legend-name is hoge. 
  k.plot(40,40,RED,"hoo");        // Write line: legend-name is hoge.
  k.scatter(40,40,RED,"hoo");     //Add Scatter-legend:hoge
  k.scatter(10,50,YELLOW,"hog");  //Add  Scatter-legend:hog
}

void loop() {
  // put your main code here, to run repeatedly:
}