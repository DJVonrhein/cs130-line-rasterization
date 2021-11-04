#include "application.h"

#include <iostream>
#include <cassert>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

using namespace std;
// set pixel (x,y) in framebuffer to color col, where
// col is a float array of three values between 0 and 1
// which specify the amount of red, green, and blue to mix (e.g.
// RED: (1,0,0) GREEN: (0,1,0) BLUE: (0,0,1)
// YELLOW: (1,1,0) MAGENTA: (1,0,1) CYAN: (0,1,1)
// )
//
void set_pixel(int x, int y, float col[3])
{
    // write a 1x1 block of pixels of color col to framebuffer
    // coordinates (x, y)
    glRasterPos2i(x, y);
    glDrawPixels(1, 1, GL_RGB, GL_FLOAT, col);
}

void set_pixel(int x, int y)
{
    float col[] = { 1.0, 1.0, 1.0 };
    set_pixel(x,y,col);
}


//
// This is where the line drawing algorithm gets called
//
void draw_line(int x0, int y0, int x1, int y1, float col[3])
{
    // TODO: Implement the correct algorithm here.

    if(abs(y0 - y1) <= abs(x0 - x1)){   // |m| <= 1

        if(x1 < x0){        //swap the points if necessary
            std::swap(x0,x1);
            std::swap(y0,y1);
        }
        // float g = 2 *( (y0 - y1)* (x0 + 1) + (x1 - x0) * (y0 + 0.5) + (x0 * y1 )- (x1 * y0)); // Ax + By + C
        
        // float g =  2* ( (y0 - y1)* (x0 + 1) + (x1 - x0) * (y0 + 0.5) + (x0 * y1 )- (x1 * y0)); // Ax + By + C
        
        // float dx = x1 - x0;
        // int deltaX = 1;
        int deltaY = (y1 > y0) ? 1 : -1;
        float A = y1 - y0;
        float B = x1 - x0;
        float C = (x0 * y1 )- (x1 * y0);
        // float m = dy/dx;
        // float dg0 = 2.0 * dy *((y0 - y1) + (x1 - x0));
        // float dg1 = 2.0 * dy *(y0 - y1);
        
        for(int x = x0, y = y0; x <= x1; x++)
        {
            set_pixel(x, y, col);
            int g = 2* deltaY * ( -A * (x + 1) + B * (y + 0.5) + C);
            if ( g < 0){
                y += deltaY;
            }
            
        }
    }
    else{               // |m| > 1

        if(y1 < y0){        //swap the points if necessary
            std::swap(x0,x1);
            std::swap(y0,y1);
        }


        // float g = 2 *( (x0 - x1)* (y0 + 1) + (y1 - y0) * (x0 + 0.5) + (y0 * x1 )- (y1 * x0)); // Ax + By + C
        // float dy = y1 - y0;
        // int deltaY = 1;
        int deltaX = (x1 > x0) ? 1 : -1;
        float A = x1 - x0;
        float B = y1 - y0;
        float C = (-1 * x0 * y1 ) + (x1 * y0);
        // float m = dy/dx;
        // float dg0 = 2.0 * dx *((x0 - x1) + (y1 - y0));
        // float dg1 = 2.0 * dx *(x0 - x1);

        for(int x = x0, y = y0; y <= y1; y++)
        {
            set_pixel(x, y, col);
            int g = 2 * deltaX * ( -A * (y + 1) + B * (x + 0.5) + C);
            if ( g < 0){
                x += deltaX;
            }
        }
    }
}

// triggered each time the application needs to redraw
void application::draw_event()
{
    //
    // This code draws a single red horizontal line across
    // the the middle of the window using the function
    // set_pixel. Replace this code with your DDA
    // implementation that can draw a line of any slope. You
    // should only need to call set_pixel within your
    // routine.
    //
    float red[] = { 1.0, 0.0, 0.0 };
    float white[] = { 1.0, 1.0, 1.0 };

    for(int i = 0; i < this->width; ++i)
        set_pixel(i, this->height/2, red);

    //
    // This code will draw each of the lines
    //
    for(size_t i = 0; i < lines.size(); ++i)
        draw_line(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1, white);
}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
    // Each time the mouse button is pushed down, add a new
    // line segment to our list.
    if(button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN) {
        lines.push_back(line_segment(x, y, x, y));
        button_down = true;
    }
    if(button == GLUT_LEFT_BUTTON && button_state == GLUT_UP) {
        button_down = false;
    }
    //cout << "button event: " << button << " " << button_state
    //    << " " << x << " " << y << endl;
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
    // While the mouse button is down, update the last
    // endpoint in our list to reflect the current location
    // of the mouse.
    lines[lines.size()-1].x1 = x;
    lines[lines.size()-1].y1 = y;

    //cout << "move event: " << x << " " << y << endl;
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
    // clear our stored clicks
    if(key == 'c')
        lines.clear();

    //cout << "keyboard event: " << key << " "
    //    << x << " " << y << endl;
}
