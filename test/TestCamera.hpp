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

#ifndef CAMERA_BASE_TEST_H
#define CAMERA_BASE_TEST_H

#include "CameraInterface/CameraBase.hpp"
#include "CameraInterface/CapturedImage.hpp"

namespace rw {
namespace camera {

/**
 * @brief Camera Interface test class. 
 */
template <typename PixelType>
class CameraTest : public CameraBase<PixelType> {
 public:

  CameraTest() : 
    CameraBase<PixelType>(),
    gain(0),
    rate(0.0),
    exposure(0.0) {};

  double gain;
  double rate;
  double exposure;
  
 private:

  // Do not want the camera to be copyable.
  CameraTest ( const CameraTest & ) = delete;
  CameraTest & operator= (const CameraTest & other) = delete;

  // See SetGain
  bool handleSetGain ( int gain, std::string & error_message ) override;

  // See SetExposure
  bool handleSetExposure ( double exposure, std::string & error_message ) override;

  // See SetRate
  bool handleSetRate ( double frame_rate, std::string & error_message ) override;

  // See GrabImage
  bool handleGrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message ) override;

  // See Initialize
  bool handleInitialize ( std::string & error_message ) override;


};

// See GrabImage
template <typename PixelType>
bool CameraTest<PixelType>::handleGrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                 std::string & error_message ) {
  //TODO Get test image into buffer. 
  return true;
}

template <typename PixelType>
bool CameraTest<PixelType>::handleSetGain ( int gain, std::string & error_message ) {
  gain = gain;
  return true;
}

template <typename PixelType>
bool CameraTest<PixelType>::handleSetExposure ( double exposure, std::string & error_message ) {
  exposure = exposure;
  return true;
}

template <typename PixelType>
bool CameraTest<PixelType>::handleSetRate ( double frame_rate, std::string & error_message ) {
  rate = frame_rate;
  return true;
}

template <typename PixelType>
bool CameraTest<PixelType>::handleInitialize ( std::string & error_message ) {
  return true;
}


}
}


#endif //CAMERA_TEST_H
