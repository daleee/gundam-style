/* Base Implementation - Modelling Layer
 *
 * Base.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h" // for Base class definition

//-------------------------------- Base ---------------------------------------
//
// Base is the base class of all design items in the Modelling Layer
//
iCoordinator*   Base::coordinator = nullptr;
unsigned        Base::now         = 0;
unsigned        Base::lastUpdate  = 0;
bool            Base::active      = false;
int             Base::width       = 0;
int             Base::height      = 0;
int             Base::volume      = 0;
int             Base::frequency   = 0;
