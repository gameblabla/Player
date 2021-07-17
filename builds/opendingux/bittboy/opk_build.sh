#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt default.bittboy.desktop EasyRPG_bittboy.opk -all-root -noappend -no-exports -no-xattrs
