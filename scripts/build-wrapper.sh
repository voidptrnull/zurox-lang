#!/bin/sh

echo "Enter build mode(R for release,empty for debug):"
read BUILD_MODE
if [[ "$BUILD_MODE" == "R" ]];then
    sh scripts/build-rel.sh
else
    sh scripts/build-dbg.sh
fi