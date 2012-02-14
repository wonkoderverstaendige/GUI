/*
  ==============================================================================

    FilterNode.h
    Created: 7 May 2011 5:07:28pm
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __FILTERNODE_H_CED428E__
#define __FILTERNODE_H_CED428E__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Dsp/Dsp.h"
#include "GenericProcessor.h"
#include "Editors/FilterEditor.h"

class FilterEditor;
class FilterViewport;

class FilterNode : public GenericProcessor

{
public:
	
	FilterNode();
	~FilterNode();
	
	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void process(AudioSampleBuffer &buffer, MidiBuffer &midiMessages, int& nSamples);
	void setParameter (int parameterIndex, float newValue);

	AudioProcessorEditor* createEditor();

	bool hasEditor() const {return true;}

	// void setSourceNode(GenericProcessor* sn);
	// void setDestNode(GenericProcessor* dn);
	
	void setNumInputs(int);
	void setSampleRate(float);
	
private:
	double lowCut, highCut;

  float *w1;
  float a1_1, a1_2, b1_0, b1_1, b1_2;
  float *w2;
  float a2_1, a2_2, b2_0, b2_1, b2_2;

	Dsp::Filter* filter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterNode);

	void setFilterParameters();

};





#endif  // __FILTERNODE_H_CED428E__
