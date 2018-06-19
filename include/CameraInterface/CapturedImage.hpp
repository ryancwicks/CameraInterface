/**
 MIT License

Copyright (c) 2018 Ryan Wicks 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
 */

#ifndef CAPTURED_IMAGE_H
#define CAPTURED_IMAGE_H

#include <string>
#include <vector>
#include <cstdint>

namespace rw
{
namespace camera
{

/**
 * @brief Captured image base class. 
 *        Templated for use with variable image types.
 */
template <typename PixelType>
class CapturedImage
{
 public:
  
  CapturedImage ( uint32_t width, uint32_t height ) : 
      m_width (width),
      m_height (height) 
      { m_image.reserve (width * height * sizeof (PixelType) ); };
  
  virtual ~CapturedImage () {};

 private: 

  CapturedImage () = delete;
  
  ///< storage for image
  std::vector < uint8_t > m_image;

  ///< width and height for image
  uint32_t m_width;
  uint32_t m_height;
};

}
}


#endif //CPTURED_IMAGE_H
