#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DPCMBitcrusherAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DPCMBitcrusherAudioProcessorEditor(DPCMBitcrusherAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~DPCMBitcrusherAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DPCMBitcrusherAudioProcessor& processorRef;
    juce::AudioProcessorValueTreeState& apvts;

    juce::Slider _inputGainSlider;
    juce::Slider _outputGainSlider;
    juce::Slider _mixSlider;
    juce::ToggleButton _bypassToggle;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _bypassAttachment;

    juce::Label _inputGainLabel;
    juce::Label _outputGainLabel;
    juce::Label _mixLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DPCMBitcrusherAudioProcessorEditor)
};
