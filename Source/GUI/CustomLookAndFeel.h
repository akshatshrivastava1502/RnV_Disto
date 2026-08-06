#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

namespace Othrys
{
namespace GUI
{

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Set the default font for labels and text boxes
        setDefaultSansSerifTypefaceName ("Helvetica");
    }

    // This override is where the magic happens. We completely rewrite how a knob is drawn.
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, const float rotaryStartAngle,
                           const float rotaryEndAngle, juce::Slider& slider) override
    {
        juce::ignoreUnused (slider);

        // Load knob cap image
        auto knobImg = juce::ImageCache::getFromMemory (BinaryData::knob_cap_jpg, BinaryData::knob_cap_jpgSize);

        // Calculate the bounding box and center points
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f - 4.0f;
        auto centreX = bounds.getCentreX();
        auto centreY = bounds.getCentreY();
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // --- 1. Draw the background track ---
        juce::Path backgroundArc;
        backgroundArc.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour (juce::Colour::fromRGB (40, 40, 45)); // Deep grey track
        g.strokePath (backgroundArc, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // --- 2. Draw the Active Value Fill ---
        if (sliderPos > 0.0f)
        {
            juce::Path fillArc;
            fillArc.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
            g.setColour (juce::Colour::fromRGB (255, 110, 0)); // Glowing orange fill
            g.strokePath (fillArc, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // --- 3. Draw the Knob Cap Image (Rotated) ---
        if (knobImg.isValid())
        {
            // Calculate a rectangle slightly smaller than our track radius to nest the knob cap nicely
            auto capSize = rw - 14.0f;
            auto capRect = juce::Rectangle<float> (centreX - capSize / 2.0f, centreY - capSize / 2.0f, capSize, capSize);

            // We create a rotation transform around the center of the knob
            auto transform = juce::AffineTransform::rotation (angle, centreX, centreY);

            // Draw the knob cap with circular mask and rotation transform applied
            g.saveState();
            
            juce::Path clipPath;
            clipPath.addEllipse (capRect);
            g.reduceClipRegion (clipPath);
            
            g.addTransform (transform);
            g.drawImageWithin (knobImg, 
                              static_cast<int> (capRect.getX()), 
                              static_cast<int> (capRect.getY()), 
                              static_cast<int> (capRect.getWidth()), 
                              static_cast<int> (capRect.getHeight()), 
                              juce::RectanglePlacement::centred, 
                              false);
            g.restoreState();

            // --- 4. Draw Ambient Knob Shadow for depth ---
            g.setColour (juce::Colours::black.withAlpha (0.4f));
            g.drawEllipse (capRect.getX(), capRect.getY(), capRect.getWidth(), capRect.getHeight(), 2.0f);
        }
        else
        {
            // Fallback in case image is missing
            g.setColour (juce::Colours::darkgrey);
            g.fillEllipse (rx + 4.0f, ry + 4.0f, rw - 8.0f, rw - 8.0f);
        }
    }

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::ignoreUnused (shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        auto switchOnImg  = juce::ImageCache::getFromMemory (BinaryData::switch_on_jpg, BinaryData::switch_on_jpgSize);
        auto switchOffImg = juce::ImageCache::getFromMemory (BinaryData::switch_off_jpg, BinaryData::switch_off_jpgSize);
        auto ledOnImg     = juce::ImageCache::getFromMemory (BinaryData::led_on_jpg, BinaryData::led_on_jpgSize);
        auto ledOffImg    = juce::ImageCache::getFromMemory (BinaryData::led_off_jpg, BinaryData::led_off_jpgSize);

        auto bounds = button.getLocalBounds().toFloat();
        bool isToggled = button.getToggleState();
        
        // Bypass logic inversion
        bool isBypass = button.getButtonText().equalsIgnoreCase ("Bypass");
        bool isLedActive = isBypass ? !isToggled : isToggled;

        auto& activeSwitch = isToggled ? switchOnImg : switchOffImg;
        auto& activeLed = isLedActive ? ledOnImg : ledOffImg;

        if (activeSwitch.isValid() && activeLed.isValid())
        {
            auto size = juce::jmin (bounds.getWidth(), bounds.getHeight()) - 4.0f;
            auto switchRect = juce::Rectangle<float> (bounds.getX() + 2.0f, bounds.getCentreY() - size / 2.0f, size, size);

            // Draw switch
            g.drawImageWithin (activeSwitch, 
                              static_cast<int> (switchRect.getX()), 
                              static_cast<int> (switchRect.getY()), 
                              static_cast<int> (switchRect.getWidth()), 
                              static_cast<int> (switchRect.getHeight()), 
                              juce::RectanglePlacement::centred, 
                              false);

            // Draw LED indicator (slightly smaller than switch)
            auto ledSize = size * 0.6f;
            auto ledRect = juce::Rectangle<float> (switchRect.getRight() + 6.0f, bounds.getCentreY() - ledSize / 2.0f, ledSize, ledSize);
            g.drawImageWithin (activeLed, 
                              static_cast<int> (ledRect.getX()), 
                              static_cast<int> (ledRect.getY()), 
                              static_cast<int> (ledRect.getWidth()), 
                              static_cast<int> (ledRect.getHeight()), 
                              juce::RectanglePlacement::centred, 
                              false);

            // Draw label
            g.setColour (juce::Colours::white);
            g.setFont (juce::FontOptions (13.0f));
            auto textRect = bounds.withTrimmedLeft (switchRect.getWidth() + ledSize + 14.0f);
            g.drawText (button.getButtonText(), textRect, juce::Justification::centredLeft, true);
        }
        else
        {
            g.setColour (isToggled ? juce::Colour::fromRGB (255, 110, 0) : juce::Colours::grey);
            g.fillEllipse (bounds.getX() + 2.0f, bounds.getCentreY() - 8.0f, 16.0f, 16.0f);
            
            g.setColour (juce::Colours::white);
            g.setFont (juce::FontOptions (13.0f));
            g.drawText (button.getButtonText(), bounds.withTrimmedLeft (24.0f), juce::Justification::centredLeft, true);
        }
    }
};

} // namespace GUI
} // namespace Othrys