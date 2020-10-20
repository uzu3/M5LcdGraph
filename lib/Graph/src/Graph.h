#ifndef _GRAPH_H_
#define _GRAPH_H_
  
#include<M5Stack.h>
#include<Arduino.h>
#include<map>
struct GridParam{
  uint8_t linewidth;
  uint16_t xticks;
  uint16_t yticks;
  uint16_t back_ground_color;
  uint16_t grid_color;
};

struct GraphXYParam{
  String title;
  String xlabel;
  String ylabel;
  int16_t xlim_left;
  int16_t xlim_right;
  int16_t ylim_bottom;
  int16_t ylim_top;
};

struct GraphFrame{
  const uint16_t left = 40;
  const uint16_t right = 300;
  const uint16_t bottom = 200;
  const uint16_t top = 0;
};


class Graph{
  struct Legend{
    uint8_t id;
    uint16_t legend_ypos;
    int16_t before_x;
    int16_t before_y;
    String name;
    static uint8_t maxid;
  };

  GraphXYParam _xyparam;
  GraphFrame _gframe;
  GridParam _grid;
  std::map<uint16_t,Legend> _legend_plot;
  std::map<uint16_t,Legend> _legend_scatter;
  
  void drawFrame();
  void drawXFrame(const uint8_t mode); // 1:Grid, 2: Ticks
  void drawYFrame(const uint8_t mode); // 1:Grid, 2: Ticks
  void drawXLabel(const String& xlabel);
  void drawYLabel(const String& ylabel);

  void xlim(const int16_t left,const int16_t right);
  void ylim(const int16_t bottom,const int16_t top);

  void legend();
  public:
  Graph(GraphXYParam gxyp,GraphFrame gf);
  Graph(const String &title="no title",
        const int16_t xlim_left=0,
        const int16_t xlim_right=100,
        const int16_t ylim_top=100,
        const int16_t ylim_bottom=0,
        const uint16_t xticks=10,
        const uint16_t yticks=10,
        bool write_grid=true,
        bool wirte_ticks=true
        );
  
  ~Graph()=default;

  void plot(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color=RED,const String &legend_name="no name");
  void plot(const int16_t x,const int16_t y,const int16_t color=RED,const String &legend_name="no name");
  
  void scatter(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color=RED,const String &legend_name="no name");
  void scatter(const int16_t x,const int16_t y,const int16_t color=RED,const String &legend_name="no name");
  
};

#endif