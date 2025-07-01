#include "PluginProcessor.h"
#include "PluginEditor.h"

CAudioProcessor::CAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

CAudioProcessor::~CAudioProcessor() {}

const juce::String CAudioProcessor::getName() const { return JucePlugin_Name; }

bool CAudioProcessor::acceptsMidi() const { return false; }
bool CAudioProcessor::producesMidi() const { return false; }
bool CAudioProcessor::isMidiEffect() const { return false; }
double CAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int CAudioProcessor::getNumPrograms() { return 1; }
int CAudioProcessor::getCurrentProgram() { return 0; }
void CAudioProcessor::setCurrentProgram(int) {}
const juce::String CAudioProcessor::getProgramName(int) { return {}; }
void CAudioProcessor::changeProgramName(int, const juce::String&) {}

void CAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    dryWet.prepare(spec);
}

void CAudioProcessor::releaseResources() {}

void CAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const bool isBypassed = apvts.getRawParameterValue("bypass")->load() > 0.5f;
    if (isBypassed)
        return;

    float inputGain = apvts.getRawParameterValue("inputGain")->load();
    float outputGain = apvts.getRawParameterValue("gain")->load();
    float mix = apvts.getRawParameterValue("mix")->load();

    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    // Apply input gain
    buffer.applyGain(inputGain);

    // --- Place your future effect processing here ---

    // Apply output gain
    buffer.applyGain(outputGain);

    // Blend dry/wet
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* wet = buffer.getWritePointer(channel);
        auto* dry = dryBuffer.getReadPointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            wet[i] = dry[i] * (1.0f - mix) + wet[i] * mix;
        }
    }
}

bool CAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* CAudioProcessor::createEditor()
{
    return new CAudioProcessorEditor(*this, apvts);
}

void CAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (state.isValid())
    {
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    }
}

void CAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState)
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout CAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("inputGain", "Input Gain", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Output Gain", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        1.0f
    ));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CAudioProcessor();
}
