#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

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

        // Calculate the bounding box and center points
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();

        // We shrink the radius slightly so the thick strokes don't clip outside the bounds
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
        g.strokePath (backgroundArc, juce::PathStrokeType (5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // --- 2. Draw the Active Value Fill ---
        if (sliderPos > 0.0f)
        {
            juce::Path fillArc;
            fillArc.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
            // Premium "Neon Orange" accent color for the active value
            g.setColour (juce::Colour::fromRGB (255, 85, 0));
            g.strokePath (fillArc, juce::PathStrokeType (5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // --- 3. Draw the Inner Knob Body ---
        g.setColour (juce::Colour::fromRGB (20, 20, 23)); // Dark inner circle
        g.fillEllipse (rx + 4.0f, ry + 4.0f, rw - 8.0f, rw - 8.0f);

        // Add a subtle metallic outline for depth
        g.setColour (juce::Colour::fromRGB (70, 70, 75));
        g.drawEllipse (rx + 4.0f, ry + 4.0f, rw - 8.0f, rw - 8.0f, 1.5f);

        // --- 4. Draw the Pointer ---
        juce::Path pointer;
        auto pointerLength = radius * 0.4f;
        auto pointerThickness = 3.0f;
        pointer.addRectangle (-pointerThickness * 0.5f, -radius + 6.0f, pointerThickness, pointerLength);

        // Rotate the pointer to match the current value angle
        pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        g.setColour (juce::Colours::white);
        g.fillPath (pointer);
    }
};

} // namespace GUI
} // namespace Othrys