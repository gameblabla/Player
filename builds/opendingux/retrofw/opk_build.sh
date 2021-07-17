#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt default.retrofw.desktop EasyRPG_retrofw.opk -all-root -noappend -no-exports -no-xattrs
