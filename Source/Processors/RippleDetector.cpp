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


#include <stdio.h>
#include "RippleDetector.h"
//#include "Editors/RippleDetectorEditor.h"

RippleDetector::RippleDetector()
	: GenericProcessor("Ripple Detector"), canBeTriggered(true)

{

}

RippleDetector::~RippleDetector()
{
   
}

void RippleDetector::setParameter (int parameterIndex, float newValue)
{


}

void RippleDetector::updateSettings()
{


}

bool RippleDetector::enable()
{

}

void RippleDetector::handleEvent(int eventType, MidiMessage& event, int sampleNum)
{

	//std::cout << "GOT EVENT." << std::endl;

	if (eventType == TTL)
	{
		uint8* dataptr = event.getRawData();

    	int eventNodeId = *(dataptr+1);
    	int eventId = *(dataptr+2);
    	int eventChannel = *(dataptr+3);

    	if (eventId == 1 && eventChannel == 5)
    	{
    		canBeTriggered = true;
    	} else if (eventId == 0 && eventChannel == 5) {
    		canBeTriggered = false;
    	}

    }

}

void RippleDetector::process(AudioSampleBuffer &buffer, 
                            MidiBuffer &events,
                            int& nSamples)
{

	// checkForEvents(events);

	// for (int i = 0; i < nSamples; i++)
	// {

	// 	float sample = *buffer.getSampleData(0, i);

	//   	if (ripple is detected)
	//   	{
	// 			addEvent(events, TTL, i, 0, 3);
	//   	}

	// }


}

