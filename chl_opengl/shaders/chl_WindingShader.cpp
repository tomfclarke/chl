/**
 *  chl_WindingShader.cpp
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
WindingShader::WindingShader (juce::OpenGLContext& context)
    : program (context)
    , builder (program)
    , params (program)
{
}

const WindingShader& WindingShader::select (juce::OpenGLContext& context)
{
    return ::chl::select<WindingShader>(context, "chlWindingShader");
}

WindingShader::Builder::Builder (juce::OpenGLShaderProgram& program)
{
    program.addVertexShader (juce::OpenGLHelpers::translateVertexShaderToV3 (
        "attribute " JUCE_HIGHP " vec2 position;"
        "uniform " JUCE_HIGHP " vec2 targetSize;"
        "void main()"
        "{"
        JUCE_HIGHP " vec2 scaled = position / (0.5 * targetSize.xy);"
        "gl_Position = vec4 (scaled.x - 1.0, scaled.y - 1.0, 0, 1.0);"
        "}"));
    
    program.addFragmentShader (juce::OpenGLHelpers::translateFragmentShaderToV3 (
        "void main()"
        "{"
        "gl_FragColor = vec4 (1.0) * (gl_FrontFacing ? 16.0 / 255.0 : 1.0 / 255.0);"
        "}"));
    
    program.link();
}

WindingShader::Params::Params (juce::OpenGLShaderProgram& program)
    : position (program, "position")
    , targetSize (program, "targetSize")
{
}
}
