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

#ifndef __NETWORKTHREAD_H_DD31EB15__
#define __NETWORKTHREAD_H_DD31EB15__

#include <stdio.h>

#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
#include "../../../JuceLibraryCode/JuceHeader.h"
#include <sys/socket.h> //for linux
#include <netinet/in.h> //for linux
#include <arpa/inet.h> //for linux
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// #include "../../Network/unp.h"
// #include "../../Network/netcom.h"
// #include "../../Network/datapacket.h"

#include "DataThread.h"

/**

  --OBSOLETE--

  Receives data from a network source.

  Based on the network communication protocol from ArTE (https://code.google.com/p/arte-ephys/)

  @see DataThread

*/

class NetworkThread : public DataThread
{
public:

	NetworkThread(SourceNode* sn);
	~NetworkThread();

	bool foundInputSource();
	bool startAcquisition(); //{return true;}
	bool stopAcquisition(); //{return true;}
	int getNumChannels();
	int getNumEventChannels();
	float getBitVolts();
	float getSampleRate();

private:

	// NetCom my_netcom;
	// NetComDat my_netcomdat;

	// lfp_bank_net_t lfp;

	DataBuffer* dataBuffer;

	float thisSample[8];

	bool updateBuffer();
	bool deviceFound;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NetworkThread);

};



#endif  // __NETWORKTHREAD_H_DD31EB15__
