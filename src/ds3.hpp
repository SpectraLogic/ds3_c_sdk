/*
 * ******************************************************************************
 *   Copyright 2014 Spectra Logic Corporation. All Rights Reserved.
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

#ifndef __DS3_HPP__
#define __DS3_HPP__

#include <string>

using namespace std;

namespace ds3 {

    class Credentials {
        public:
            Credentials(const string& access_key, const string& secret_key)
                : m_access_key(access_key), m_secret_key(secret_key) {/* Nothing */}
            string accessKey() const { return m_access_key; }
            string secretKey() const { return m_secret_key; }
        private:
            const string m_access_key;
            const string m_secret_key;
    };

    class Ds3Client {
        public:
            Ds3Client(const Credentials& creds) : m_creds(creds) {/* Nothing */}
            Credentials credentials() const { return m_creds; }
            //~Ds3Client();
        private:
            const Credentials m_creds;
    };
}

#endif
