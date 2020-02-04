/**
 *  chl_FillShader.h
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
    Shader that takes a texture containing pixel values that are multiples of
    1/255 and fills based on whether those values are odd.

    If a pixel's colour value when scaled back up by 255 is odd, then the pixel
    lies inside the outline of the polygon and can be filled during a second
    pass with the fill shader.

    @see WindingShader
 */
struct FillShader : public juce::ReferenceCountedObject
{
    FillShader (juce::OpenGLContext& context);
    
    static const FillShader& select (juce::OpenGLContext& context);
    
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
            windingTexture.set (0);
        }
        
        juce::OpenGLShaderProgram::Attribute position;
        juce::OpenGLShaderProgram::Uniform targetSize, windingTexture;
    };
    
    juce::OpenGLShaderProgram program;
    Builder builder;
    Params params;
};
}
