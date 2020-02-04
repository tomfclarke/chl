/**
 *  chl_FillShader.cpp
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

namespace chl
{
FillShader::FillShader (juce::OpenGLContext& context)
    : program (context)
    , builder (program)
    , params (program)
{
}

const FillShader& FillShader::select (juce::OpenGLContext& context)
{
    return ::chl::select<FillShader>(context, "chlFillShader");
}

FillShader::Builder::Builder (juce::OpenGLShaderProgram& program)
{
    program.addVertexShader (juce::OpenGLHelpers::translateVertexShaderToV3 (
        "attribute " JUCE_HIGHP " vec2 position;"
        "uniform " JUCE_HIGHP " vec2 targetSize;"
        "varying " JUCE_HIGHP " vec2 texturePos;"
        "void main()"
        "{"
        JUCE_HIGHP " vec2 scaled = position / (0.5 * targetSize.xy);"
        "gl_Position = vec4 (scaled.x - 1.0, 1.0 - scaled.y, 0, 1.0);"
        "texturePos = position / targetSize;"
        "}"));
    
    program.addFragmentShader (juce::OpenGLHelpers::translateFragmentShaderToV3 (
        "uniform sampler2D windingTexture;"
        "varying " JUCE_HIGHP " vec2 texturePos;"
        "void main()"
        "{"
        JUCE_HIGHP " float winding = texture2D (windingTexture, texturePos).x;"
        JUCE_HIGHP " float scaled = winding * 255.0;"
        JUCE_HIGHP " float lower = mod (scaled, 16.0);"
        JUCE_HIGHP " float upper = (scaled - lower) / 16.0;"
        JUCE_HIGHP " float alpha = min (abs (upper - lower), 1.0);"
        "gl_FragColor = vec4 (1.0, 1.0, 1.0, alpha);"
        "}"));
    
    program.link();
}

FillShader::Params::Params (juce::OpenGLShaderProgram& program)
    : position (program, "position")
    , targetSize (program, "targetSize")
    , windingTexture (program, "windingTexture")
{
}
}
