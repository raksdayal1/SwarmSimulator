//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells (v50907, see header below)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: block.cpp 51 2011-05-18 13:08:06Z underwoodml $
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
#include <typeinfo>
#include <cstdlib>
#include "block.h"
#include <string>

State *Block::integrator = NULL;


Block::Block() {
}

Block::~Block() {
  vector<State*>::iterator pState;
  for( pState = vState.begin(); pState != vState.end(); pState++)
    delete *pState;
}

void Block::addIntegrator( double &x, double &xd) {
  State *si = getIntegrator();
  string integrator_id = typeid( *si).name();
  State *s = si->factory( x, xd);
  string created_integrator_id = typeid( *s).name();
  if( integrator_id != created_integrator_id) {
    cout << integrator_id << "," << created_integrator_id << endl;
    cout << "error:\n";
    cout << "  Need to add \"STATE( ";
    cout << integrator_id << ");\" to header file for ";
    cout << integrator_id << "." << endl;
    exit( 1);
  }
  vState.push_back( s);
}

void Block::propagateStates() {
  vector<State*>::iterator pState;
  for( pState = vState.begin(); pState != vState.end(); pState++) {
    (*pState)->propagate();
  }
}

void Block::giveIntegrator( State *integratorIn) {
  if (Block::integrator == NULL) {
    Block::integrator = integratorIn;
  } else {
    cout << "Integrator has already been set, cannot give another" << endl;
    exit(-1);
  }
}

State *Block::getIntegrator() {
  if (Block::integrator == NULL) {
    Block::integrator = new State();
  }
  return Block::integrator;
}

