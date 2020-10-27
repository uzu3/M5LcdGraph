#ifndef _GRAPH_H_
#define _GRAPH_H_
  
#include<M5Stack.h>
#include<Arduino.h>
#include<map>
struct GridParam{
  uint8_t line_width;
  uint16_t xticks;
  uint16_t yticks;
  uint16_t back_ground_color;
  uint16_t grid_color;
  String legend_position; //"inner" or "outer"
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
  uint16_t left;
  uint16_t right;
  uint16_t bottom;
  uint16_t top;
};

class Graph{
  struct Legend{
    uint8_t id;
    uint16_t legend_ypos;
    int16_t before_x;
    int16_t before_y;
    String name;
    uint8_t width;
    static uint8_t maxid;
  };

  GraphXYParam _xyparam;
  GraphFrame _gframe;
  GridParam _grid;
  std::map<uint16_t,Legend> _legend_plot;
  std::map<uint16_t,Legend> _legend_scatter;
  
  void drawFrame();
  void drawXFrame(const uint8_t mode); // 1:Grid, 2: Ticks 3: both
  void drawYFrame(const uint8_t mode); // 1:Grid, 2: Ticks 3: both
  void drawXLabel(const String& xlabel);
  void drawYLabel(const String& ylabel);

  void xlim(const int16_t left,const int16_t right);
  void ylim(const int16_t bottom,const int16_t top);

  void drawLegendScatter(const uint16_t color);
  void drawLegendPlot(const uint16_t color);
  void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color,uint8_t linewidth=1);

  public:
  Graph(GraphXYParam gxyp,GraphFrame gf);
  Graph(const String &title="no title",
        const int16_t xlim_left=0,
        const int16_t xlim_right=100,
        const int16_t ylim_top=100,
        const int16_t ylim_bottom=0,
        const uint16_t xticks=10,
        const uint16_t yticks=10,
        const uint16_t grid_width = 1,
        bool write_grid=true,
        bool wirte_ticks=true
        );
  
  ~Graph()=default;

  void plot(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color=RED,const String &legend_name="no name",const uint8_t linewidth=1);
  void plot(const int x,const int y,const int16_t color=RED,const String &legend_name="no name",const uint8_t linewidth=1);
  
  void scatter(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color=RED,const String &legend_name="no name",const uint8_t radius=2);
  void scatter(const int x,const int y,const int16_t color=RED,const String &legend_name="no name",const uint8_t radius=2);
  
};

#endif
