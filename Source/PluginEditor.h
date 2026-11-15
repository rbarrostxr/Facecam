#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_video/juce_video.h>

class CameraEditor : public juce::AudioProcessorEditor
{
public:
    explicit CameraEditor (CameraProcessor&);
    ~CameraEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDoubleClick (const juce::MouseEvent&) override;

private:
    void refreshDeviceList();
    void openSelectedDevice();
    void closeDevice();
    void applyControlsVisibility();

    CameraProcessor& processor;

    juce::ComboBox deviceBox;
    juce::TextButton refreshButton { "Atualizar" };
    juce::Label statusLabel;

    // A ordem importa: o viewer precisa ser destruído antes da câmera.
    std::unique_ptr<juce::CameraDevice> camera;
    std::unique_ptr<juce::Component> viewer;

    juce::StringArray devices;
    bool controlsHidden = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CameraEditor)
};
