#!/bin/bash
[ -z "$1" ] && exit 2

colors=(ff7 fd6 fa5 f84 f62 f50 f40 f30 f20 f10)

for x in {0..9}; do
    d=$((9 - x))
    sed 's/<path/& transform="translate(0,'"$x"')"/g' "$1" |
        sed 's/ style="stop-color:#9.*/style="stop-color:#'"${colors[$x]}"';stop-opacity:1;"/g' |
            sed 's/ offset="0\..*/offset="0.'"$d"'"/g' > "images/thered_${x}.svg" ||
                exit 1
done

