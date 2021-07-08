#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt easyrpg.soundfont default.lepus.desktop EasyRPG_lepus.opk -all-root -noappend -no-exports -no-xattrs
