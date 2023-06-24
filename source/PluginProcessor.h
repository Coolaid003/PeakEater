#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>

#include "Controller/WaveShaperController.h"
#include "DSP/LevelMeter.h"
#include "DSP/OversampledWaveShaper.h"

class PeakEaterAudioProcessor : public juce::AudioProcessor
{
public:
    PeakEaterAudioProcessor();
    ~PeakEaterAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Hack to preserve plugin size
    struct PluginSizeConstraints
    {
        int const minWidth = 640;
        int const minHeight = 400;
        int const maxWidth = 3840;
        int const maxHeight = 2400;
        double const aspectRatio = 16.0 / 10.0;
    };
    struct PluginSizeState
    {
        int width;
        int height;
    };
    PluginSizeConstraints getPluginSizeConstraints() const;
    void setPluginSizeState (PluginSizeState const&& pluginSizeState);
    PluginSizeState getPluginSizeState() const;

private:
    std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
    juce::AudioParameterFloat* mInputGain;
    juce::AudioParameterFloat* mOutputGain;
    juce::AudioParameterBool* mLinkInOut;
    juce::AudioParameterBool* mBypass;
    juce::AudioParameterFloat* mCeiling;
    juce::AudioParameterChoice* mClippingType;
    juce::AudioParameterChoice* mOversampleRate;

    pe::controller::WaveShaperController<float> mWaveShaperController;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mLevelMeterPostIn;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mLevelMeterPostClipper;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mLevelMeterPostOut;

    PluginSizeConstraints mPluginSizeConstraints;
    PluginSizeState mPluginSizeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakEaterAudioProcessor)
};