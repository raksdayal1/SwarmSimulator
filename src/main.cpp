/*
 * Created by Rakshit Allamraju
 * Date: Feb. 24 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>

#include "sim.h"
#include "libAP_JSON.h"

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#include <json.hpp>

#include "simplerover.h"
#include "genericmultirotor.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char** argv) {
  double  tmax = 50000.00;
  double  dt = 0.01;

  bool flag(0);

  /*
  // read a JSON file
  std::ifstream file("init.json");
  json j;
  json j1;
  file >> j;

  j1 = j.value("Vehicles","oops No value AP_addr");
  std::string f1 = j1.value("test","oops");

  std::cout << f1 << std::endl;

  */

  int N_mr(0), N_fw(0), N_sr(0); // number of multirotor, Fixedwing and rovers

  N_sr = 1;

  vector<Block*> vObj0;
  vector< vector<Block*> > vStage;

  // allocating dynamic array of Size N
  SimpleRover* sr_arr = (SimpleRover*)malloc(sizeof(SimpleRover) * N_sr);

  // calling constructor for each index of array
  for (int i = 0; i < N_sr; i++) {
      sr_arr[i] = SimpleRover(0.0, 0.0, 0.0, 9002, 5006);
      vObj0.push_back(&sr_arr[i]);
  }

  vStage.push_back( vObj0);

  double dts[] = { dt};
  Sim *sim = new Sim( dts, tmax, vStage);
  sim->run();

  sim->cleanup();
  delete sim;

  for (int i = 0; i < N_sr; i++) {
      delete &sr_arr[i];
  }

}
