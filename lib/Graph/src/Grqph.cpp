#include"Graph.h"
#define GRID 1
#define TICKS 2
#define GRIDandTICKS GRID|TICKS

uint8_t Graph::Legend::maxid = 0;

Graph::Graph(const String &title,const int16_t xlim_left,const int16_t xlim_right,const int16_t ylim_top,const int16_t ylim_bottom,const uint16_t xticks,const uint16_t yticks,bool write_grid,bool wirte_ticks){
    
    _xyparam.title = title;
    
    _xyparam.xlim_left = xlim_left;
    _xyparam.xlim_right = xlim_right;
    
    _xyparam.ylim_top = ylim_top;
    _xyparam.ylim_bottom = ylim_bottom;

    _grid.xticks = xticks;
    _grid.yticks = yticks;
    
    _xyparam.ylabel = "no ylabel";
    
    _xyparam.xlabel = "no xlabel";


    drawFrame();
    if(write_grid&&wirte_ticks){
        drawXFrame(GRIDandTICKS);
        drawYFrame(GRIDandTICKS);
    }else if(wirte_ticks){
        drawXFrame(TICKS);
        drawYFrame(TICKS);
    }else if(write_grid){
        drawXFrame(GRID);
        drawYFrame(GRID);
    }

    drawXLabel(_xyparam.xlabel);
    drawYLabel(_xyparam.ylabel);
    
    plot(0,0,BLUE);
    plot(10,10,BLUE);
    

    //plot(x,y,3,BLUE);
    scatter(50,50,YELLOW);
    
}

void Graph::xlim(const int16_t left,const int16_t right){
    _xyparam.xlim_left = left;
    _xyparam.xlim_right = right;
}

void Graph::ylim(const int16_t top,const int16_t bottom){
    _xyparam.ylim_top = top;
    _xyparam.ylim_bottom = bottom;
}

void Graph::plot(const int x,const int y,const int16_t color,const String &legend_name){ 
    auto xunit_pixel_par_data = (float)abs(_gframe.right-_gframe.left)/abs(_xyparam.xlim_right-_xyparam.xlim_left);
    auto yunit_pixel_par_data = (float)abs(_gframe.top-_gframe.bottom)/abs(_xyparam.ylim_top-_xyparam.ylim_bottom);
    
    if(x<_xyparam.xlim_left||_xyparam.xlim_right<x||
       y<_xyparam.ylim_bottom||_xyparam.ylim_top<y) return;

    if (_legend_plot[color].id==0){
        _legend_plot[color].maxid+=1;
        _legend_plot[color].legend_ypos = _gframe.top +10*_legend_plot[color].maxid;
        _legend_plot[color].before_x = _xyparam.xlim_left-1;
        _legend_plot[color].before_y = _xyparam.ylim_bottom-1;
        _legend_plot[color].name = legend_name;
        _legend_plot[color].id = _legend_plot[color].maxid;
    }
    
    {
        int16_t before_x = _legend_plot[color].before_x;
        int16_t before_y = _legend_plot[color].before_y;
        if(_xyparam.xlim_left <= before_x && before_x <=_xyparam.xlim_right &&
        _xyparam.ylim_bottom <= before_y && before_y <= _xyparam.ylim_top){
            uint16_t xpos_before = _gframe.left + xunit_pixel_par_data * before_x;
            uint16_t xpos_now = _gframe.left + xunit_pixel_par_data * x;
            uint16_t ypos_before = _gframe.bottom - yunit_pixel_par_data * before_y;
            uint16_t ypos_now = _gframe.bottom-yunit_pixel_par_data * y;
            M5.Lcd.drawLine(xpos_before, ypos_before, xpos_now,ypos_now,color);
        }
    }

    drawLegendPlot(color);

    _legend_plot[color].before_x = x;
    _legend_plot[color].before_y = y;
}

void Graph::plot(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color,const String &legend_name){
    for(auto i=0;i<xy_bufsize;i++){
        plot(*x,*y,color,legend_name);
        x++;
        y++;
    }
}

void Graph::scatter(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color,const String &legend_name){
    for(auto i=0;i<xy_bufsize;i++){
        scatter(*x,*y,color,legend_name);
        x++;
        y++;
    }
}

void Graph::scatter(const int x,const int y,const int16_t color,const String &legend_name){
    if(x<_xyparam.xlim_left||_xyparam.xlim_right<x||
       y<_xyparam.ylim_bottom||_xyparam.ylim_top<y) return;

    if (_legend_scatter[color].id==0){
        _legend_scatter[color].maxid+=1;
        _legend_scatter[color].legend_ypos = _gframe.top +10*_legend_scatter[color].maxid;
        _legend_scatter[color].before_x = _xyparam.xlim_left-1;
        _legend_scatter[color].before_y = _xyparam.ylim_bottom-1;
        _legend_scatter[color].name = legend_name;
        _legend_scatter[color].id = _legend_scatter[color].maxid;
    }

    uint16_t xpos = _gframe.left+(float)abs(_gframe.right-_gframe.left)/abs(_xyparam.xlim_right-_xyparam.xlim_left)*x;
    uint16_t ypos = _gframe.bottom-(float)abs(_gframe.top-_gframe.bottom)/abs(_xyparam.ylim_top-_xyparam.ylim_bottom)*y;

    M5.Lcd.fillCircle(xpos,ypos,5,color);

    drawLegendScatter(color);

    _legend_scatter[color].before_x = x;
    _legend_scatter[color].before_y = y;
}

