#!/bin/bash

set -e

#this clears anything out that we might have put in before
printf "\033c"
cmake -DDEBUG=1 ..

printf "\033c"
cd ..
make

printf "\033c"
./target_aid TestMedia/test3.png

printf "\033c"
./target_aid TestMedia/aim-test3.png


set +e
