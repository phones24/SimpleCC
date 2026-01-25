#include "PluginProcessor.h"
#include "PluginEditor.h"

SlotRowComponent::SlotRowComponent(SimpleCCProcessor& p, int slotIndex)
    : processor(p), index(slotIndex)
{
    auto& config = processor.getSlotConfig(index);

    slotNumberLabel.setText(juce::String(index + 1), juce::dontSendNotification);
    slotNumberLabel.setJustificationType(juce::Justification::centred);
    slotNumberLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(slotNumberLabel);

    enableButton.setToggleState(config.enabled, juce::dontSendNotification);
    enableButton.onClick = [this]() {
        processor.getSlotConfig(index).enabled = enableButton.getToggleState();
        updateEnabledState();
    };
    addAndMakeVisible(enableButton);

    if (config.ccNumber >= 0)
        ccInput.setText(juce::String(config.ccNumber), false);
    else
        ccInput.setText("", false);
    ccInput.setJustification(juce::Justification::centred);
    ccInput.setInputRestrictions(3, "0123456789");
    ccInput.onFocusLost = [this]() {
        juce::String text = ccInput.getText();
        if (text.isEmpty())
        {
            processor.getSlotConfig(index).ccNumber = -1;
        }
        else
        {
            int cc = text.getIntValue();
            cc = juce::jlimit(0, 127, cc);
            processor.getSlotConfig(index).ccNumber = cc;
            ccInput.setText(juce::String(cc), false);
        }
    };
    ccInput.onReturnKey = [this]() {
        juce::String text = ccInput.getText();
        if (text.isEmpty())
        {
            processor.getSlotConfig(index).ccNumber = -1;
        }
        else
        {
            int cc = text.getIntValue();
            cc = juce::jlimit(0, 127, cc);
            processor.getSlotConfig(index).ccNumber = cc;
            ccInput.setText(juce::String(cc), false);
        }
    };
    addAndMakeVisible(ccInput);

    for (int ch = 1; ch <= 16; ++ch)
        channelSelector.addItem(juce::String(ch), ch);
    channelSelector.setSelectedId(config.midiChannel, juce::dontSendNotification);
    channelSelector.onChange = [this]() {
        processor.getSlotConfig(index).midiChannel = channelSelector.getSelectedId();
    };
    addAndMakeVisible(channelSelector);

    nameInput.setText(config.name, false);
    nameInput.setJustification(juce::Justification::centredLeft);
    nameInput.onFocusLost = [this]() {
        processor.updateSlotName(index, nameInput.getText());
    };
    nameInput.onReturnKey = [this]() {
        processor.updateSlotName(index, nameInput.getText());
    };
    addAndMakeVisible(nameInput);

    addAndMakeVisible(activityIndicator);

    updateEnabledState();
    
    startTimerHz(30);
}

void SlotRowComponent::timerCallback()
{
    if (processor.getSlotActivity(index))
    {
        activityIndicator.setActive(true);
        processor.clearSlotActivity(index);
    }
    else
    {
        activityIndicator.setActive(false);
    }
}

void SlotRowComponent::refreshFromProcessor()
{
    auto& config = processor.getSlotConfig(index);
    
    enableButton.setToggleState(config.enabled, juce::dontSendNotification);
    
    if (config.ccNumber >= 0)
        ccInput.setText(juce::String(config.ccNumber), false);
    else
        ccInput.setText("", false);
    
    channelSelector.setSelectedId(config.midiChannel, juce::dontSendNotification);
    
    nameInput.setText(config.name, false);
    
    updateEnabledState();
}

void SlotRowComponent::updateEnabledState()
{
    bool enabled = enableButton.getToggleState();
    
    ccInput.setEnabled(enabled);
    channelSelector.setEnabled(enabled);
    nameInput.setEnabled(enabled);
    
    float alpha = enabled ? 1.0f : 0.5f;
    ccInput.setAlpha(alpha);
    channelSelector.setAlpha(alpha);
    nameInput.setAlpha(alpha);
    slotNumberLabel.setAlpha(alpha);
}

void SlotRowComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    if (index % 2 == 0)
        g.setColour(juce::Colour(0xff2a2a2a));
    else
        g.setColour(juce::Colour(0xff252525));
    
    g.fillRect(bounds);
    
    g.setColour(juce::Colour(0xff3a3a3a));
    g.drawLine(0, bounds.getHeight(), bounds.getWidth(), bounds.getHeight(), 1.0f);
}

