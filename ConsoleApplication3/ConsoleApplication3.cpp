// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include "UrgDevice.h"
#include "delay.h"
#include "ticks.h"
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <fstream>
#include "tserial.h"
using namespace qrk;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream myfile;
	myfile.open("DATA.txt");
	 const char device[] = "COM7"; //lidar com port
	  UrgDevice urg;
      if (! urg.connect(device)) {
        printf("UrgDevice::connect: %s\n", urg.what());
		return 20;
        //exit(1);
      }
	  printf("YAY!!!!");
	  urg.setCaptureMode(AutoCapture);
	  int scan_msec = urg.scanMsec();

	    int pre_timestamp = ticks();

  // Data is acquired continuously using MD command
  // but outputs data of specified number of times.
  enum { CaptureTimes = 1721};
  urg.setCaptureTimes(CaptureTimes);
  
  
  vector<long> data;
  vector<long> tdata;
  tdata.clear();
  int count=0;
  
    Tserial* t;
    t = new Tserial();
    t->connect("COM3", 9600, spNONE);
	delay(100);
	t->sendChar('r');
	delay(2000);
  for (int i = 0; i < CaptureTimes;) {
	  
       long timestamp = 0;
       data.clear();
	   
       // Get data
       int n = urg.capture(data, &timestamp);
       if (n <= 0) {
          delay(scan_msec);
          continue;
       }
        // Display
	   cout<<"COUNT - "<<i<<"   ---   "<<((double)(i+1)/1721)*100<<endl;
	   
        pre_timestamp = timestamp;
	    for(int j=0;j<data.size();j++){
			if(data[j] == -1){
				continue;
			}
		//	tdata.push_back(data[j]);
			myfile << data[j] << " " ;
		}
		myfile<<endl;
	t->sendChar('u');
	  delay(10);
	  t->sendChar('u');
	  delay(10);
	  i++;
  }
  myfile.close();
  t->disconnect();
	return (int)tdata.size();
}

