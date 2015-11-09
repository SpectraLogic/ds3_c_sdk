/*
* ******************************************************************************
*   Copyright 2014-2015 Spectra Logic Corporation. All Rights Reserved.
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

void         ds3_log_message(const ds3_log* log, ds3_log_lvl lvl, const char* message, ...);
size_t       ds3_load_buffer(void* buffer, size_t size, size_t nmemb, void* user_data);
ds3_error*   ds3_create_error(ds3_error_code code, const char * message);
