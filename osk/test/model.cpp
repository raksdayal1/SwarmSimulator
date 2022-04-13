//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells, DESE Research, Inc. (v50907)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: model.cpp 46 2009-12-08 15:05:36Z underwoodml $
//
#include "model.h"
#include <cstdio>
#include <iostream>

Model::Model( double x10_, double sdt_,
              double te_0_, double te_dt_, double te_drift_) {
  x10 = x10_;
  sdt = sdt_;
  te_0 = te_0_; 
  te_dt = te_dt_; 
  te_drift = te_drift_; 
  addIntegrator( x1, x1d);
}

void Model::init() {
  cout << "starting Model..." << endl << "dt = " << State::dt;
  cout << ", sdt = " << sdt;
  cout << ", te_0 = " << te_0;
  cout << ", te_dt = " << te_dt;
  cout << ", te_drift = " << te_drift << endl;
  if( initCount == 0) {
    x1 = x10;
    n = 0;
    te = te_0;
  }
}

void Model::update() {
  x1d = 1.0;
  if( State::sample( State::EVENT, te)) {
    //cout << "**** " << te << " " << State::t << endl;
    printf( "**** %.6f %.6f\n", te, State::t);
    te += ( te_dt + te_drift * ( State::t - te_0));
    State::sample( State::EVENT, te);
  }
  if( State::sample( sdt)) {
    n = n + 1;
  }
}

void Model::rpt() {
  //if( State::sample( 1.0e-2) || State::ticklast) {
  #if 1 
  //if( State::sample( 1.0) || State::ticklast) {
  if( State::sample( 1000.0) || State::ticklast) {
    printf( "%10.9f %10.9f %14.9f ", State::t, this->x1, State::dt); 
    printf( "%ld\n", n);
  }
  #endif
}
