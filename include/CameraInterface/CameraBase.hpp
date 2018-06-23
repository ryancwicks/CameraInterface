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

#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include <future>

#include "CameraInterface/CapturedImage.hpp"

namespace rw
{
namespace camera
{

/**
 * @brief Camera Interface virtual base class. All cameras derive from this one. 
 *        Templated for use with variable image types.
 */
template <typename PixelType>
class CameraBase
{
 public:
  
  CameraBase ();
  virtual ~CameraBase () {};

  /**
   * @brief Initialize the camera.
   * @param callback that is called when image is captured.
   * @param callback that is called when an error occurs.
   * @param error_message returned by reference.
   */
  bool initialize ( 
       std::function < void ( std::unique_ptr < PixelType > ) > callback,
       std::function < void ( std::string error_message ) >  error_callback, 
       std::string & error_message );

  /**
   * @brief Set the camera gain in percent.
   * @param gain in percent
   * @param error_message returned by reference.
   * @return bool was the set successful
   */
  bool setGain ( int exposure, std::string & error_message );


  /**
   * @brief Set the camera exposure time in seconds.
   * @param exposure time in seconds
   * @param error_message returned by reference.
   * @return bool was the set successful
   */
  bool setExposure ( double exposure, std::string & error_message );

  /**
   * @brief Set the camera rate time in seconds.
   * @param rate in Hz
   * @param error_message returned by reference.
   * @return bool was the set successful
   */
  bool setRate ( double frame_rate, std::string & error_message );

  /**
   * @brief Grabs a single image. Blocking, runs in the current thread.
   * @param unique pointer to grabbed image.
   * @param error message returned by reference
   * @return bool was the image read successful
   */
  bool grabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message );

  /**
   * @brief Capture a continuous series of images in a loop in it's own thread. 
   *        Return by a callback. Runs in separate thread. non-blocking.
   */
  bool startCapture ( std::string & error_message );

  /**
   * @brief Stop the capture loop. 
   */
  bool stopCapture ( std::string & error_message );

 private:

  // Do not want the camera to be copyable.
  CameraBase ( const CameraBase & ) = delete;
  CameraBase & operator= (const CameraBase & other) = delete;

  // See SetGain
  virtual bool handleSetGain ( int gain, std::string & error_message ) = 0;

  // See SetExposure
  virtual bool handleSetExposure ( double exposure, std::string & error_message ) = 0;

  // See SetRate
  virtual bool handleSetRate ( double frame_rate, std::string & error_message ) = 0;

  // See GrabImage
  virtual bool handleGrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message ) = 0;

  // See Initialize
  virtual bool handleInitialize ( std::string & error_message ) = 0;

  ///< callback function for returning images when run in a loop.
  std::function < void ( std::unique_ptr < PixelType > ) > m_callback; 

  ///< callback function when error is raised in capture loop.
  std::function < void ( std::string ) > m_error_callback;

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
  bool isNotRunning ( std::string & error_message );
  
/**
   * @brief These are helper functions to check if the camera is initialized.
   * error messages.
   * @param error_message
   * @return bool is the camera initialized.
   */
  bool isInitialized ( std::string & error_message );

  /**
   * @brief Grabs images in a loop, runs with m_is_running is set.
   */
  void captureLoop (); 

  const std::string uninitialized_camera_error_message; 
  const std::string camera_running_error_message;

 

};

const std::string uninitialized_camera_error_message = 
      "ERROR: The camera has not been properly initialized.";
const std::string camera_running_error_message = 
      "ERROR: The camera is currently running. Stop the camera first.";



template <typename PixelType>
CameraBase<PixelType>::CameraBase () :
    m_callback (),
    m_is_initialized ( false ),
    m_is_running ( false ) {};

template <typename PixelType>
bool CameraBase<PixelType>::initialize ( 
       std::function < void ( std::unique_ptr < PixelType > ) > callback,
       std::function < void ( std::string error_message ) >  error_callback, 
       std::string & error_message ) {
  if ( m_is_initialized ) return true;
  m_callback = callback;
  m_error_callback = error_callback;
  if ( !handleInitialize ( error_message ) ) return false;
  m_is_initialized = true;  
  return true;
}

template <typename PixelType>
bool CameraBase<PixelType>::grabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                 std::string & error_message ){
  if ( !isInitialized ( error_message ) ) return false;
  if ( !isNotRunning ( error_message ) ) return false;
  return handleGrabImage ( image, error_message );
}

template <typename PixelType>
bool CameraBase<PixelType>::setGain ( int gain, std::string & error_message ) {
  if ( !isInitialized ( error_message ) ) return false;
  if ( !isNotRunning ( error_message ) ) return false;
  return handleSetGain ( gain, error_message );
}


template <typename PixelType>
bool CameraBase<PixelType>::setExposure ( double exposure, std::string & error_message ) {
  if ( !isInitialized ( error_message ) ) return false;
  if ( !isNotRunning ( error_message ) ) return false;
  return handleSetExposure ( exposure, error_message );
}

template <typename PixelType>
bool CameraBase<PixelType>::setRate ( double frame_rate, std::string & error_message ) {
  if ( !isInitialized ( error_message ) ) return false;
  if ( !isNotRunning ( error_message ) ) return false;
  return handleSetRate ( frame_rate, error_message );
}

template <typename PixelType>
bool CameraBase<PixelType>::startCapture ( std::string & error_message ) {
  if ( !isInitialized ( error_message ) ) return false;
  if ( m_is_running ) return true; 
  
  m_is_running = true;
  m_future = std::async(std::launch::async, &CameraBase::CaptureLoop, this );
}

template <typename PixelType>
bool CameraBase<PixelType>::stopCapture ( std::string & error_message ) {
  if ( !isInitialized ( error_message ) ) return false;
  if ( m_is_running ) return true;

  m_is_running = false;
  m_future.get(); //wait for thread to exit (blocking);
}

template <typename PixelType>
bool CameraBase<PixelType>::isNotRunning ( std::string & error_message ) {
  if ( !m_is_running ) return true;
  
  error_message = camera_running_error_message;
  return false;
}

template <typename PixelType>
bool CameraBase<PixelType>::isInitialized ( std::string & error_message ) {
  if (m_is_initialized) return true;

  error_message = uninitialized_camera_error_message;
  return false;
}

template <typename PixelType>
void CameraBase<PixelType>::captureLoop () {
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


}
}


#endif //CAMERA_BASE_H
