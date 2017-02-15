#!/bin/bash
set -e
#ver=v3.2.3  # FIXME
ver=`git describe --tags --abbrev=4 |tr - _`
sver=${ver#v}
# git archive --format=tar --prefix=ds3_c_sdk-${sver}/ ${ver}^{tree} |\
git archive --format=tar --prefix=ds3_c_sdk-${sver}/ HEAD |\
    gzip > ~/rpmbuild/SOURCES/ds3_c_sdk-${sver}.tar.gz
# sed -e 's/$VERSION/'$sver'/' < ds3_c_sdk.spec.in > ds3_c_sdk.spec
rpmbuild -D "sdk_version ${sver}" -bb ds3_c_sdk.spec
