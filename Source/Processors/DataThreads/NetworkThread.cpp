/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2012 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "NetworkThread.h"

NetworkThread::NetworkThread(SourceNode* sn) : DataThread(sn)
 {
// 	char host[] = "10.121.43.47";
// 	char port[] = "5227";

// 	my_netcomdat = my_netcom.initUdpRx(host, port);
	
	// I'm just going to start by pulling the first 8 channels and discarding the rest.
	// Does the size depend on the number of channels somehow?
 	dataBuffer = new DataBuffer(8, 4096);
 	
 	deviceFound = false; //initialization

// 	startThread();

// 	std::cout << "Network interface created." << std::endl;
}

NetworkThread::~NetworkThread() {
	// stopThread(500);
	// close(my_netcomdat.sockfd);
	
	// // need to close socket in order to reopen
	// close(my_netcomdat.sockfd);

	// std::cout << "Network interface destroyed." << std::endl;

	// delete dataBuffer;
	// dataBuffer = 0;
}

int NetworkThread::getNumChannels()
{
    return 8;
}

int NetworkThread::getNumEventChannels()
{
    return 1;
}

float NetworkThread::getSampleRate()
{
    return 31250.0;
}

float NetworkThread::getBitVolts()
{
    return 0.1907; //this needs to be changed!
}

bool NetworkThread::foundInputSource()
{
	return deviceFound;
}


bool NetworkThread::updateBuffer(){
		
	 // NetCom::rxWave (my_netcomdat, &lfp);

	 // for (int s = 0; s < lfp.n_samps_per_chan; s++) {
	 // 	for (int c = 0; c < lfp.n_chans; c++) {
	 // 		thisSample[c] = float(lfp.data[s*lfp.n_chans + c])/500.0f;
	 // 	}
	 // 	dataBuffer->addToBuffer(thisSample,1);
	 // }

	 // return true;
}