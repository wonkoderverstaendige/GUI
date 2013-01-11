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
#include <errno.h>

NetworkThread::NetworkThread(SourceNode* sn) : DataThread(sn)
 {
	
	memset(&dataddr, 0, sizeof(dataddr));
	//still using IPv4, change this if you want to switch!
	dataddr.sin_family = AF_INET;
	dataddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	dataddr.sin_port=htons(61557);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	std::cout << "Socket ID: " << sockfd << std::endl;
	
	int bindfd;	
	bindfd = bind(sockfd, (struct sockaddr *) &dataddr, sizeof(dataddr));
	std::cout << "Bind ID: " << bindfd << std::endl;
	
	
	// I'm just going to start by pulling the first 8 channels and discarding the rest.
	// Does the size depend on the number of channels somehow?
 	dataBuffer = new DataBuffer(8, 4096);
 	
 	deviceFound = true; //initialization

 	startThread();

	std::cout << "Network interface created." << std::endl;
}

NetworkThread::~NetworkThread() {
	stopThread(500);

	std::cout << "Network interface destroyed." << std::endl;

	//delete dataBuffer;
	//dataBuffer = 0;
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

bool NetworkThread::startAcquisition()
{
	
	if (sockfd == 0)
	{	
		sockfd = socket(AF_INET,SOCK_DGRAM,0);
		if (sockfd < 0)
		{
			deviceFound = false;
			return false;
		}
	
		int bindfd;	
		bindfd = bind(sockfd, (struct sockaddr *) &dataddr, sizeof(dataddr));

		if (bindfd < 0)
		{
			deviceFound = false;
			return false;
		}
	}
	startThread();
	
	deviceFound = true;
	return true;
}

bool NetworkThread::stopAcquisition()
{
	close(sockfd);
	sockfd = 0;

	if (isThreadRunning()) {
        	signalThreadShouldExit();
    	}

	deviceFound = false;
	return true;
}


bool NetworkThread::updateBuffer()
{
	char newData[80];
	int receiveData;

	socklen_t addrlen;

	addrlen = sizeof(dataddr);

	receiveData = recvfrom(sockfd, &newData, 80, 0, (struct sockaddr *) & dataddr, &addrlen);

	if (receiveData < 0)
	{
		std::cout << errno << std::endl;
		return false;
	}
	
	if (newData[4] == 0)
	{
		for (int i = 0; i++; i<8)
		{
			thisSample[i] = float(newData[i + 17])*15/pow(2.0,16);
		}
		
		timestamp = uint64(newData[12]);
		eventCode = 0;
		
		dataBuffer->addToBuffer(thisSample, &timestamp, &eventCode, 1);
	}
	
	 return true;
}