void SlotRowComponent::resized()
{
    auto bounds = getLocalBounds().reduced(4, 2);
    
    int slotNumWidth = 30;
    int enableWidth = 30;
    int ccWidth = 70;
    int channelWidth = 70;
    int activityWidth = 20;
    int gap = 8;
    
    slotNumberLabel.setBounds(bounds.removeFromLeft(slotNumWidth));
    bounds.removeFromLeft(gap);
    
    enableButton.setBounds(bounds.removeFromLeft(enableWidth));
    bounds.removeFromLeft(gap);
    
    ccInput.setBounds(bounds.removeFromLeft(ccWidth));
    bounds.removeFromLeft(gap);
    
    channelSelector.setBounds(bounds.removeFromLeft(channelWidth));
    bounds.removeFromLeft(gap);
    
    auto activityBounds = bounds.removeFromRight(activityWidth);
    activityIndicator.setBounds(activityBounds);
    bounds.removeFromRight(gap);
    
    nameInput.setBounds(bounds);
}

SimpleCCEditor::SimpleCCEditor(SimpleCCProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    presetLabel.setText("Instrument:", juce::dontSendNotification);
    presetLabel.setJustificationType(juce::Justification::centredRight);
    presetLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(presetLabel);
    
    presetSelector.setTextWhenNothingSelected("");
    
    presetSelector.onChange = [this]() {
        int selectedId = presetSelector.getSelectedId();
        
        if (selectedId == 1)
        {
            // Empty preset selected - clear preset tracking
            processorRef.resetAllSlotConfigs();
            for (auto* row : slotRows)
            {
                row->refreshFromProcessor();
            }
        }
        else if (selectedId >= userPresetStartId && selectedId < userPresetStartId + (int)cachedUserPresets.size())
        {
            int userPresetIndex = selectedId - userPresetStartId;
            juce::File presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("SimpleCC").getChildFile("Presets");
            
            juce::String manufacturer = cachedUserPresets[userPresetIndex].first;
            juce::String name = cachedUserPresets[userPresetIndex].second;
            juce::String safeFilename = (manufacturer + "_" + name).replaceCharacter(' ', '_')
                .replaceCharacter('/', '_').replaceCharacter('\\', '_')
                .replaceCharacter(':', '_').replaceCharacter('*', '_')
                .replaceCharacter('?', '_').replaceCharacter('"', '_')
                .replaceCharacter('<', '_').replaceCharacter('>', '_')
                .replaceCharacter('|', '_') + ".xml";
            
            juce::File presetFile = presetDir.getChildFile(safeFilename);
            processorRef.loadUserPresetFromFile(presetFile);
            
            for (auto* row : slotRows)
            {
                row->refreshFromProcessor();
            }
        }
        else if (selectedId >= defaultPresetStartId)
        {
            int adjustedIndex = selectedId - defaultPresetStartId;
            applyPreset(adjustedIndex);
        }
    };
    
    rebuildPresetDropdown();
    restorePresetSelection();
    addAndMakeVisible(presetSelector);

    saveButton.setButtonText("Save");
    saveButton.onClick = [this]() { saveCustomPreset(); };
    addAndMakeVisible(saveButton);

    resetButton.setButtonText("Reset");
    resetButton.onClick = [this]() { resetAllSlots(); };
    addAndMakeVisible(resetButton);
    
    versionLabel.setText("Version: 1.0.1", juce::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::centredLeft);
    versionLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey.withAlpha(0.6f));
    versionLabel.setFont(juce::Font(10.0f, juce::Font::plain));
    addAndMakeVisible(versionLabel);
    
    auto setupHeader = [](juce::Label& label, const juce::String& text, juce::Justification just = juce::Justification::centred) {
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(just);
        label.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    };
    
    setupHeader(headerSlot, "SLOT");
    setupHeader(headerCC, "CC");
    setupHeader(headerCh, "CH");
    setupHeader(headerName, "NAME", juce::Justification::centredLeft);
    setupHeader(headerActivity, "");
    
    addAndMakeVisible(headerSlot);
    addAndMakeVisible(headerCC);
    addAndMakeVisible(headerCh);
    addAndMakeVisible(headerName);
    addAndMakeVisible(headerActivity);

    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto* row = new SlotRowComponent(processorRef, i);
        slotRows.add(row);
        slotContainer.addAndMakeVisible(row);
    }

    viewport.setViewedComponent(&slotContainer, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);

    processorRef.loadUserPreset();
    for (auto* row : slotRows)
    {
        row->refreshFromProcessor();
    }

    int rowHeight = 28;
    int logoHeight = 50;
    int presetBarHeight = 32;
    int headerHeight = 28;
    int totalHeight = logoHeight + presetBarHeight + headerHeight + (NUM_SLOTS * rowHeight) + 20;
    
    setSize(480, totalHeight);
    setResizable(true, true);
    setResizeLimits(380, 200, 800, 700);
}

