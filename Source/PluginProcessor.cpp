#include "PluginProcessor.h"
#include "PluginEditor.h"

class SlotParameter : public juce::AudioParameterFloat
{
public:
    SlotParameter(const juce::ParameterID& parameterID,
                  const juce::String& parameterName,
                  juce::NormalisableRange<float> normalisableRange,
                  float defaultValue)
        : juce::AudioParameterFloat(parameterID, parameterName, normalisableRange, defaultValue),
          currentName(parameterName)
    {
    }

    juce::String getName(int maximumStringLength) const override
    {
        if (maximumStringLength > 0 && currentName.length() > maximumStringLength)
            return currentName.substring(0, maximumStringLength);
        return currentName;
    }

    void setCustomName(const juce::String& newName)
    {
        currentName = newName;
    }

    juce::String getText(float normalisedValue, int) const override
    {
        int midiValue = juce::roundToInt(normalisedValue * 127.0f);
        return juce::String(midiValue);
    }

    float getValueForText(const juce::String& text) const override
    {
        int midiValue = text.getIntValue();
        midiValue = juce::jlimit(0, 127, midiValue);
        return midiValue / 127.0f;
    }

private:
    juce::String currentName;
};

SimpleCCProcessor::SimpleCCProcessor()
    : AudioProcessor(BusesProperties())
{
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        slotConfigs[i].name = "Slot " + juce::String(i + 1);
        slotConfigs[i].ccNumber = -1;
        slotConfigs[i].midiChannel = 1;
        slotConfigs[i].enabled = (i == 0);
        lastSentValues[i] = -1;
        slotActivity[i].store(false);

        auto paramId = "slot" + juce::String(i + 1);
        auto paramName = "Slot " + juce::String(i + 1);
        
        auto* param = new SlotParameter(
            juce::ParameterID(paramId, 1),
            paramName,
            juce::NormalisableRange<float>(0.0f, 1.0f),
            0.0f
        );
        
        slotParameters[i] = param;
        addParameter(param);
    }
}

SimpleCCProcessor::~SimpleCCProcessor()
{
}

void SimpleCCProcessor::updateSlotName(int slot, const juce::String& newName)
{
    if (slot >= 0 && slot < NUM_SLOTS)
    {
        slotConfigs[slot].name = newName;
        
        if (auto* param = dynamic_cast<SlotParameter*>(slotParameters[slot]))
        {
            param->setCustomName(newName);
            updateHostDisplay(juce::AudioProcessor::ChangeDetails().withParameterInfoChanged(true));
        }
    }
}

const juce::String SimpleCCProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleCCProcessor::acceptsMidi() const
{
    return true;
}

bool SimpleCCProcessor::producesMidi() const
{
    return true;
}

bool SimpleCCProcessor::isMidiEffect() const
{
    return true;
}

double SimpleCCProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleCCProcessor::getNumPrograms()
{
    return 1;
}

int SimpleCCProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleCCProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SimpleCCProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SimpleCCProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void SimpleCCProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    
    for (int i = 0; i < NUM_SLOTS; ++i)
        lastSentValues[i] = -1;
}

void SimpleCCProcessor::releaseResources()
{
}

bool SimpleCCProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    juce::ignoreUnused(layouts);
    return true;
}

void SimpleCCProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                      juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    
    // Don't clear midiMessages - we want to pass MIDI through!
    // Just add our CC messages to the existing MIDI

    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        if (!slotConfigs[i].enabled || slotConfigs[i].ccNumber < 0)
            continue;

        float value = slotParameters[i]->get();
        int ccValue = juce::roundToInt(value * 127.0f);
        
        if (ccValue != lastSentValues[i])
        {
            lastSentValues[i] = ccValue;
            
            int channel = slotConfigs[i].midiChannel;
            int ccNumber = slotConfigs[i].ccNumber;
            
            auto message = juce::MidiMessage::controllerEvent(channel, ccNumber, ccValue);
            midiMessages.addEvent(message, 0);
            
            slotActivity[i].store(true);
        }
    }
}

bool SimpleCCProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SimpleCCProcessor::createEditor()
{
    return new SimpleCCEditor(*this);
}

void SimpleCCProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::XmlElement xml("SimpleCCState");
    
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto* slotXml = xml.createNewChildElement("Slot");
        slotXml->setAttribute("index", i);
        slotXml->setAttribute("cc", slotConfigs[i].ccNumber);
        slotXml->setAttribute("channel", slotConfigs[i].midiChannel);
        slotXml->setAttribute("enabled", slotConfigs[i].enabled);
        slotXml->setAttribute("name", slotConfigs[i].name);
        slotXml->setAttribute("value", slotParameters[i]->get());
    }
    
    if (userPresetState.isNotEmpty())
        xml.setAttribute("userPreset", userPresetState);
    
    if (currentPresetManufacturer.isNotEmpty())
        xml.setAttribute("currentPresetManufacturer", currentPresetManufacturer);
    if (currentPresetName.isNotEmpty())
        xml.setAttribute("currentPresetName", currentPresetName);
    xml.setAttribute("isCurrentPresetUser", isCurrentPresetUser);
    
    copyXmlToBinary(xml, destData);
}

void SimpleCCProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);
    
    if (xml != nullptr && xml->hasTagName("SimpleCCState"))
    {
        for (auto* slotXml : xml->getChildIterator())
        {
            if (slotXml->hasTagName("Slot"))
            {
                int index = slotXml->getIntAttribute("index", -1);
                if (index >= 0 && index < NUM_SLOTS)
                {
                    slotConfigs[index].ccNumber = slotXml->getIntAttribute("cc", -1);
                    slotConfigs[index].midiChannel = slotXml->getIntAttribute("channel", 1);
                    slotConfigs[index].enabled = slotXml->getBoolAttribute("enabled", index == 0);
                    
                    juce::String name = slotXml->getStringAttribute("name", "Slot " + juce::String(index + 1));
                    updateSlotName(index, name);
                    
                    float value = (float)slotXml->getDoubleAttribute("value", 0.0);
                    slotParameters[index]->setValueNotifyingHost(value);
                }
            }
        }
        
        userPresetState = xml->getStringAttribute("userPreset", "");
        currentPresetManufacturer = xml->getStringAttribute("currentPresetManufacturer", "");
        currentPresetName = xml->getStringAttribute("currentPresetName", "");
        isCurrentPresetUser = xml->getBoolAttribute("isCurrentPresetUser", false);
    }
}

void SimpleCCProcessor::saveCurrentStateAsUserPreset(const juce::String& manufacturer, const juce::String& name)
{
    juce::XmlElement xml("UserPreset");
    xml.setAttribute("manufacturer", manufacturer);
    xml.setAttribute("name", name);
    
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto* slotXml = xml.createNewChildElement("Slot");
        slotXml->setAttribute("index", i);
        slotXml->setAttribute("cc", slotConfigs[i].ccNumber);
        slotXml->setAttribute("channel", slotConfigs[i].midiChannel);
        slotXml->setAttribute("enabled", slotConfigs[i].enabled);
        slotXml->setAttribute("name", slotConfigs[i].name);
        slotXml->setAttribute("value", slotParameters[i]->get());
    }
    userPresetState = xml.toString();
    
    juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("SimpleCC").getChildFile("Presets");
    
    if (!presetDir.exists())
        presetDir.createDirectory();
    
    juce::String safeFilename = (manufacturer + "_" + name).replaceCharacter(' ', '_')
        .replaceCharacter('/', '_').replaceCharacter('\\', '_')
        .replaceCharacter(':', '_').replaceCharacter('*', '_')
        .replaceCharacter('?', '_').replaceCharacter('"', '_')
        .replaceCharacter('<', '_').replaceCharacter('>', '_')
        .replaceCharacter('|', '_') + ".xml";
    
    juce::File presetFile = presetDir.getChildFile(safeFilename);
    presetFile.replaceWithText(userPresetState);
    
    currentPresetManufacturer = manufacturer;
    currentPresetName = name;
    isCurrentPresetUser = true;
}

