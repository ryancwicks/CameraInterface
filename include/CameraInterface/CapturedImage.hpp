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

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <chrono>

namespace rw {
namespace camera {

/**
 * @brief Captured image base class.
 *        Templated for use with variable image types.
 */
template <typename PixelType> class CapturedImage {
public:
  CapturedImage(uint32_t width, uint32_t height)
      : m_width(width), m_height(height) {
    m_image.reserve(width * height * sizeof(PixelType));
  };

  virtual ~CapturedImage(){};

  /**
   * Set the internal image data from a variety of data structures.
   */

  /**
   * @brief copy from array into the internal vector.
   * @param array pointer of void * to the data
   * @param array size, number of values in the array. Must match the width x
   *        height x sizeof(PixelType) or an exception is thrown.
   */
  void setData(const void *array, std::size_t array_size);
  /**
   * @brief copy from array into the internal vector.
   * @param array pointer to the data
   * @param array size, number of values in the array. Must match width x
   *        height or an exception is thrown.
   */
  void setData(const PixelType *array, std::size_t array_size);
  /**
   * @brief set data contained in a std::string
   * @param buffer of string characters, size must equal width*height*
   *        sizeof(PixelType), or an exception is thrown.
   */
  void setData(const std::string &buffer);
  /**
   * @brief set data contained in a vector container
   * @param vector of pixel type. Must have size equal to length*width,
   *        or an exception is thrown.
   */
  void setData(const std::vector<PixelType> &buffer);

  /**
   * @brief Get a constant reference to the image data.
   * @return const vector reference to the image data.
   */
  const std::vector<PixelType> & getData() const;
  
  /**
   * @brief get the size of the internal datatype.
   * @return sizeof PixelType in bytes.
   */
  int sizeOfData() const;

  /**
   * @brief Get the dimensions of the image (width, height) as a std::pair.
   * @return pair (width, height)
   */
  std::pair<uint32_t, uint32_t> getDimension() const {
    return std::make_pair(m_width, m_height);
  };

  /**
   * @brief Get capture time.
   * @return std::chrono::high_resolution_clock::time_point of capture/data set.
   */
  std::chrono::high_resolution_clock::time_point getTime() const {
    return m_time;
  };

private:
  static const std::string c_range_error_message;
  CapturedImage() = delete;

  ///< storage for image
  std::vector<PixelType> m_image;

  ///< width and height for image
  uint32_t m_width;
  uint32_t m_height;

  ///< The capture time
  std::chrono::high_resolution_clock::time_point m_time;
  /**
   * @brief Set the capture time to when the data is set.
   */
  void setToCurrentTime() {
    m_time = std::chrono::high_resolution_clock::now();
  };
};

template <typename PixelType>
const std::string CapturedImage<PixelType>::c_range_error_message =
    "Mismatch between input data and internal image buffer size.";

template <typename PixelType>
void CapturedImage<PixelType>::setData(const void *array,
                                       std::size_t array_size) {
  if (array_size != m_width * m_height * sizeOfData()) {
    throw std::runtime_error(c_range_error_message);
  }
  this->setToCurrentTime();
  m_image.clear();
  const PixelType *array_pixel_type =
      reinterpret_cast<const PixelType *>(array);
  m_image = std::vector<PixelType>(array_pixel_type,
                                   array_pixel_type + m_width * m_height);
}

template <typename PixelType>
void CapturedImage<PixelType>::setData(const PixelType *array,
                                       std::size_t array_size) {
  if (array_size != m_width * m_height) {
    throw std::runtime_error(c_range_error_message);
  }
  this->setToCurrentTime();
  m_image.clear();
  m_image = std::vector<PixelType>(array, array + m_width * m_height);
}

template <typename PixelType>
void CapturedImage<PixelType>::setData(const std::string &buffer) {
  if (buffer.size() != m_width * m_height * sizeOfData()) {
    throw std::runtime_error(c_range_error_message);
  }
  this->setToCurrentTime();
  m_image.clear();
  const PixelType *array_pixel_type =
      reinterpret_cast<const PixelType *>(buffer.data());
  m_image = std::vector<PixelType>(array_pixel_type,
                                   array_pixel_type + m_width * m_height);
}

template <typename PixelType>
void CapturedImage<PixelType>::setData(const std::vector<PixelType> &buffer) {
  if (buffer.size() != m_width * m_height) {
    throw std::runtime_error(c_range_error_message);
  }
  this->setToCurrentTime();
  m_image.clear();
  std::copy(buffer.begin(), buffer.end(), m_image.begin());
}

template <typename PixelType>
int CapturedImage<PixelType>::sizeOfData() const {
  return sizeof(PixelType);
};

template <typename PixelType>
const std::vector<PixelType> & CapturedImage<PixelType>::getData() const {
  return m_image;
}

}
}

#endif // CPTURED_IMAGE_H
