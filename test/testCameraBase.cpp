#include "gtest/gtest.h"

#include <memory>

#include "CameraInterface/CameraBase.hpp"
#include "TestCamera.hpp"

struct CallbackTracker {
  
  CallbackTracker () :
    capture_counter (0),
    error_counter (0)
  {};

  unsigned int capture_counter;
  unsigned int error_counter;

  void imageCaptureCallback (std::unique_ptr < rw::camera::CapturedImage<uint8_t> > buffer) {
    ++capture_counter;
    std::cout << "Image # " << capture_counter << " Captured. W: ";
    std::cout << buffer->getDimension().first << " H: " << buffer->getDimension().second << std::endl;
  };

  void errorCallback (const std::string & error_message) {
    std::cout << "Captured Error: " << std::endl;
    std::cout << error_message << std::endl;
    ++error_counter;
  }

};

class TestCamSetup : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::string error_message;
    auto callback_function = [&]( std::unique_ptr<rw::camera::CapturedImage<uint8_t>> buffer ){ callback_tracker.imageCaptureCallback(std::move(buffer)); };
    auto error_function = [&]( const std::string & error_message ){ callback_tracker.errorCallback(error_message); };
    test_cam.initialize( callback_function, error_function, error_message );
  };

  rw::camera::CameraTest<uint8_t> test_cam;
  CallbackTracker callback_tracker;
};
