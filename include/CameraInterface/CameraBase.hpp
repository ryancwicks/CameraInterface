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

#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include <string>
#include <memory>
#include <function>
#include <atomic>
#include <future>

#include "CameraInterface/CapturedImage.hpp"

namespace rw
{
namespace CameraInterface
{

/**
 * @brief Camera Interface virtual base class. All cameras derive from this one. 
 *        Templated for use with variable image types.
 */
template<typename PixelType>
class CameraBase
{
 public:
  
  CameraBase ();
  virtual ~CameraBase () {};

  /**
   * @brief Initialize the camera.
   * @param callback that is called when image is captured.
   * @param error_message returned by reference.
   */
  bool Initialize ( 
       std::function < void ( std::unique_ptr < PixelType > ) > callback );


  /**
   * @brief Set the camera exposure time in seconds.
   * @param exposure time in seconds
   * @param error_message returned by reference.
   * @return bool was the set successful
   */
  bool SetExposure ( double exposure, std::string & error_message );

  /**
   * @brief Set the camera rate time in seconds.
   * @param rate in Hz
   * @param error_message returned by reference.
   * @return bool was the set successful
   */
  bool SetRate ( double frame_rate, std::string & error_message );

  /**
   * @brief Grabs a single image. Blocking, runs in the current thread.
   * @param unique pointer to grabbed image.
   * @param error message returned by reference
   * @return bool was the image read successful
   */
  bool GrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message );

  /**
   * @brief Capture a continuous series of images in a loop in it's own thread. 
   *        Return by a callback. Runs in separate thread. non-blocking.
   */
  bool StartCapture ( std::string & error_message );

  /**
   * @brief Stop the capture loop. 
   */
  bool StopCapture ( std::string & error_message );

 private:

  // Do not want the camera to be copyable.
  CameraBase ( const CameraBase & ) = delete;
  CameraBase & operator= (const CameraBase & other) = delete;

  // See SetExposure
  bool handleSetExposure ( double exposure, std::string & error_message ) = 0;

  // See SetRate
  bool handleSetRate ( double frame_rate, std::string & error_message ) = 0;

  // See GrabImage
  bool handleGrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message ) = 0;

  // See Initialize
  bool handleInitialize ( std::string & error_message ) = 0;

  ///< callback function for returning images when run in a loop.
  std::function < void ( std::unique_ptr < PixelType > ) > m_callback; 

  ///< callback function when error is raised in capture loop.
  std::function < void ( std::string & ) > m_error_callback;

  ///< was the camera initialized
  bool m_is_initialized;

  ///< is the camera running in it's capture loop (thread safe atomic)
  std::atomic_bool m_is_running;

  ///< Future holding std::async job of camera capture.
  std::future <void> m_future;

  /**
   * @brief These are helper functions to check if the camera is running.
   * error messages.
   * @param error_message
   * @return bool is the camera running
   */
  bool IsNotRunning ( std::string & error_message );
  
/**
   * @brief These are helper functions to check if the camera is initialized.
   * error messages.
   * @param error_message
   * @return bool is the camera initialized.
   */
  bool IsInitialized ( std::string & error_message );

  /**
   * @brief Grabs images in a loop, runs with m_is_running is set.
   */
  void CaptureLoop () {
    while ( m_is_running ) {
      std::unique_ptr < CapturedImage <PixelType> > image;
      std::string error_message;
      if ( !handleGrabImage ( image, error_message ) ) {
        m_error_callback ( error_message );
        m_is_running = false;
        
      } else {
        m_callback ( image );
      }
    }
  }; 

};

}
}


#endif //CAMERA_INTERFACE_H
