#pragma once

#include <JuceHeader.h>

constexpr int NUM_SLOTS = 16;

struct SlotConfig
{
    int ccNumber = -1;
    int midiChannel = 1;
    bool enabled = false;
    juce::String name = "Slot";
};

class SimpleCCProcessor : public juce::AudioProcessor
{
public:
    SimpleCCProcessor();
    ~SimpleCCProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    SlotConfig& getSlotConfig(int slot) { return slotConfigs[slot]; }
    juce::AudioParameterFloat* getSlotParameter(int slot) { return slotParameters[slot]; }
    void updateSlotName(int slot, const juce::String& newName);

    bool getSlotActivity(int slot) const { return slotActivity[slot].load(); }
    void clearSlotActivity(int slot) { slotActivity[slot].store(false); }

    void saveCurrentStateAsUserPreset(const juce::String& manufacturer, const juce::String& name);
    void loadUserPreset();
    void loadUserPresetFromFile(const juce::File& file);
    void loadDefaultPreset(const juce::String& manufacturer, const juce::String& name);
    std::vector<std::pair<juce::String, juce::String>> getAllUserPresets();
    void resetAllSlotConfigs();
    const juce::String& getUserPresetState() const { return userPresetState; }
    
    juce::String getCurrentPresetManufacturer() const { return currentPresetManufacturer; }
    juce::String getCurrentPresetName() const { return currentPresetName; }
    bool isUserPreset() const { return isCurrentPresetUser; }

private:
    std::array<SlotConfig, NUM_SLOTS> slotConfigs;
    std::array<juce::AudioParameterFloat*, NUM_SLOTS> slotParameters;
    std::array<int, NUM_SLOTS> lastSentValues;
    std::array<std::atomic<bool>, NUM_SLOTS> slotActivity;
    juce::String userPresetState;
    
    juce::String currentPresetManufacturer;
    juce::String currentPresetName;
    bool isCurrentPresetUser = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleCCProcessor)
};
