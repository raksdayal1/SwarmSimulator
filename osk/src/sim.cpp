//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells (v50907, see header below)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: sim.cpp 51 2011-05-18 13:08:06Z underwoodml $
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
#include "sim.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
int Sim::stop;
int Sim::stop0;

Sim::Sim( const vector<double> &dtsIn, const double &tmaxIn,
          const vector< vector<Block *> > &vStageIn)
: dts(dtsIn), tmax(tmaxIn), vStage(vStageIn)
{
  if (dtsIn.size() != vStageIn.size()) {
    cout << "Inconsistent sizes for dts and vStage in Sim constructor" << endl;
    exit(-1);
  }
}

Sim::Sim( const double * const dtsArrayIn, const double &tmaxIn,
          const vector< vector<Block *> > &vStageIn)
: tmax(tmaxIn), vStage(vStageIn)
{
  int dtsSize = vStage.size();
  for ( int i = 0; i < dtsSize; i++)
    dts.push_back( dtsArrayIn[i]);
}

void Sim::run() {
  run_init();
  run_until(tmax);
  return;
}

void Sim::run_init() {
  State::set();
  Sim::stop = Sim::stop0 = 0;

  vector<Block*>::iterator obj;
  for( stage = vStage.begin(); stage != vStage.end(); stage++) {
    for( obj = stage->begin(); obj != stage->end(); obj++) {
      (*obj)->initCount = 0;
    }
  }

  ii = 0;
  new_stage = 1;
  n = 0;
  nflag = true;

  return;
}

void Sim::run_until(const double &t1) {

  vector<Block*>::iterator obj;
  for( ;;) {
    if( new_stage == 1) {
      n = ii;
      stage = vStage.begin();
      advance( stage, ii);
      State::dt = this->dts[ii];
      State::reset( this->dts[ii]);
      State::tickfirst = 1;
      stage0 = stage;
      for( obj = stage->begin(); obj != stage->end(); obj++) {
        (*obj)->init();
        (*obj)->initCount++;
      }
      new_stage = 0;
    }
    while( true) {
      State::sample( State::EVENT, tmax);
      State::sample( State::EVENT, t1);
      if( nflag) {
        for( obj = stage->begin(); obj != stage->end(); obj++) {
          (*obj)->update();
        }
      }
      if( State::ready) {
        if( nflag) {
          for( obj = stage->begin(); obj != stage->end(); obj++) {
            (*obj)->rpt();
          }
          State::tickfirst = 0;
          if( Sim::stop != Sim::stop0) {
            Sim::stop0 = Sim::stop;
            ii++;
            new_stage = 1;
            break;
          }
          if( State::t + State::EPS >= tmax) {
            Sim::stop = -1;
            break;
          }
        }
        nflag = true;
        if( State::t + State::EPS >= t1) {
          new_stage = 0;
          nflag = false;
          return;
        }
      }
      for( obj = stage->begin(); obj != stage->end(); obj++) {
        (*obj)->propagateStates();
      }
      ( *(stage->begin()))->integrator->updateclock();
    }
    if( Sim::stop < 0) {
      break;
    }
  }
  State::ticklast = 1;
  for( obj = stage0->begin(); obj != stage0->end(); obj++) {
    (*obj)->rpt();
  }
  return;
}

void Sim::cleanup() {
  if (Block::integrator != NULL) {
    delete Block::integrator;
    Block::integrator = NULL;
  }
}
