//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells (v50907, see header below)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: state.cpp 46 2009-12-08 15:05:36Z underwoodml $
//
/*
----------------
This OPEN SOURCE CODE is part of the Object-Oriented Simulation Kernel (OSK)
created by

                Ray Sells, DESE Research, Inc., 2003.

                        All Rights Reserved.

The OSK consists of state.h, state.cpp, block.h, block.cpp, sim.h, and sim.cpp.

Permission to use, copy, modify, and distribute the OSK software and its  
documentation for any purpose and without fee is hereby granted,  
provided that this notice appears in all copies and supporting documentation.

DESE RESEARCH, INC. AND RAY SELLS DISCLAIM ALL WARRANTIES WITH REGARD
TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS, IN NO EVENT SHALL DESE RESEARCH, INC. OR RAY SELLS
BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE, EVEN IF THE SOFTWARE IS USED FOR ITS INTENDED PURPOSE.
----------------
*/

#include "state.h"
#include <cmath>

double  State::dt;
double  State::dtp;
int     State::kpass;
bool    State::ready;
double  State::t;
double  State::t1;
int     State::ticklast;
int     State::tickfirst;
const double State::EVENT = -1.0;
const double State::EPS = 1e-8;

State::State( double &xIn, double &xdIn)
: x(&xIn), xd(&xdIn)
{}

void State::propagate() {
  switch( kpass) {
    case 0:
      x0 = *x;
      xd0 = *xd;
      *x = x0 + dt / 2.0 * xd0;
      break;
    case 1:
      xd1 = *xd;
      *x = x0 + dt / 2.0 * xd1;
      break;
    case 2:
      xd2 = *xd;
      *x = x0 + dt * xd2;
      break;
    case 3:
      xd3 = *xd;
      *x = x0 + dt / 6 * ( xd0 + 2 * xd1 + 2 * xd2 + xd3);
      break;
  }
}

void State::updateclock() {
  if( kpass == 0) {
    t += dt / 2;
  } else if( kpass == 2) {
    t = t1;
  }
  kpass++;
  kpass = kpass % 4;
  if( kpass == 0) {
    ready = 1;
    t1 = floor( ( t + EPS) / dtp + 1) * dtp;
  } else {
    ready = 0;
  }
}

void State::set() {
  kpass = 0;
  ready = 1;
  t = 0.0;
  ticklast = 0;
}


void State::reset( const double &dtp) {
  State::dt = State::dtp = dtp;
  State::t1 = State::t + dtp;
}

int State::sample() {
  if( !State::ready) return 0;
  return 1;
}

int State::sample( const double &sdt, const double &t_event) {
  if( !State::ready) return 0;
  if( sdt < 0.0) {
    if( t_event < t1 - EPS && t_event >= t + EPS) {
      t1 = t_event;
    }
    State::dt = t1 - t;
    if( fabs( t_event - t) < EPS) {
      return 1;
    } else {
      return 0;
    }
  }
  // calculate end of next sample time
  double ts = floor( ( t + EPS) / sdt + 1) * sdt;
  if( ts < t1 - EPS) {
    t1 = ts;
  }
  State::dt = t1 - t;
  // check to see if it's time to sample
  if( t - ts + sdt < EPS) {
    return 1;
  } else {
    return 0;
  }
}

