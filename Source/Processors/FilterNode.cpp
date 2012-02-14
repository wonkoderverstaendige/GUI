/*
  ==============================================================================

    FilterNode.cpp
    Created: 7 May 2011 5:07:28pm
    Author:  jsiegle

  ==============================================================================
*/

#include <stdio.h>
#include "FilterNode.h"
//#include "FilterEditor.h"

FilterNode::FilterNode()
	: GenericProcessor("Bandpass Filter"), filter(0),
	  highCut(6000.0), lowCut(600.0)
	
{
	setNumInputs(10);
	setSampleRate(20000.0);
	// set up default configuration
	setPlayConfigDetails(16, 16, 44100.0, 128);

		// each family of filters is given its own namespace
		// RBJ: filters from the RBJ cookbook
		// Butterworth
		// ChebyshevI: ripple in the passband
		// ChebyshevII: ripple in the stop band
		// Elliptic: ripple in both the passband and stopband
		// Bessel: theoretically with linear phase
		// Legendre: "Optimum-L" filters with steepest transition and monotonic passband
		// Custom: Simple filters that allow poles and zeros to be specified directly

		// within each namespace exists a set of "raw filters"
		// Butterworth::LowPass
		//				HighPass
		// 				BandPass
		//				BandStop
		//				LowShelf
		// 				HighShelf
		//				BandShelf
		//
		//	class templates (such as SimpleFilter) which require FilterClasses
		//    expect an identifier of a raw filter
		//  raw filters do not support introspection, or the Params style of changing
		//    filter settings; they only offer a setup() function for updating the IIR
		//    coefficients to a given set of parameters
		//

		// each filter family namespace also has the nested namespace "Design"
		// here we have all of the raw filter names repeated, except these classes
		//  also provide the Design interface, which adds introspection, polymorphism,
		//  the Params style of changing filter settings, and in general all fo the features
		//  necessary to interoperate with the Filter virtual base class and its derived classes


	


}

FilterNode::~FilterNode()
{
	filter = 0;
}

AudioProcessorEditor* FilterNode::createEditor()
{
	FilterEditor* filterEditor = new FilterEditor(this, viewport);
	setEditor(filterEditor);
	
	std::cout << "Creating editor." << std::endl;
	//filterEditor = new FilterEditor(this);
	return filterEditor;

	//return 0;
}

// void FilterNode::setSourceNode(GenericProcessor* sn)
// {
// 	sourceNode = sn;
// 	setNumInputs(sourceNode->getNumOutputs());

// 	if (destNode != 0)
// 	{
// 		destNode->setNumInputs(getNumOutputs());
// 	}

// }

// void FilterNode::setDestNode(GenericProcessor* dn) 
// {
// 	if (dn != 0) {
// 		if (!dn->isSource())
// 		{
// 			destNode = dn;
// 			destNode->setSourceNode(this);
// 		}
// 	} 
// }	


void FilterNode::setNumInputs(int inputs)
{		

	numInputs = inputs;
	setNumOutputs(inputs);

	if (filter != 0)
	{
		delete filter;
		filter = 0;
	}

	const int nChans = inputs;

	if (nChans == 16) {

	filter = new Dsp::SmoothedFilterDesign 
			<Dsp::Butterworth::Design::BandPass 	// design type
			<4>,								 	// order
			16,										// number of channels (must be const)
			Dsp::DirectFormII>						// realization
			(1024);									// number of samples over which to fade 

      w1 = new float[16*3];
      w2 = new float[16*3];
		
	} else if (nChans == 32) {
	
	filter = new Dsp::SmoothedFilterDesign 
			<Dsp::Butterworth::Design::BandPass 	// design type
			<4>,								 	// order
			32	,									// number of channels (must be const)
			Dsp::DirectFormII>						// realization
			(1024);									// number of samples over which to fade 
													//   parameter changes

      w1 = new float[32*3];
      w2 = new float[32*3];

	} else {
		// send a message saying this is not implemented
	}									

	//std::cout << "Filter created with " << getNumInputs() << " channels." << std::endl;

	setFilterParameters();


  // [b,a] = butter(2,600/15000,'high')
  a1_1 = -1.787432517956485;
  a1_2 = 0.807949591420913;
  b1_0 = 0.898845527344350;
  b1_1 = -1.797691054688699;
  b1_2 = 0.898845527344350;

  // [b,a] = butter(2,6000/15000)
  a2_1 = -0.073623846384979;
  a2_2 = 0.172531250527518;
  b2_0 = 0.274726851035635;
  b2_1 = 0.549453702071270;
  b2_2 = 0.274726851035635;

	setPlayConfigDetails(getNumInputs(), getNumOutputs(), 44100.0, 128);


}

//AudioProcessorEditor* FilterNode::createEditor(AudioProcessorEditor* const editor)
//{
	
//	return editor;
//}

void FilterNode::setSampleRate(float r)
{
	sampleRate = r;
	setFilterParameters();
}

