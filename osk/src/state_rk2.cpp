//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells (v50907, see header below)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: state_rk2.cpp 46 2009-12-08 15:05:36Z underwoodml $
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
#include "state_rk2.h"
#include <cmath>

void State_rk2::propagate() {
  switch( kpass) {
    case 0:
      x0 = *x;
      xd0 = *xd;
      *x = x0 + dt / 2.0 * xd0;
      break;
    case 1:
      xd1 = *xd;
      *x = x0 + dt * xd1;
      break;
  }
}

void State_rk2::updateclock() {
  if( kpass == 0) {
    t += dt / 2;
  } else if( kpass == 1) {
    t = t1;
  }
  kpass++;
  kpass = kpass % 2;
  if( kpass == 0) {
    ready = 1;
    t1 = floor( ( t + EPS) / dtp + 1) * dtp;
  } else {
    ready = 0;
  }
}

