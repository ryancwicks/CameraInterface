#include "CameraInterface/CameraBase.hpp"

namespace rw
{
namespace CameraInterface
{

const std::string uninitialized_camera_error_message = 
      "ERROR: The camera has not been properly initialized.";
const std::string camera_running_error_message = 
      "ERROR: The camera is currently running. Stop the camera first.";


CameraBase::CameraBase () :
  m_callback (),
  m_is_initialized ( false ),
  m_is_running ( false ) {
}

bool CameraBase::Initialize ( 
       std::function < void ( std::unique_ptr < PixelType > ) > callback,
       std::function < void ( std::string ) error_callback ) {
  if ( m_is_initialized ) return true;
  m_callback = callback;
  m_error_callback = error_callback;
  if ( !handleInitialize ( error_message ) ) return false;
  m_is_initialized = true;  
  return true;
}

bool CameraBase::SetExposure ( double exposure, std::string & error_message ) {
  if ( !IsInitialized ( error_message ) ) return false;
  if ( !IsNotRunning ( error_message ) ) return false;
  return handleSetExposure ( exposure, error_message );
}

bool CameraBase::SetRate ( double frame_rate, std::string & error_message ) {
  if ( !IsInitialized ( error_message ) ) return false;
  if ( !IsNotRunning ( error_message ) ) return false;i
  return handleSetRate ( frame_rate, error_message );
}

bool CameraBase::GrabImage ( std::unique_ptr < CapturedImage < PixelType > > & image, 
                   std::string & error_message ) {
  if ( !IsInitialized ( error_message ) ) return false;
  if ( !IsNotRunning ( error_message ) ) return false;
  return handleGrabImage ( image, error_message );
}


bool CameraBase::StartCapture ( std::string & error_message ) {
  if ( !IsInitialized ( error_message ) ) return false;
  if ( m_is_running ) return true; 
  
  m_is_running = true;
  m_future = std::async(std::launch::async, &CameraBase::CaptureLoop, this );
}

bool CameraBase::StopCapture ( std::string & error_message ) {
  if ( !IsInitialized ( error_message ) ) return false;
  if ( m_is_running ) return true;

  m_is_running = false;
  
  m_future.get(); //wait for thread to exit (blocking);
}

bool CameraBase::IsNotRunning ( std::string & error_message ) {
  if ( !m_is_running ) return true;
  
  error_message = camera_running_error_message;
  return false;
}

bool CameraBase::IsInitialized ( std::string & error_message ) {
  if (m_is_initialized) return true;

  error_message = uninitialized_camera_error_message;
  return false;
}

} //CameraInterface
} //rw
