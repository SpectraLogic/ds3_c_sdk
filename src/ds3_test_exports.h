/*
 * ******************************************************************************
 *   Copyright 2014-2018 Spectra Logic Corporation. All Rights Reserved.
 *   Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 *   this file except in compliance with the License. A copy of the License is located at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   or in the "license" file accompanying this file.
 *   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *   CONDITIONS OF ANY KIND, either express or implied. See the License for the
 *   specific language governing permissions and limitations under the License.
 * ****************************************************************************
 */

#ifndef DS3_TEST_EXPORTS_H
#define DS3_TEST_EXPORTS_H

// For windows unit test symbol exports.
#ifdef _WIN32
#    ifdef TEST_EXPORTS
#        define TEST_API __declspec(testexport)
#    else
#        define TEST_API __declspec(testimport)
#    endif
#else
#    define TEST_API
#endif

#endif