void Graph::drawFrame(){
    const auto xl = _gframe.left;
    const auto xr = _gframe.right;
    const auto yb = _gframe.bottom;
    const auto yt = _gframe.top;
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.drawLine(xl, yb, xr, yb, WHITE); //bottom
    M5.Lcd.drawLine(xl, yt, xr, yt, WHITE); //top
    M5.Lcd.drawLine(xl, yb, xl, yt, WHITE); //left
    M5.Lcd.drawLine(xr, yb, xr, yt, WHITE); //right
}


void Graph::drawXFrame(const uint8_t mode){
    auto xwidth = (float)abs(_gframe.right-_gframe.left)/abs(_xyparam.xlim_right-_xyparam.xlim_left);
    int16_t xliml = _xyparam.xlim_left;
    int16_t xlimr = _xyparam.xlim_right;
    int16_t xticks = _grid.xticks;
    int16_t x = xliml;
    float xpos = _gframe.left;

    for(xpos = _gframe.left;xpos<=_gframe.right;xpos+=xwidth*xticks){
     
        if(GRID){
            M5.Lcd.drawLine(xpos, _gframe.bottom, xpos,_gframe.top, WHITE);
        }

        if(TICKS>>1){
            M5.Lcd.drawString(String(x),xpos,_gframe.bottom+1);    
        }
        x += xticks;     
    }

    if(TICKS>>1) M5.Lcd.drawString(String(xlimr),_gframe.right,_gframe.bottom+1);
}

void Graph::drawYFrame(const uint8_t mode){
    auto ysize = (float)abs(_gframe.top-_gframe.bottom)/abs(_xyparam.ylim_top-_xyparam.ylim_bottom);
    int16_t ylimb = _xyparam.ylim_bottom;
    int16_t ylimt = _xyparam.ylim_top;
    int16_t yticks = _grid.yticks;
    int16_t y = ylimb;
    float ypos =_gframe.bottom;

    for(ypos = _gframe.bottom;ypos>=_gframe.top;ypos-=ysize*yticks){
        if(GRID){
            M5.Lcd.drawLine(_gframe.left,ypos, _gframe.right,ypos, WHITE);      
        }

        if(TICKS>>1){
            M5.Lcd.drawString(String(y),_gframe.left-20,ypos);
        }

        y+=yticks;
    }
    
    if(TICKS>>1) M5.Lcd.drawString(String(ylimt),_gframe.left-20,_gframe.top);
}

void Graph::drawXLabel(const String& xlabel){
    _xyparam.xlabel = xlabel;
    String str = _xyparam.xlabel;
    uint16_t xpos = (_gframe.right+_gframe.left)/2;
    uint16_t ypos = _gframe.bottom+15;

    M5.Lcd.drawCentreString(String("               "),xpos,ypos,1);
    M5.Lcd.drawCentreString(str,xpos,ypos,1);
}

void Graph::drawYLabel(const String& ylabel){
    _xyparam.ylabel = ylabel;
    String str = _xyparam.ylabel;
    uint16_t xpos = 0;
    uint16_t ypos = 40;

    for(auto s:_xyparam.ylabel){
        M5.Lcd.drawChar(s,xpos,ypos);
        ypos+=10;
    }
    
}

void Graph::drawLegendScatter(const uint16_t color){
    M5.Lcd.setTextColor(color);
    const auto xpos = _gframe.right-10;
    const auto ypos = _legend_scatter[color].legend_ypos;
    const auto name = _legend_scatter[color].name;
    const auto w = name.length()*8;
    const auto h = 15;
    M5.Lcd.fillRect(xpos-w,ypos+3,w,h,BLACK);
    M5.Lcd.drawRightString(name,xpos,ypos,2);
    M5.Lcd.setTextColor(WHITE);
}

void Graph::drawLegendPlot(const uint16_t color){
    M5.Lcd.setTextColor(color);
    const auto xpos = _gframe.right-10;
    const auto ypos = _legend_plot[color].legend_ypos;
    const auto name = _legend_plot[color].name;
    const auto w = name.length()*8;
    const auto h = 15;
    M5.Lcd.fillRect(xpos-w,ypos+3,w,h,BLACK);
    M5.Lcd.drawRightString(name,xpos,ypos,2);
    M5.Lcd.setTextColor(WHITE);
}
  