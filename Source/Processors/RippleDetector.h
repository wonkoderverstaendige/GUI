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

#ifndef __RIPPLEDETECTOR_H_3C509249__
#define __RIPPLEDETECTOR_H_3C509249__

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../../JuceLibraryCode/JuceHeader.h"
#include "GenericProcessor.h"

/**

  Detects ripple events in the local field potential

  @see GenericProcessor

*/


class RippleDetector : public GenericProcessor

{
public:
	
	RippleDetector();
	~RippleDetector();
	
	void process (AudioSampleBuffer &buffer, MidiBuffer &events, int& nSamples);
	void setParameter (int parameterIndex, float newValue);

	//AudioProcessorEditor* createEditor();

	bool hasEditor() const {return false;}

	bool enable();

	void updateSettings();

private:

	bool canBeTriggered;

	void handleEvent(int eventType, MidiMessage& event, int sampleNum);


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RippleDetector);

};

#endif  // __RIPPLEDETECTOR_H_3C509249__
