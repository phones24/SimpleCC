#pragma once

#include "PluginProcessor.h"
#include "InstrumentPresets.h"

class MidiActivityIndicator : public juce::Component
{
public:
    MidiActivityIndicator() : isActive(false) {}
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(4);
        
        if (isActive)
            g.setColour(juce::Colour(0xff00ff00));
        else
            g.setColour(juce::Colour(0xff404040));
        
        g.fillRoundedRectangle(bounds, 4.0f);
    }
    
    void setActive(bool active)
    {
        if (isActive != active)
        {
            isActive = active;
            repaint();
        }
    }
    
    bool getActive() const { return isActive; }

private:
    bool isActive;
};

class SlotRowComponent : public juce::Component,
                         public juce::Timer
{
public:
    SlotRowComponent(SimpleCCProcessor& p, int slotIndex);
    ~SlotRowComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void updateEnabledState();
    void timerCallback() override;
    void refreshFromProcessor();

private:
    SimpleCCProcessor& processor;
    int index;

    juce::Label slotNumberLabel;
    juce::ToggleButton enableButton;
    juce::TextEditor ccInput;
    juce::ComboBox channelSelector;
    juce::TextEditor nameInput;
    MidiActivityIndicator activityIndicator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SlotRowComponent)
};

class SimpleCCEditor : public juce::AudioProcessorEditor
{
public:
    explicit SimpleCCEditor(SimpleCCProcessor&);
    ~SimpleCCEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void applyPreset(int presetIndex);
    void saveCustomPreset();
    void resetAllSlots();
    void drawLogo(juce::Graphics& g, juce::Rectangle<float> bounds);
    void rebuildPresetDropdown();
    void selectUserPreset(const juce::String& manufacturer, const juce::String& name);
    void restorePresetSelection();

private:
    SimpleCCProcessor& processorRef;
    
    juce::Label presetLabel;
    juce::ComboBox presetSelector;
    juce::TextButton saveButton;
    juce::TextButton resetButton;
    
    juce::Label headerSlot;
    juce::Label headerCC;
    juce::Label headerCh;
    juce::Label headerName;
    juce::Label headerActivity;
    
    juce::Label versionLabel;
    
    juce::OwnedArray<SlotRowComponent> slotRows;
    juce::Viewport viewport;
    juce::Component slotContainer;
    
    int userPresetStartId = 2;
    int defaultPresetStartId = 2;
    std::vector<std::pair<juce::String, juce::String>> cachedUserPresets;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleCCEditor)
};
