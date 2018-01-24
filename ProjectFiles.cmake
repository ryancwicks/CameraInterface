# Copyright (c) 2018 Ryan Wicks (ryancwicks@gmail.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

# Set project source files.
set(SRC
  "${SRC_PATH}/CameraBase.cpp"
  "${SRC_PATH}/CapturedImage.cpp"
)

set (HEADER
  "${INCLUDE_PATH}/CameraInterface/CameraBase.hpp"
  "${INCLUDE_PATH}/CameraInterface/CapturedImage.hpp"
)

# Set project main file.
set(MAIN_SRC
  #"${SRC_PATH}/main.cpp"
)

# Set project test source files.
set(TEST_SRC
  "${TEST_SRC_PATH}/testCameraBase.cpp"
  "${TEST_SRC_PATH}/testCapturedImage.cpp"
  "${TEST_SRC_PATH}/TestCamera.cpp"

)

