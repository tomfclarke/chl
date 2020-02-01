/**
 *  chl_VertexGenerator.cpp
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

namespace chl
{
size_t getNumPointsInPath (const juce::Path& path)
{
    juce::Path::Iterator i (path);
    auto count = size_t (0);
    
    while (i.next())
    {
        switch (i.elementType)
        {
            case juce::Path::Iterator::startNewSubPath:
            case juce::Path::Iterator::lineTo:
                count += 2;
                break;
            case juce::Path::Iterator::quadraticTo:
                count += 4;
                break;
            case juce::Path::Iterator::cubicTo:
                count += 6;
                break;
            case juce::Path::Iterator::closePath:
                break;
        }
    }
    return count;
}

std::vector<GLshort> VertexGenerator::fromPath (const juce::Path& path)
{
    juce::Path::Iterator i (path);
    std::vector<GLshort> vertices;
    vertices.reserve (getNumPointsInPath (path));

    // build up a container with vertices for triangles that
    // start at 0,0 and join every two points in the path
    // TODO: handle curves
    while (i.next())
    {
        if (i.elementType == i.closePath && vertices.size() > 3)
        {
            vertices.push_back (vertices[2]);
            vertices.push_back (vertices[3]);
        }
        else if (i.elementType != i.startNewSubPath)
        {
            vertices.push_back (i.x1);
            vertices.push_back (i.y1);
        }
        if (i.elementType != i.closePath)
        {
            vertices.push_back (0);
            vertices.push_back (0);
            vertices.push_back (i.x1);
            vertices.push_back (i.y1);
        }
    }
    return vertices;
}
}
