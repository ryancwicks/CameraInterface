#include "gtest/gtest.h"

#include "CameraInterface/CapturedImage.hpp"

TEST (CapturedImage, InitializerTests) {
  
  rw::camera::CapturedImage<uint8_t> buffer_8 (640, 480);
  rw::camera::CapturedImage<uint16_t> buffer_16 (2048, 1024);

  EXPECT_EQ ( buffer_8.sizeOfData(), sizeof (uint8_t) );
  EXPECT_EQ ( buffer_16.sizeOfData(), sizeof (uint16_t) );
  
  EXPECT_EQ ( buffer_8.getDimension().first, std::make_pair (640, 480).first );
  EXPECT_EQ ( buffer_16.getDimension().first, std::make_pair (2048, 1024).first );
  EXPECT_EQ ( buffer_8.getDimension().second, std::make_pair (640, 480).second );
  EXPECT_EQ ( buffer_16.getDimension().second, std::make_pair (2048, 1024).second );
}

TEST (CapturedImage, SetDataTests) {

  const uint8_t array_8[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  const uint16_t array_16[] = {11, 12, 13, 14, 15, 16, 17, 18, 19};

  std::size_t size_8 = sizeof(array_8) / sizeof(uint8_t);
  std::size_t size_16 = sizeof(array_16) / sizeof(uint16_t);

  std::string str_8 (reinterpret_cast <const char *> (array_8), sizeof (array_8) );
  std::string str_16 (reinterpret_cast <const char *> (array_16), sizeof (array_16) );
 
  std::vector<uint8_t> vec_8 (array_8, array_8 + sizeof(array_8)/sizeof(uint8_t));
  std::vector<uint16_t> vec_16 (array_16, array_16 + sizeof(array_16)/sizeof(uint16_t));
 
  rw::camera::CapturedImage <uint8_t> buffer_8 (3, 3);
  rw::camera::CapturedImage <uint16_t> buffer_16 (3, 3);

  try {
    buffer_8.setData (array_8, 0);
  } catch ( std::exception & e ) {
    EXPECT_EQ (e.what(), std::string ("Mismatch between input data and internal image buffer size."));
  }

  //Test different setters for 8 bit data
  buffer_8.setData( reinterpret_cast <const void *> (array_8), sizeof(array_8));
  for (unsigned int i = 0; i<sizeof(array_8); ++i) {
    EXPECT_EQ (buffer_8.getData()[i], array_8[i]);
  } 

  buffer_8.setData(array_8, size_8);
  for (unsigned int i = 0; i<size_8; ++i) {
    EXPECT_EQ (buffer_8.getData()[i], array_8[i]);
  } 
 
  buffer_8.setData(str_8);
  for (unsigned int i = 0; i<size_8; ++i) {
    EXPECT_EQ (buffer_8.getData()[i], array_8[i]);
  } 
  
  buffer_8.setData(vec_8);
  for (unsigned int i = 0; i<size_8; ++i) {
    EXPECT_EQ (buffer_8.getData()[i], array_8[i]);
  } 
 
  //Test different setters for 16 bit buffers
  buffer_16.setData( reinterpret_cast <const void *> (array_16), sizeof(array_16));
  for (unsigned int i = 0; i<size_16; ++i) {
    EXPECT_EQ (buffer_16.getData()[i], array_16[i]);
  } 

  buffer_16.setData(array_16, size_16);
  for (unsigned int i = 0; i<size_16; ++i) {
    EXPECT_EQ (buffer_16.getData()[i], array_16[i]);
  } 
 
  buffer_16.setData(str_16);
  for (unsigned int i = 0; i<size_16; ++i) {
    EXPECT_EQ (buffer_16.getData()[i], array_16[i]);
  } 
  
  buffer_16.setData(vec_16);
  for (unsigned int i = 0; i<size_16; ++i) {
    EXPECT_EQ (buffer_16.getData()[i], array_16[i]);
  } 

}
