#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt easyrpg.soundfont default.rg99.desktop EasyRPG_rg99.opk -all-root -noappend -no-exports -no-xattrs