SimpleCCEditor::~SimpleCCEditor()
{
}

void SimpleCCEditor::drawLogo(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(28.0f, juce::Font::bold | juce::Font::italic));
    
    auto titleBounds = bounds.removeFromTop(28.0f);
    g.drawText("SimpleCC", titleBounds, juce::Justification::centredLeft, false);
    
    g.setFont(juce::Font(9.0f, juce::Font::plain));
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.drawText("by Randomware Audio", bounds, juce::Justification::topLeft, false);
}

void SimpleCCEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a2a));
    
    juce::Rectangle<float> headerArea(0.0f, 0.0f, (float)getWidth(), 82.0f);
    juce::ColourGradient gradient(
        juce::Colour(0xff5982A8), headerArea.getX(), headerArea.getY(),
        juce::Colour(0xff2D8C65), headerArea.getRight(), headerArea.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRect(headerArea);
    
    drawLogo(g, juce::Rectangle<float>(8.0f, 8.0f, 200.0f, 40.0f));
}

void SimpleCCEditor::resized()
{
    auto bounds = getLocalBounds();
    
    bounds.removeFromTop(50);
    
    auto presetBounds = bounds.removeFromTop(32).reduced(8, 4);
    presetLabel.setBounds(presetBounds.removeFromLeft(80));
    presetBounds.removeFromLeft(8);
    presetSelector.setBounds(presetBounds.removeFromLeft(250));
    presetBounds.removeFromLeft(8);
    saveButton.setBounds(presetBounds.removeFromLeft(50));
    presetBounds.removeFromLeft(8);
    resetButton.setBounds(presetBounds.removeFromLeft(50));
    
    auto headerBounds = bounds.removeFromTop(28).reduced(4, 2);
    
    int slotNumWidth = 30;
    int enableWidth = 30;
    int ccWidth = 70;
    int channelWidth = 70;
    int activityWidth = 20;
    int gap = 8;
    
    int slotHeaderWidth = slotNumWidth + gap + enableWidth;
    headerSlot.setBounds(headerBounds.removeFromLeft(slotHeaderWidth));
    
    headerCC.setBounds(headerBounds.removeFromLeft(ccWidth));
    headerBounds.removeFromLeft(gap);
    headerCh.setBounds(headerBounds.removeFromLeft(channelWidth));
    headerBounds.removeFromLeft(gap);
    
    headerActivity.setBounds(headerBounds.removeFromRight(activityWidth));
    headerBounds.removeFromRight(gap);
    
    headerName.setBounds(headerBounds);
    
    auto viewportBounds = bounds;
    auto versionBounds = viewportBounds.removeFromBottom(20).reduced(8, 2);
    versionLabel.setBounds(versionBounds);
    
    viewport.setBounds(viewportBounds);
    viewport.setScrollBarsShown(false, false);
    
    int rowHeight = 28;
    int containerHeight = NUM_SLOTS * rowHeight;
    int containerWidth = viewportBounds.getWidth();
    
    slotContainer.setSize(containerWidth, containerHeight);
    
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        slotRows[i]->setBounds(0, i * rowHeight, containerWidth, rowHeight);
    }
}

void SimpleCCEditor::applyPreset(int presetIndex)
{
    auto presets = getInstrumentPresets();
    
    if (presetIndex < 0 || presetIndex >= (int)presets.size())
        return;
    
    const auto& preset = presets[presetIndex];
    
    processorRef.loadDefaultPreset(preset.manufacturer, preset.name);
    
    int numMappings = juce::jmin((int)preset.mappings.size(), NUM_SLOTS);
    
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        auto& config = processorRef.getSlotConfig(i);
        
        if (i < numMappings)
        {
            config.enabled = true;
            config.ccNumber = preset.mappings[i].ccNumber;
            processorRef.updateSlotName(i, preset.mappings[i].paramName);
        }
        else
        {
            config.enabled = false;
            config.ccNumber = -1;
            processorRef.updateSlotName(i, "Slot " + juce::String(i + 1));
        }
    }
    
    for (auto* row : slotRows)
    {
        row->refreshFromProcessor();
    }
}