void SimpleCCProcessor::loadUserPreset()
{
    juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("SimpleCC").getChildFile("Presets");
    
    juce::File presetFile = presetDir.getChildFile("UserPreset.xml");
    
    if (!presetFile.exists())
    {
        userPresetState = "";
        return;
    }
    
    userPresetState = presetFile.loadFileAsString();
    
    if (userPresetState.isEmpty())
        return;

    auto xml = juce::XmlDocument::parse(userPresetState);
    if (xml == nullptr || !xml->hasTagName("UserPreset"))
        return;

    for (auto* slotXml : xml->getChildIterator())
    {
        if (slotXml->hasTagName("Slot"))
        {
            int index = slotXml->getIntAttribute("index", -1);
            if (index >= 0 && index < NUM_SLOTS)
            {
                slotConfigs[index].ccNumber = slotXml->getIntAttribute("cc", -1);
                slotConfigs[index].midiChannel = slotXml->getIntAttribute("channel", 1);
                slotConfigs[index].enabled = slotXml->getBoolAttribute("enabled", index == 0);
                
                juce::String name = slotXml->getStringAttribute("name", "Slot " + juce::String(index + 1));
                updateSlotName(index, name);
                
                float value = (float)slotXml->getDoubleAttribute("value", 0.0);
                slotParameters[index]->setValueNotifyingHost(value);
            }
        }
    }
}

void SimpleCCProcessor::resetAllSlotConfigs()
{
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        slotConfigs[i].ccNumber = -1;
        slotConfigs[i].midiChannel = 1;
        slotConfigs[i].enabled = (i == 0);
        updateSlotName(i, "Slot " + juce::String(i + 1));
        slotParameters[i]->setValueNotifyingHost(0.0f);
    }
    userPresetState = "";
    currentPresetManufacturer = "";
    currentPresetName = "";
    isCurrentPresetUser = false;
}

void SimpleCCProcessor::loadUserPresetFromFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return;
    
    juce::String presetXmlString = file.loadFileAsString();
    if (presetXmlString.isEmpty())
        return;
    
    auto xml = juce::XmlDocument::parse(presetXmlString);
    if (xml == nullptr || !xml->hasTagName("UserPreset"))
        return;
    
    currentPresetManufacturer = xml->getStringAttribute("manufacturer", "");
    currentPresetName = xml->getStringAttribute("name", "");
    isCurrentPresetUser = true;
    
    for (auto* slotXml : xml->getChildIterator())
    {
        if (slotXml->hasTagName("Slot"))
        {
            int index = slotXml->getIntAttribute("index", -1);
            if (index >= 0 && index < NUM_SLOTS)
            {
                slotConfigs[index].ccNumber = slotXml->getIntAttribute("cc", -1);
                slotConfigs[index].midiChannel = slotXml->getIntAttribute("channel", 1);
                slotConfigs[index].enabled = slotXml->getBoolAttribute("enabled", index == 0);
                
                juce::String name = slotXml->getStringAttribute("name", "Slot " + juce::String(index + 1));
                updateSlotName(index, name);
                
                float value = (float)slotXml->getDoubleAttribute("value", 0.0);
                slotParameters[index]->setValueNotifyingHost(value);
            }
        }
    }
    
    userPresetState = presetXmlString;
}

void SimpleCCProcessor::loadDefaultPreset(const juce::String& manufacturer, const juce::String& name)
{
    currentPresetManufacturer = manufacturer;
    currentPresetName = name;
    isCurrentPresetUser = false;
}

std::vector<std::pair<juce::String, juce::String>> SimpleCCProcessor::getAllUserPresets()
{
    std::vector<std::pair<juce::String, juce::String>> presets;
    
    juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("SimpleCC").getChildFile("Presets");
    
    if (!presetDir.exists())
        return presets;
    
    juce::Array<juce::File> files = presetDir.findChildFiles(juce::File::findFiles, false, "*.xml");
    
    for (const auto& file : files)
    {
        auto xml = juce::XmlDocument::parse(file.loadFileAsString());
        if (xml && xml->hasTagName("UserPreset"))
        {
            juce::String manufacturer = xml->getStringAttribute("manufacturer", "Unknown");
            juce::String name = xml->getStringAttribute("name", "Unnamed");
            presets.push_back({manufacturer, name});
        }
    }
    
    return presets;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleCCProcessor();
}
