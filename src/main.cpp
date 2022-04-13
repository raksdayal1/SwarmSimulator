/*
 * Created by Rakshit Allamraju
 * Date: Feb. 24 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 13 2022
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
  int N_mr(0), N_fw(0), N_sr(0); // number of multirotor, Fixedwing and rovers
  uint16_t ap_port, gazebo_port;
  double x_init(0), y_init(0), z_init(0), roll_init(0), pitch_init(0), yaw_init(0);

  int temp_ap(0), temp_gazebo(0);

  // read the JSON file containing simulation info
  std::ifstream file(argv[1]);
  json sim_json;
  file >> sim_json;

  // Parse the number of vehicles for each type
  N_sr = sim_json["Vehicles"]["Rovers"]["Instances"];
  N_mr = sim_json["Vehicles"]["Quadcopters"]["Instances"];
  N_fw = sim_json["Vehicles"]["Fixedwings"]["Instances"];

  vector<Block*> vObj0;
  vector< vector<Block*> > vStage;

  // Setting Rover data
  SimpleRover* sr_array[N_sr];

  // calling constructor for each index of array
  for (int i = 0; i < N_sr; i++) {

      temp_ap = sim_json["Vehicles"]["Rovers"]["InstanceNo"][i];

      ap_port = 9002 + 10*temp_ap;
      gazebo_port = 5006 + 10*temp_gazebo;

      x_init = sim_json["Vehicles"]["Rovers"]["Xstart"][i];
      y_init = sim_json["Vehicles"]["Rovers"]["Ystart"][i];
      yaw_init = 0;

      sr_array[i] = new SimpleRover(x_init, y_init, yaw_init, ap_port, gazebo_port);
      vObj0.push_back(sr_array[i]);

  }

  vStage.push_back( vObj0);

  double dts[] = { dt};
  Sim *sim = new Sim( dts, tmax, vStage);
  sim->run();

  sim->cleanup();
  delete sim;

  for (int i = 0; i < N_sr; i++) {
      delete sr_array[i];
  }

}