void SimpleCCEditor::saveCustomPreset()
{
    auto* alertWindow = new juce::AlertWindow("Save Preset", "Enter preset details:", juce::MessageBoxIconType::QuestionIcon);
    
    juce::String currentManufacturer = processorRef.getCurrentPresetManufacturer();
    juce::String currentName = processorRef.getCurrentPresetName();
    
    alertWindow->addTextEditor("manufacturer", currentManufacturer, "Manufacturer:");
    alertWindow->addTextEditor("name", currentName, "Instrument Name:");
    
    alertWindow->addButton("Save", 1, juce::KeyPress(juce::KeyPress::returnKey));
    alertWindow->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    
    alertWindow->enterModalState(true, juce::ModalCallbackFunction::create([this, alertWindow](int result) {
        if (result == 1)
        {
            juce::String manufacturer = alertWindow->getTextEditorContents("manufacturer").trim();
            juce::String name = alertWindow->getTextEditorContents("name").trim();
            
            if (manufacturer.isEmpty())
                manufacturer = "Custom";
            
            if (name.isEmpty())
            {
                juce::AlertWindow::showMessageBoxAsync(
                    juce::MessageBoxIconType::WarningIcon,
                    "Invalid Name",
                    "Instrument name cannot be empty. Please enter a name for the preset."
                );
                delete alertWindow;
                return;
            }
            
            processorRef.saveCurrentStateAsUserPreset(manufacturer, name);
            rebuildPresetDropdown();
            selectUserPreset(manufacturer, name);
        }
        delete alertWindow;
    }), true);
}

void SimpleCCEditor::resetAllSlots()
{
    auto options = juce::MessageBoxOptions()
        .withIconType(juce::MessageBoxIconType::QuestionIcon)
        .withTitle("Reset All Slots")
        .withMessage("Are you sure you want to reset all CC slots to their initial default state? This action cannot be undone.")
        .withButton("Reset")
        .withButton("Cancel");
    
    juce::AlertWindow::showAsync(options, [this](int result) {
        if (result == 1)
        {
            processorRef.resetAllSlotConfigs();
            for (auto* row : slotRows)
            {
                row->refreshFromProcessor();
            }
            presetSelector.setSelectedId(1, juce::dontSendNotification);
        }
    });
}

void SimpleCCEditor::rebuildPresetDropdown()
{
    presetSelector.clear(juce::dontSendNotification);
    
    int nextId = 1;
    presetSelector.addItem("", nextId++);
    
    cachedUserPresets = processorRef.getAllUserPresets();
    userPresetStartId = nextId;
    
    for (const auto& preset : cachedUserPresets)
    {
        presetSelector.addItem(preset.first + " - " + preset.second, nextId++);
    }
    
    if (!cachedUserPresets.empty())
    {
        presetSelector.addSeparator();
    }
    
    auto presets = getInstrumentPresets();
    defaultPresetStartId = nextId;
    
    std::vector<juce::String> manufacturers;
    for (const auto& preset : presets)
    {
        bool found = false;
        for (const auto& m : manufacturers)
        {
            if (m == preset.manufacturer)
            {
                found = true;
                break;
            }
        }
        if (!found)
            manufacturers.push_back(preset.manufacturer);
    }
    
    for (const auto& manufacturer : manufacturers)
    {
        presetSelector.addSectionHeading(manufacturer);
        
        for (int i = 0; i < (int)presets.size(); ++i)
        {
            if (presets[i].manufacturer == manufacturer)
            {
                presetSelector.addItem(presets[i].name, nextId + i);
            }
        }
    }
}

void SimpleCCEditor::selectUserPreset(const juce::String& manufacturer, const juce::String& name)
{
    for (int i = 0; i < (int)cachedUserPresets.size(); ++i)
    {
        if (cachedUserPresets[i].first == manufacturer && cachedUserPresets[i].second == name)
        {
            presetSelector.setSelectedId(userPresetStartId + i, juce::dontSendNotification);
            return;
        }
    }
}

void SimpleCCEditor::restorePresetSelection()
{
    juce::String manufacturer = processorRef.getCurrentPresetManufacturer();
    juce::String name = processorRef.getCurrentPresetName();
    
    if (manufacturer.isEmpty() && name.isEmpty())
    {
        presetSelector.setSelectedId(1, juce::dontSendNotification);
        return;
    }
    
    if (processorRef.isUserPreset())
    {
        selectUserPreset(manufacturer, name);
    }
    else
    {
        auto presets = getInstrumentPresets();
        for (int i = 0; i < (int)presets.size(); ++i)
        {
            if (presets[i].manufacturer == manufacturer && presets[i].name == name)
            {
                presetSelector.setSelectedId(defaultPresetStartId + i, juce::dontSendNotification);
                return;
            }
        }
    }
}
