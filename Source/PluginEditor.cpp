#include "PluginEditor.h"

CAudioProcessorEditor::CAudioProcessorEditor(CAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
    : AudioProcessorEditor(&p), processorRef(p), apvts(state)
{
    _inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "inputGain", _inputGainSlider);
    _outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "gain", _outputGainSlider);
    _mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "mix", _mixSlider);
    _bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "bypass", _bypassToggle);

    _inputGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(_inputGainSlider);

    _inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    addAndMakeVisible(_inputGainLabel);

    _outputGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(_outputGainSlider);

    _outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    addAndMakeVisible(_outputGainLabel);

    _mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    _mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    _mixSlider.setRange(0.0, 1.0, 0.01);

    _mixSlider.textFromValueFunction = [](double value)
        {
            return juce::String(static_cast<int>(value * 100.0)) + "%";
        };

    _mixSlider.valueFromTextFunction = [](const juce::String& text)
        {
            return text.upToFirstOccurrenceOf("%", false, false).getDoubleValue() / 100.0;
        };

    addAndMakeVisible(_mixSlider);

    _bypassToggle.setButtonText("Bypass");
    addAndMakeVisible(_bypassToggle);

    setSize(300, 350);
}

CAudioProcessorEditor::~CAudioProcessorEditor() {}

void CAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("John Cage's 4'33'' Plugin v1.0 by potatoTeto", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void CAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto rowHeight = 40;
    const int spacing = 10;

    area.removeFromTop(30);

    _inputGainLabel.setBounds(area.removeFromTop(20));
    _inputGainSlider.setBounds(area.removeFromTop(rowHeight));

    area.removeFromTop(spacing);

    _outputGainLabel.setBounds(area.removeFromTop(20));
    _outputGainSlider.setBounds(area.removeFromTop(rowHeight));

    area.removeFromTop(spacing);

    _mixLabel.setBounds(area.removeFromTop(20));
    _mixSlider.setBounds(area.removeFromTop(80));

    area.removeFromTop(spacing);
    _bypassToggle.setBounds(area.removeFromTop(30));
}
