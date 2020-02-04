/**
 *  chl_WindingShader.h
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
/**
    Shader that writes colour values of 1/255 which, when used with additive
    blending, accumulates to calculate the winding number of pixels inside a
    polygon.
 
    If a pixel's colour value scaled back up by 255 is odd, then the pixel lies
    inside the outline of the polygon and can be filled during a second pass
    with the fill shader.
 
    @see FillShader
*/
struct WindingShader : public juce::ReferenceCountedObject
{
    WindingShader (juce::OpenGLContext& context);
    
    static const WindingShader& select (juce::OpenGLContext& context);
    
    struct Builder
    {
        Builder (juce::OpenGLShaderProgram& program);
    };
    
    struct Params
    {
        Params (juce::OpenGLShaderProgram& program);
        
        void set (const float targetWidth, const float targetHeight) const
        {
            targetSize.set (targetWidth, targetHeight);
        }
        
        juce::OpenGLShaderProgram::Attribute position;
        juce::OpenGLShaderProgram::Uniform targetSize;
    };
    
    juce::OpenGLShaderProgram program;
    Builder builder;
    Params params;
};
}
