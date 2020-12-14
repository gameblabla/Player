#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt default.funkey-s.desktop easyrpg.soundfont EasyRPG.opk -all-root -noappend -no-exports -no-xattrs