void FilterNode::setFilterParameters()
{

	Dsp::Params params;
	params[0] = getSampleRate(); // sample rate
	params[1] = 4; // order
	params[2] = (highCut + lowCut)/2; // center frequency
	params[3] = highCut - lowCut; // bandwidth

	if (filter != 0)
		filter->setParams (params);

}

void FilterNode::setParameter (int parameterIndex, float newValue)
{
	//std::cout << "Message received." << std::endl;

	if (parameterIndex == 0) {
		lowCut = newValue;
	} else {
		highCut = newValue;
	}

	setFilterParameters();

}


void FilterNode::prepareToPlay (double sampleRate_, int estimatedSamplesPerBlock)
{
	//std::cout << "Filter node preparing." << std::endl;
}

void FilterNode::releaseResources() 
{	
}

void FilterNode::process(AudioSampleBuffer &buffer, 
                            MidiBuffer &midiMessages,
                            int& nSamples)
{
	//std::cout << "Filter node processing." << std::endl;
	//std::cout << buffer.getNumChannels() << std::endl;
	//::cout << buffer.getNumSamples() << std::endl;

	//int nSamps = getNumSamples(midiMessages);
	//std::cout << nSamples << std::endl;

    const int nChan = buffer.getNumChannels();

    static int flag = 100;

    if (flag == 100) {
      std::cout << "Filternode: nChan " << nChan << " nSamples " << nSamples << std::endl;
      flag = 0;
    }
    else 
      flag++;

    static int offset = 0;
    static int offset1 = 1;
    static int offset2 = 2;

    float **data = buffer.getArrayOfChannels();

    //filter->process (nSamples, buffer.getArrayOfChannels());

#ifdef USE_INTRINSICS
  __m128d A1_1, A1_2, B1_0, B1_1, B1_2;
  __m128d W1_0, W1_1, W1_2;
  __m128d Y, W2_0, W2_1, W2_2;
  __m128d A2_1, A2_2, B2_0, B2_1, B2_2;
  __m128d mul1, mul2, mul3, mul4, mul5;

    A1_1 = _mm_loadl_ps(&a1_1);
    A1_2 = _mm_loadl_ps(&a1_2);
    B1_0 = _mm_loadl_ps(&b1_0);
    B1_1 = _mm_loadl_ps(&b1_1);
    B1_2 = _mm_loadl_ps(&b1_2);

    A2_1 = _mm_loadl_ps(&a2_1);
    A2_2 = _mm_loadl_ps(&a2_2);
    B2_0 = _mm_loadl_ps(&b2_0);
    B2_1 = _mm_loadl_ps(&b2_1);
    B2_2 = _mm_loadl_ps(&b2_2);
#endif
    for (int i=0; i < nSamples; i++) {
#ifdef USE_INTRINSICS
      for (int c=0; c<nChan; c = c + 4) {
        W1_1 = _mm_loadu_ps(&(w[c + offset1*nChan]));
        W1_2 = _mm_loadu_ps(&(w[c + offset2*nChan]));

        // CRAP - the order of data in memory is wrong (each channel is sequential)
        W1_0 = _mm_loadu_ps(&(w[c + offset1*nChan]));

        w1[c + offset*nChan] = data[c][i] - \
               a1_1 * w1[c + offset1*nChan] - \
               a1_2 * w1[c + offset2*nChan];

        w2[c + offset*nChan] = b1_0 * w1[c + offset*nChan] + \
                   b1_1 * w1[c + offset1*nChan] + \
                   b1_2 * w1[c + offset2*nChan];

        w2[c + offset*nChan] = w2[c+offset*nChan] - \
               a2_1 * w2[c + offset1*nChan] - \
               a2_2 * w2[c + offset2*nChan];

        data[c][i] = b2_0 * w2[c + offset*nChan] + \
                   b2_1 * w2[c + offset1*nChan] + \
                   b2_2 * w2[c + offset2*nChan];
      }
#else
      for (int c=0; c<nChan; c++) {
        w1[c + offset*nChan] = data[c][i] - \
               a1_1 * w1[c + offset1*nChan] - \
               a1_2 * w1[c + offset2*nChan];

        w2[c + offset*nChan] = b1_0 * w1[c + offset*nChan] + \
                   b1_1 * w1[c + offset1*nChan] + \
                   b1_2 * w1[c + offset2*nChan];

        w2[c + offset*nChan] = w2[c+offset*nChan] - \
               a2_1 * w2[c + offset1*nChan] - \
               a2_2 * w2[c + offset2*nChan];

        data[c][i] = b2_0 * w2[c + offset*nChan] + \
                   b2_1 * w2[c + offset1*nChan] + \
                   b2_2 * w2[c + offset2*nChan];
      }
#endif
      if (offset == 0) {
        offset = 2; offset1 = 0; offset2 = 1;
      }
      else if (offset == 1) {
        offset = 0; offset1 = 1; offset2 = 2;
      }
      else {
        offset = 1; offset1 = 2; offset2 = 0;
      }
    
    }


    //std::cout << "Filter node:" << *buffer.getSampleData(0,0);
}
