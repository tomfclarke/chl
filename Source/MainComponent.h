/**
 *  MainComponent.h
 *
 *  MIT License
 *
 *  Copyright (c) 2020, Tom Clarke
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <JuceHeader.h>

class MainComponent   : public OpenGLAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void initialise() override;
    void shutdown() override;
    void render() override;

    void paint (Graphics& g) override;
    void resized() override;

private:
    void renderToWindingFrameBuffer();
    void renderToMainFrameBuffer();
    void setAdditiveBlending();
    void setAlphaBlending();
    void updateVertexData();

    const OpenGLExtensionFunctions& e = openGLContext.extensions;

    Path path;

    std::vector<GLshort> vertices;
    std::mutex verticesMutex;

    std::atomic<bool> boundsHaveChanged { false };
    OpenGLFrameBuffer windingFrameBuffer;

    GLuint vertexBuffer { 0 };
    GLuint screenBuffer { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
