#!/bin/sh
rm -rf EasyRPG.opk
mksquashfs EasyRPG easyrpg.png run.sh readme.txt default.rg99.desktop EasyRPG.opk -all-root -noappend -no-exports -no-xattrs
