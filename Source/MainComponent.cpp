/**
 *  MainComponent.cpp
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
    e.glGenBuffers (1, &vertexBuffer);
    e.glGenBuffers (1, &screenBuffer);
}

void MainComponent::shutdown()
{
    e.glDeleteBuffers (1, &vertexBuffer);
    e.glDeleteBuffers (1, &screenBuffer);
}

void MainComponent::render()
{
    if (boundsHaveChanged)
    {
        GLint viewport[4] { 0 };
        glGetIntegerv (GL_VIEWPORT, viewport);
        windingFrameBuffer.initialise (openGLContext, viewport[2], viewport[3]);

        updateVertexData();
        boundsHaveChanged = false;
    }

    windingFrameBuffer.makeCurrentAndClear();

    renderToWindingFrameBuffer();

    windingFrameBuffer.releaseAsRenderingTarget();

    renderToMainFrameBuffer();
}

void MainComponent::paint (Graphics& g)
{
    g.setColour (Colours::orangered);
    g.strokePath (path, PathStrokeType (1.f));
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

    std::scoped_lock l (verticesMutex);
    vertices = chl::VertexGenerator::fromPath (path);

    boundsHaveChanged = true;
}

void MainComponent::renderToWindingFrameBuffer()
{
    const auto& shader = chl::WindingShader::select (openGLContext);
    shader.params.set (windingFrameBuffer.getWidth(), windingFrameBuffer.getHeight());

    e.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    auto index = (GLuint) shader.params.position.attributeID;
    e.glVertexAttribPointer (index, 2, GL_SHORT, GL_FALSE, 0, nullptr);
    e.glEnableVertexAttribArray (index);

    setAdditiveBlending();

    if (e.glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glDrawArrays (GL_TRIANGLES, 0, GLsizei (vertices.size() / 2));
    }

    e.glBindBuffer (GL_ARRAY_BUFFER, 0);
    e.glUseProgram (0);
    e.glDisableVertexAttribArray (index);
}

void MainComponent::renderToMainFrameBuffer()
{
    OpenGLHelpers::clear (Colours::black);

    const auto bounds = getLocalBounds().toFloat();
    const auto& shader = chl::FillShader::select (openGLContext);
    shader.params.set (bounds.getWidth(), bounds.getHeight());
    
    auto left   = (GLshort) bounds.getX();
    auto top    = (GLshort) bounds.getY();
    auto right  = (GLshort) bounds.getRight();
    auto bottom = (GLshort) bounds.getBottom();
    const GLshort screenQuad[] = { left, bottom, right, bottom, left, top, right, top };

    e.glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, windingFrameBuffer.getTextureID());

    e.glBindBuffer (GL_ARRAY_BUFFER, screenBuffer);
    e.glBufferData (GL_ARRAY_BUFFER, sizeof (screenQuad), screenQuad, GL_STATIC_DRAW);
    
    auto index = (GLuint) shader.params.position.attributeID;
    e.glVertexAttribPointer (index, 2, GL_SHORT, GL_FALSE, 0, nullptr);
    e.glEnableVertexAttribArray (index);

    setAlphaBlending();

    if (e.glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
    }

    glBindTexture (GL_TEXTURE_2D, 0);
    e.glBindBuffer (GL_ARRAY_BUFFER, 0);
    e.glUseProgram (0);
    e.glDisableVertexAttribArray (index);
}

void MainComponent::setAdditiveBlending()
{
    glBlendFunc (GL_ONE, GL_ONE);
    glEnable (GL_BLEND);
}

void MainComponent::setAlphaBlending()
{
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);
}

void MainComponent::updateVertexData()
{
    std::scoped_lock l (verticesMutex);

    e.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    e.glBufferData (GL_ARRAY_BUFFER,
                    sizeof (GLshort) * vertices.size(),
                    vertices.data(),
                    GL_STATIC_DRAW);
    e.glBindBuffer (GL_ARRAY_BUFFER, 0);
}
