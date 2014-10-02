#include <string>

using namespace std;

namespace ds3 {

    class Credentials {
        public:
            Credentials(const string& access_key, const string& secret_key)
                : _access_key(access_key), _secret_key(secret_key) {/* Nothing */}
            string accessKey() const { return _access_key; }
            string secretKey() const { return _secret_key; }
        private:
            const string _access_key;
            const string _secret_key;
    };

    class Ds3Client {
        public:
            Ds3Client(const Credentials& creds) : _creds(creds) {/* Nothing */}
            Credentials credentials() const { return _creds; }
            //~Ds3Client();
        private:
            const Credentials _creds;
    };
}
