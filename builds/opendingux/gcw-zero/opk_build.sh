#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.soundfont easyrpg.png run.sh easyrpg.soundfont readme.txt default.gcw0.desktop EasyRPG.opk -all-root -noappend -no-exports -no-xattrs
