#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

namespace RnVDisto
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

            // Draw the knob cap with rotation transform applied
            g.saveState();
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
};

} // namespace GUI
} // namespace RnVDisto