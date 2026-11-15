#include "PluginEditor.h"

static constexpr int barHeight = 32;

CameraEditor::CameraEditor (CameraProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    controlsHidden = processor.controlsHidden;

    deviceBox.setTextWhenNothingSelected ("Escolha o dispositivo de video...");
    deviceBox.setTextWhenNoChoicesAvailable ("Nenhum dispositivo encontrado");
    deviceBox.onChange = [this]
    {
        const int index = deviceBox.getSelectedId() - 1;
        if (index >= 0 && index < devices.size())
        {
            processor.selectedDevice = devices[index];
            openSelectedDevice();
        }
    };
    addAndMakeVisible (deviceBox);

    refreshButton.onClick = [this]
    {
        refreshDeviceList();
        openSelectedDevice();
    };
    addAndMakeVisible (refreshButton);

    statusLabel.setJustificationType (juce::Justification::centred);
    statusLabel.setColour (juce::Label::textColourId, juce::Colours::lightgrey);
    statusLabel.setText ("Escolha um dispositivo acima.", juce::dontSendNotification);
    addAndMakeVisible (statusLabel);

    setResizable (true, true);
    setResizeLimits (320, 200, 3840, 2160);
    setSize (processor.editorWidth, processor.editorHeight);

    refreshDeviceList();

    // Reabre o último dispositivo usado (salvo no projeto do FL)
    const int saved = devices.indexOf (processor.selectedDevice);
    if (saved >= 0)
    {
        deviceBox.setSelectedId (saved + 1, juce::dontSendNotification);
        openSelectedDevice();
    }

    applyControlsVisibility();
}

CameraEditor::~CameraEditor()
{
    processor.editorWidth  = getWidth();
    processor.editorHeight = getHeight();
    processor.controlsHidden = controlsHidden;
    closeDevice();
}

void CameraEditor::refreshDeviceList()
{
    devices = juce::CameraDevice::getAvailableDevices();

    deviceBox.clear (juce::dontSendNotification);
    for (int i = 0; i < devices.size(); ++i)
        deviceBox.addItem (devices[i], i + 1);

    const int current = devices.indexOf (processor.selectedDevice);
    if (current >= 0)
        deviceBox.setSelectedId (current + 1, juce::dontSendNotification);
}

void CameraEditor::closeDevice()
{
    viewer.reset();   // primeiro o viewer...
    camera.reset();   // ...depois a câmera
}

void CameraEditor::openSelectedDevice()
{
    closeDevice();

    const int index = devices.indexOf (processor.selectedDevice);
    if (index < 0)
    {
        statusLabel.setText ("Escolha um dispositivo acima.", juce::dontSendNotification);
        statusLabel.setVisible (true);
        resized();
        return;
    }

    statusLabel.setText ("Abrindo dispositivo...", juce::dontSendNotification);
    statusLabel.setVisible (true);

    juce::Component::SafePointer<CameraEditor> safeThis (this);

    juce::CameraDevice::openDeviceAsync (
        index,
        [safeThis] (juce::CameraDevice* device, const juce::String& error)
        {
            // Quem recebe o callback passa a ser dono do CameraDevice.
            std::unique_ptr<juce::CameraDevice> owned (device);

            if (safeThis == nullptr)
                return; // o editor foi fechado; 'owned' é destruído aqui

            auto& self = *safeThis;

            if (owned == nullptr)
            {
                self.statusLabel.setText ("Nao foi possivel abrir o dispositivo: " + error,
                                          juce::dontSendNotification);
                self.statusLabel.setVisible (true);
                return;
            }

            self.camera = std::move (owned);
            self.viewer.reset (self.camera->createViewerComponent());
            self.addAndMakeVisible (self.viewer.get());
            self.statusLabel.setVisible (false);
            self.resized();
        },
        640, 360,       // resolução mínima
        1920, 1080,     // resolução máxima
        true);          // alta qualidade
}

void CameraEditor::applyControlsVisibility()
{
    deviceBox.setVisible (! controlsHidden);
    refreshButton.setVisible (! controlsHidden);
    resized();
}

void CameraEditor::mouseDoubleClick (const juce::MouseEvent&)
{
    // Duplo clique alterna a barra de controles, deixando só o video.
    controlsHidden = ! controlsHidden;
    processor.controlsHidden = controlsHidden;
    applyControlsVisibility();
}

void CameraEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void CameraEditor::resized()
{
    auto area = getLocalBounds();

    if (! controlsHidden)
    {
        auto bar = area.removeFromTop (barHeight).reduced (4);
        refreshButton.setBounds (bar.removeFromRight (90));
        bar.removeFromRight (4);
        deviceBox.setBounds (bar);
    }

    statusLabel.setBounds (area);

    if (viewer != nullptr)
        viewer->setBounds (area);

    processor.editorWidth  = getWidth();
    processor.editorHeight = getHeight();
}
