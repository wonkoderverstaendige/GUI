/*
 ------------------------------------------------------------------

 This file is part of the Open Ephys GUI
 Copyright (C) 2013 Florian Franzen

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

#include "HDF5Recording.h"
#define MAX_BUFFER_SIZE 10000

HDF5Recording::HDF5Recording() : processorIndex(-1)
{
    timestamp = 0;
    scaledBuffer = new float[MAX_BUFFER_SIZE];
    intBuffer = new int16[MAX_BUFFER_SIZE];
}

HDF5Recording::~HDF5Recording()
{
    delete scaledBuffer;
    delete intBuffer;
}

String HDF5Recording::getEngineID()
{
	return "KWIK";
}

void HDF5Recording::updateTimeStamp(int64 timestamp)
{
    this->timestamp = timestamp;
}

void HDF5Recording::registerProcessor(GenericProcessor* proc)
{
    HDF5RecordingInfo* info = new HDF5RecordingInfo();
    info->sample_rate = proc->getSampleRate();
    info->bit_depth = 16;
    infoArray.add(info);
    fileArray.add(new KWDFile());
    activeChannelCount.add(0);
    processorIndex++;
}

void HDF5Recording::resetChannels()
{
    processorIndex = -1;
    fileArray.clear();
    activeChannelCount.clear();
    processorMap.clear();
    infoArray.clear();
    if (spikesFile)
        spikesFile->resetChannels();
}

void HDF5Recording::addChannel(int index, Channel* chan)
{
    processorMap.add(processorIndex);
}

void HDF5Recording::openFiles(File rootFolder,  int experimentNumber, int recordingNumber)
{
    String basepath = rootFolder.getFullPathName() + rootFolder.separatorString + "experiment" + String(experimentNumber);
    //KWIK file
    mainFile->initFile(basepath);
    mainFile->open();

    //KWX file
    spikesFile->initFile(basepath);
    spikesFile->open();
    spikesFile->startNewRecording(recordingNumber);

    //Let's just put the first processor (usually the source node) on the KWIK for now
    infoArray[0]->name = String("Open-Ephys Recording #") + String(recordingNumber);
    infoArray[0]->start_time = timestamp;
    infoArray[0]->start_sample = 0;
    mainFile->startNewRecording(recordingNumber,infoArray[0]);

    //KWD files
    for (int i = 0; i < processorMap.size(); i++)
    {
        int index = processorMap[i];
        if (getChannel(i)->getRecordState())
        {
            if (!fileArray[index]->isOpen())
            {
                fileArray[index]->initFile(getChannel(i)->nodeId,basepath);
                fileArray[index]->open();
            }
            activeChannelCount.set(index,activeChannelCount[index]+1);
        }
    }
    for (int i = 0; i < fileArray.size(); i++)
    {
        if (fileArray[i]->isOpen())
        {
            File f(fileArray[i]->getFileName());
            mainFile->addKwdFile(f.getFileName());

            infoArray[i]->name = String("Open-Ephys Recording #") + String(recordingNumber);
            infoArray[i]->start_time = timestamp;
            infoArray[i]->start_sample = 0;
            fileArray[i]->startNewRecording(recordingNumber,activeChannelCount[i],infoArray[i]);
        }
    }
}

void HDF5Recording::closeFiles()
{
    mainFile->stopRecording();
    mainFile->close();
    spikesFile->stopRecording();
    spikesFile->close();
    for (int i = 0; i < fileArray.size(); i++)
    {
        fileArray[i]->stopRecording();
        fileArray[i]->close();
        activeChannelCount.set(i,0);
    }
}

void HDF5Recording::writeData(AudioSampleBuffer& buffer, int nSamples)
{
    int index;
    for (int i = 0; i < buffer.getNumChannels(); i++)
    {
        if (getChannel(i)->getRecordState())
        {
            double multFactor = 1/(float(0x7fff) * getChannel(i)->bitVolts);
            int index = processorMap[getChannel(i)->recordIndex];
            FloatVectorOperations::copyWithMultiply(scaledBuffer,buffer.getReadPointer(i,0),multFactor,nSamples);
            AudioDataConverters::convertFloatToInt16LE(scaledBuffer,intBuffer,nSamples);
            fileArray[index]->writeRowData(intBuffer,nSamples);
        }
    }
}

void HDF5Recording::writeEvent(int eventType, MidiMessage& event, int samplePosition)
{
    const uint8* dataptr = event.getRawData();
	if (eventType == GenericProcessor::TTL)
		mainFile->writeEvent(0,*(dataptr+2),*(dataptr+1),(void*)(dataptr+3),timestamp+samplePosition);
	else if (eventType == GenericProcessor::MESSAGE)
		mainFile->writeEvent(1,*(dataptr+2),*(dataptr+1),(void*)(dataptr+4),timestamp+samplePosition);
}

void HDF5Recording::addSpikeElectrode(int index, SpikeRecordInfo* elec)
{
    spikesFile->addChannelGroup(elec->numChannels);
}
void HDF5Recording::writeSpike(const SpikeObject& spike, int electrodeIndex)
{
    spikesFile->writeSpike(electrodeIndex,spike.nSamples,spike.data,spike.timestamp);
}

void HDF5Recording::startAcquisition()
{
    mainFile = new KWIKFile();
	mainFile->addEventType("TTL",HDF5FileBase::U8,"event_channels");
	mainFile->addEventType("Messages",HDF5FileBase::STR,"Text");
    spikesFile = new KWXFile();
}

RecordEngineManager* HDF5Recording::getEngineManager()
{
	RecordEngineManager* man = new RecordEngineManager("KWIK","Kwik",nullptr);
	return man;
}