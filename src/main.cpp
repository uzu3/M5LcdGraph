#include <Arduino.h>
#include "Graph.h"
void setup() {
  // put your setup code here, to run once:
   M5.begin();
  auto k = Graph();
  k.plot(10,10,RED,"hoge",3);       //Add Plot-legend-name is hoge. Line-width = 3. 
  k.plot(0,0,RED,"huu");          //Write line: legend-name is hoge. Line-width = 3.
  k.plot(100,50,RED,"hoo");        // Write line: legend-name is hoge. Line-width = 3.
  k.scatter(10,90,RED,"hoo",10);     //Add Scatter-legend:hoge. Radius = 3.
  k.scatter(10,50,YELLOW,"hog");  //Add  Scatter-legend:hog. Radius = 3.
}

void loop() {
  // put your main code here, to run repeatedly:
}