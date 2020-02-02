/**
 *  MainComponent.cpp
 *
 *  MIT License
 *
 *  Copyright (c) 2018, Tom Clarke
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

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    shutdownOpenGL();
}

void MainComponent::initialise()
{
}

void MainComponent::shutdown()
{
}

void MainComponent::render()
{
    OpenGLHelpers::clear (Colours::black);

    const auto& e = openGLContext.extensions;
    
    const auto bounds = getLocalBounds().toFloat();
    const auto& shader = chl::PathShaderProgram::select (openGLContext);
    shader.params.set (bounds.getWidth(), bounds.getHeight());

    const auto vertices { chl::VertexGenerator::fromPath (path) };
    
    GLuint vertexBuffer = 0;
    e.glGenBuffers (1, &vertexBuffer);
    e.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    e.glBufferData (GL_ARRAY_BUFFER, sizeof (GLshort) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    auto index = (GLuint) shader.params.position.attributeID;
    e.glVertexAttribPointer (index, 2, GL_SHORT, GL_FALSE, 4, nullptr);
    e.glEnableVertexAttribArray (index);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);
    
    glDrawArrays (GL_TRIANGLES, 0, GLsizei (vertices.size() / 2));
        
    e.glBindBuffer (GL_ARRAY_BUFFER, 0);
    e.glUseProgram (0);
    e.glDisableVertexAttribArray (index);
    e.glDeleteBuffers (1, &vertexBuffer);
}

void MainComponent::paint (Graphics& g)
{
    g.setColour (Colours::orangered);
    g.strokePath(path, PathStrokeType (1.f));
}

void MainComponent::resized()
{
    GlyphArrangement glyphs;
    glyphs.addFittedText (Font (float (getHeight())),
                          "e",
                          0.f,
                          0.f,
                          float (getWidth()),
                          float (getHeight()),
                          Justification::centred,
                          1);
    path.clear();
    glyphs.createPath (path);
}
