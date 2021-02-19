#pragma once

#include <JuceHeader.h>

#include "Header.h"
#include "WorkingArea.h"
#include "Footer.h"

namespace layout
{

//==============================================================================
class CentralPanel : public juce::Component, public juce::Button::Listener
{
public:
    //==============================================================================
    CentralPanel(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        workingArea(p, vts),
        footer(p, vts)
    {
        addAndMakeVisible(header);
        addAndMakeVisible(workingArea);
        addAndMakeVisible(footer);
        
        footer.addLinkInOutListener(this);
        footer.addBypassListener(this);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (3)),
            Track (Fr (18)),
            Track (Fr (2))
        };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = {
            Item(header),
            Item(workingArea),
            Item(footer)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void buttonClicked (juce::Button* button) override
    {
        if (button->getName() == "LinkInOut")
        {
            workingArea.setOutputGainEnabled (!button->getToggleState());
        }
        if (button->getName() == "Bypass")
        {
            workingArea.setEnabled (!button->getToggleState());
            footer.setLinkEnabled (!button->getToggleState());
        }
    }
    
    void buttonStateChanged (juce::Button*) override
    {}
    
private:
    layout::Header      header;
    layout::WorkingArea workingArea;
    layout::Footer      footer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentralPanel)
};


}
