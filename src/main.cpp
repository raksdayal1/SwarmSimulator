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

  libAP_JSON ap;

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


  if (argc != 2){
      std::cout << "Incorrect number of arguments " << std::endl;
      exit(-1);
  }

  flag = atoi(argv[1]);
  //std::cout << "port used is " << port << std::endl;
  if (ap.InitSockets("127.0.0.1", 9002)) //port is hardcoded to 9002
  {
      std::cout << "started socket" << std::endl;
  }

  if(flag){
      cout << "Starting Quad physics engine " << std::endl;
      GenericMultirotor *quad = new GenericMultirotor(&ap);

      vector<Block*> vObj0;
      vObj0.push_back(quad);

      vector< vector<Block*> > vStage;
      vStage.push_back( vObj0);

      double dts[] = { dt};
      Sim *sim = new Sim( dts, tmax, vStage);
      sim->run();

      sim->cleanup();
      delete sim;
      delete quad;
  }else
  {
      cout << "Starting Rover physics engine " << std::endl;
      SimpleRover *rover = new SimpleRover(0.0, 0.0, 0.0, &ap);
      vector<Block*> vObj0;
      vObj0.push_back(rover);

      vector< vector<Block*> > vStage;
      vStage.push_back( vObj0);

      double dts[] = { dt};
      Sim *sim = new Sim( dts, tmax, vStage);
      sim->run();

      sim->cleanup();
      delete sim;
      delete rover;
  }


}
