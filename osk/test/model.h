//
// US Army Maintained Version of C++ Model Developer (CMD)
//     Based on original work by Ray Sells, DESE Research, Inc. (v50907)
//     Maintained by: Mark Underwood (Mark.Underwood2@us.army.mil)
//                    Jay Reinebold (James.Reinebold@us.army.mil)
//     Release Agent: Jeff Hester (Jeffrey.Hester@us.army.mil)
//
// $Id: model.h 46 2009-12-08 15:05:36Z underwoodml $
//
#ifndef MODEL_H
#define MODEL_H

#include "block.h"

class Model : public Block {
  public:
    Model( double x10_, double sdt, 
           double te_0_, double te_dt_, double te_drift_);
  protected:
    void init();
    void update();
    void rpt();
    double x10, x20, x1d, x2d, x1, x2;
    double sdt, te;
    double te_0, te_dt, te_drift;
    long n;
};

#endif

