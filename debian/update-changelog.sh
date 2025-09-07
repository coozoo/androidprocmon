#!/usr/bin/bash

# 1. Extract version from main.cpp
VERSION=$(curl --silent 'https://raw.githubusercontent.com/coozoo/androidprocmon/master/main.cpp' | grep 'QString APP_VERSION' | tr -d ' ' | grep -oP '(?<=constQStringAPP_VERSION=").*(?=\";)')

# 2. Get commit info
COMMIT_HASH=$(git rev-parse --short HEAD)
COMMIT_DATE=$(date -R)  # RFC 2822 format for Debian changelog
AUTHOR=$(git log -1 --format=%an)
EMAIL="yuriykuzin@yahoo.com"  # Set your maintainer email here
DISTRO=$(lsb_release -sc)
DATEVER=$(date +'%Y%m%d%H%M')

# 3. Format changelog entry (Debian-compliant)
CHANGELOG_ENTRY="androidprocmon (${VERSION}-$DATEVER) ${DISTRO}; urgency=low

  * Automated update: version from source, commit ${COMMIT_HASH}, author ${AUTHOR}

 -- ${AUTHOR} <${EMAIL}>  ${COMMIT_DATE}
"

# 4. Prepend to debian/changelog
if [ -f debian/changelog ]; then
    cp debian/changelog debian/changelog.bak
fi

echo "$CHANGELOG_ENTRY" > debian/changelog.tmp
cat debian/changelog >> debian/changelog.tmp 2>/dev/null
mv debian/changelog.tmp debian/changelog

