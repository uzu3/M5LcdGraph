#include"Graph.h"
#include<math.h>
#define GRID 1
#define TICKS 2
#define GRIDandTICKS GRID|TICKS

uint8_t Graph::Legend::maxid = 0;

Graph::Graph(const String &title, 
             const int16_t xlim_left, 
             const int16_t xlim_right, 
             const int16_t ylim_top, 
             const int16_t ylim_bottom, 
             const uint16_t xticks, 
             const uint16_t yticks, 
             const uint16_t grid_width, 
             bool write_grid, 
             bool wirte_ticks
             ){ 
    
    _xyparam.title = title;
    
    _xyparam.xlim_left = xlim_left;
    _xyparam.xlim_right = xlim_right;
    
    _xyparam.ylim_top = ylim_top;
    _xyparam.ylim_bottom = ylim_bottom;

    _grid.xticks = xticks;
    _grid.yticks = yticks;
    _grid.back_ground_color = BLACK;
    _grid.grid_color = WHITE;
    _grid.line_width = grid_width;

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
    
}

void Graph::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color,uint8_t linewidth){
    if(linewidth==1){
        M5.Lcd.drawLine(x0,y0,x1,y1,color);
    }else if(y0==y1){
        const int16_t yt = max(0,y0-linewidth/2);
        const int16_t yb = y0+(linewidth+1)/2;
        M5.Lcd.fillTriangle(x0,yt,x0,yb,x1,yt,color);
        M5.Lcd.fillTriangle(x0,yb,x1,yb,x1,yt,color);
    }else if(x0==x1){
        const int16_t xl = x0-linewidth/2;
        const int16_t xr = x0+(linewidth+1)/2;
        M5.Lcd.fillTriangle(xl,y0,xr,y0,xl,y1,color);
        M5.Lcd.fillTriangle(xl,y1,xr,y1,xr,y0,color);
    }else{
        const auto theta = atan2(y0-y1,x0-x1);
        const auto thetaR = theta+PI;
        const auto dist = sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
        const int16_t x11 = dist*cos(thetaR)-sin(thetaR)*(linewidth+1)/2 + x0;
        const int16_t y11 = dist*sin(thetaR)+cos(thetaR)*(linewidth+1)/2 + y0;
        const int16_t x12 = dist*cos(thetaR)+sin(thetaR)*linewidth/2 + x0;
        const int16_t y12 = dist*sin(thetaR)-cos(thetaR)*linewidth/2 + y0;
        const int16_t x01 = dist*cos(theta)-sin(theta)*(linewidth+1)/2 + x1;
        const int16_t y01 = dist*sin(theta)+cos(theta)*(linewidth+1)/2 + y1;
        const int16_t x02 = dist*cos(theta)+sin(theta)*linewidth/2 + x1;
        const int16_t y02 = dist*sin(theta)-cos(theta)*linewidth/2 + y1;
        M5.Lcd.fillTriangle(x01,y01,x11,y11,x12,y12,color);
        M5.Lcd.fillTriangle(x01,y01,x02,y02,x11,y11,color);
    }
        
}
  

void Graph::xlim(const int16_t left,const int16_t right){
    _xyparam.xlim_left = left;
    _xyparam.xlim_right = right;
}

void Graph::ylim(const int16_t top,const int16_t bottom){
    _xyparam.ylim_top = top;
    _xyparam.ylim_bottom = bottom;
}

void Graph::plot(const int x,const int y,const int16_t color,const String &legend_name,uint8_t linewidth){ 
    auto xunit_pixel_par_data = (float)abs(_gframe.right-_gframe.left)/abs(_xyparam.xlim_right-_xyparam.xlim_left);
    auto yunit_pixel_par_data = (float)abs(_gframe.top-_gframe.bottom)/abs(_xyparam.ylim_top-_xyparam.ylim_bottom);
    
    if(x<_xyparam.xlim_left||_xyparam.xlim_right<x||
       y<_xyparam.ylim_bottom||_xyparam.ylim_top<y) return;

    if (_legend_plot[color].id==0){
        _legend_plot[color].maxid+=1;
        _legend_plot[color].legend_ypos = _gframe.top +14*_legend_plot[color].maxid;
        _legend_plot[color].before_x = _xyparam.xlim_left-1;
        _legend_plot[color].before_y = _xyparam.ylim_bottom-1;
        _legend_plot[color].name = legend_name;
        _legend_plot[color].width = linewidth;
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
            uint8_t  lw = _legend_plot[color].width;
            drawLine(xpos_before, ypos_before, xpos_now,ypos_now,color,lw);
        }
    }

    drawLegendPlot(color);

    _legend_plot[color].before_x = x;
    _legend_plot[color].before_y = y;
}

