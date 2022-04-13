//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells, DESE Research, Inc. (v50907)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: main.cpp 46 2009-12-08 15:05:36Z underwoodml $
//
#include <vector>
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

#include "model.h"
#include "state_euler.h"
#include "state_rk2.h"
#include "state_mm.h"

int main( int argc, char *argv[]) {
  cout << argv[0] << endl;
  cout << ULONG_MAX << endl;
  //cout << setprecision( 20) << DBL_MAX << endl;
  double dt = 1.0 / 400.0;
  double sdt = 1.0 / 300.0;
  double te_0 = 0.0;
  double te_dt = 0.11;
  double te_drift = 0.00001;
  double tmax = 1e3;
  /*
  double dt = 0.001;
  double sdt = 0.001;
  double te_0 = 130;
  double te_dt = 0.1;
  double te_drift = 0;
  double tmax = 1000.0;
  double  dt = 30;
  double sdt = 333.3333333333334;
  double te_0 = 130000;
  double te_dt = 222.2222222222222;
  double te_drift = 0;
  double tmax = 1000000.0;
  */

  if( argc >= 2) {
    dt = atof( argv[1]);
  }
  if( argc >= 3) {
    sdt = atof( argv[2]);
  }
  if( argc >= 4) {
    tmax = atof( argv[3]);
  }
  if( argc >= 5) {
    te_0 = atof( argv[4]);
  }
  if( argc >= 6) {
    te_dt = atof( argv[5]);
  }
  if( argc >= 7) {
    te_drift = atof( argv[6]);
  }
  double EPS = 1e-12;
  cout << setprecision( 16) << 1 + EPS << endl;
  //tmax = 3.98;

  //Block::integrator = new State();
  //Block::integrator = new State_euler();
  //Block::integrator = new State_rk2();
  //Block::integrator = new State_mm();
  Model *model = new Model( 0.0, sdt, te_0, te_dt, te_drift);

  vector<Block*> vObj0;
  vObj0.push_back( model);

  vector< vector<Block*> > vStage;
  vStage.push_back( vObj0);

  //double dta[] = { 0.000001, 0.00001, 0.0001, 0.001, 0.01, 0.1};
  for( int i = 0; i < 1; i++) {
    //double dts[] = { dta[i]};
    double dts[] = { dt};
    Sim *sim = new Sim( dts, tmax, vStage);
    sim->run();
    sim->cleanup();
    delete sim;
  }
  delete model;
}
