/********************************/
/****** Color Wheel chart *******/
/********************************/
/*                              */
/* To run the program, within   */
/* a terminal compile it with:  */ 
/*                              */
/* make                         */
/*                              */
/* And run it with:             */
/*                              */
/* vblank_mode=0 ./colorwheel   */
/*                              */
/* 'vblank_mode=0' is a flag for*/
/* Linux systems to disable the */
/* frozen 60fps rate. However,  */
/* it should still work without */
/* the flag.                    */
/*                              */
/* Implemented by M. Lazarotto  */
/* (07/12/2020 -- the end of an */
/*  odd year)                   */

#include "colorwheel.h"

int main()
{
    // App window settings
    olcColorWheel application;
    int32_t ScreenWidth  = 800;
    int32_t ScreenHeight = 600;
 
    // Main game loop  
    if (application.Construct(ScreenWidth, ScreenHeight, 1, 1))
    {
        application.Start();
    }

    return 0;
}