void Graph::plot(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color,const String &legend_name,const uint8_t linewidth){
    for(auto i=0;i<xy_bufsize;i++){
        plot(*x,*y,color,legend_name,linewidth);
        x++;
        y++;
    }
}

void Graph::scatter(const int16_t x[],const int16_t y[],const uint16_t xy_bufsize,const int16_t color,const String &legend_name,const uint8_t radius){
    for(auto i=0;i<xy_bufsize;i++){
        scatter(*x,*y,color,legend_name,radius);
        x++;
        y++;
    }
}

void Graph::scatter(const int x,const int y,const int16_t color,const String &legend_name,const uint8_t radius){
    if(x<_xyparam.xlim_left||_xyparam.xlim_right<x||
       y<_xyparam.ylim_bottom||_xyparam.ylim_top<y) return;

    if (_legend_scatter[color].id==0){
        _legend_scatter[color].maxid+=1;
        _legend_scatter[color].legend_ypos = _gframe.top +14*_legend_scatter[color].maxid;
        _legend_scatter[color].before_x = _xyparam.xlim_left-1;
        _legend_scatter[color].before_y = _xyparam.ylim_bottom-1;
        _legend_scatter[color].width = radius;
        _legend_scatter[color].name = legend_name;
        _legend_scatter[color].id = _legend_scatter[color].maxid;
    }

    uint16_t xpos = _gframe.left+(float)abs(_gframe.right-_gframe.left)/abs(_xyparam.xlim_right-_xyparam.xlim_left)*x;
    uint16_t ypos = _gframe.bottom-(float)abs(_gframe.top-_gframe.bottom)/abs(_xyparam.ylim_top-_xyparam.ylim_bottom)*y;
    const uint8_t r = _legend_scatter[color].width;
    M5.Lcd.fillCircle(xpos,ypos,r,color);

    drawLegendScatter(color);

    _legend_scatter[color].before_x = x;
    _legend_scatter[color].before_y = y;
}

void Graph::drawFrame(){
    const auto xl = _gframe.left;
    const auto xr = _gframe.right;
    const auto yb = _gframe.bottom;
    const auto yt = _gframe.top;
    const auto bc = _grid.back_ground_color;
    const auto gc = _grid.grid_color;
    uint16_t lw = _grid.line_width;
    M5.Lcd.fillScreen(bc);
    drawLine(xl, yb, xr, yb, gc,lw); //bottom
    drawLine(xl, yt, xr, yt, gc,lw); //top
    drawLine(xl, yb, xl, yt, gc,lw); //left
    drawLine(xr, yb, xr, yt, gc,lw); //right
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
            const auto gc = _grid.grid_color;
            uint16_t lw = _grid.line_width;
            drawLine(xpos, _gframe.bottom, xpos,_gframe.top, gc,lw);
        }

        if(TICKS>>1){
            M5.Lcd.drawString(String(x),xpos,_gframe.bottom+_grid.line_width);    
        }
        x += xticks;     
    }

    if(TICKS>>1) M5.Lcd.drawString(String(xlimr),_gframe.right,_gframe.bottom+_grid.line_width);
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
            auto const gc = _grid.grid_color;
            uint16_t lw = _grid.line_width;
            drawLine(_gframe.left,ypos, _gframe.right,ypos, gc,lw);      
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
    const auto bc = _grid.back_ground_color;

    M5.Lcd.fillRect(xpos-w,ypos+3,w,h,bc);
    M5.Lcd.drawRightString(name,xpos,ypos,2);
    M5.Lcd.setTextColor(bc);
}

void Graph::drawLegendPlot(const uint16_t color){
    M5.Lcd.setTextColor(color);
    const auto xpos = _gframe.right-10;
    const auto ypos = _legend_plot[color].legend_ypos;
    const auto name = _legend_plot[color].name;
    const auto w = name.length()*8;
    const auto h = 15;
    const auto bc = _grid.back_ground_color;
    
    M5.Lcd.fillRect(xpos-w,ypos+3,w,h,bc);
    M5.Lcd.drawRightString(name,xpos,ypos,2);
    M5.Lcd.setTextColor(bc);
}
  