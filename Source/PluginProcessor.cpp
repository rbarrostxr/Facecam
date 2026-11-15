#include "PluginProcessor.h"
#include "PluginEditor.h"

CameraProcessor::CameraProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

bool CameraProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == layouts.getMainInputChannelSet()
        && ! layouts.getMainOutputChannelSet().isDisabled();
}

void CameraProcessor::processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&)
{
    // Passa o áudio sem mexer em nada.
}

juce::AudioProcessorEditor* CameraProcessor::createEditor()
{
    return new CameraEditor (*this);
}

void CameraProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::XmlElement xml ("FLCameraState");
    xml.setAttribute ("device", selectedDevice);
    xml.setAttribute ("w", editorWidth);
    xml.setAttribute ("h", editorHeight);
    xml.setAttribute ("hidden", controlsHidden);
    copyXmlToBinary (xml, destData);
}

void CameraProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName ("FLCameraState"))
        {
            selectedDevice = xml->getStringAttribute ("device");
            editorWidth    = xml->getIntAttribute ("w", 640);
            editorHeight   = xml->getIntAttribute ("h", 400);
            controlsHidden = xml->getBoolAttribute ("hidden", false);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CameraProcessor();
}
