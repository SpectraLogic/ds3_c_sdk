#!/bin/bash
set -e
#ver=v3.2.3  # FIXME
# ver=`git describe --tags --abbrev=4 |tr - _`
eval `git describe --tags --abbrev=4 | ( IFS=- read ver rev id ;  echo "ver=$ver rev=$rev id=$id" )`
sver=${ver#v}
# git archive --format=tar --prefix=ds3_c_sdk-${sver}/ ${ver}^{tree} |\
git archive --format=tar --prefix=ds3_c_sdk-${sver}-$rev-$id/ HEAD |\
    gzip > ~/rpmbuild/SOURCES/ds3_c_sdk-${sver}.tar.gz
# sed -e 's/$VERSION/'$sver'/' < ds3_c_sdk.spec.in > ds3_c_sdk.spec
rpmbuild -D "sdk_version ${sver}" -D "sdk_rev ${rev}" -D "sdk_id ${id}" -bb ds3_c_sdk.spec
