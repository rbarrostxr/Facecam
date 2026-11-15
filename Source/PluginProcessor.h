#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

// Plugin de efeito "transparente": o áudio passa sem alteração.
// A única função dele é abrir a janela com a câmera/dispositivo de vídeo.
class CameraProcessor : public juce::AudioProcessor
{
public:
    CameraProcessor();
    ~CameraProcessor() override = default;

    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "FL Camera"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Estado salvo junto com o projeto do FL
    juce::String selectedDevice;
    int editorWidth = 640;
    int editorHeight = 400;
    bool controlsHidden = false;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CameraProcessor)
};
