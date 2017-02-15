#!/bin/bash
ver=v3.2.3  # FIXME
git archive --format=tar --prefix=ds3_c_sdk-${ver#v}/ ${ver}^{tree} |\
    gzip > ~/rpmbuild/SOURCES/ds3_c_sdk-${ver#v}.tar.gz
rpmbuild -bb ds3_c_sdk.spec
