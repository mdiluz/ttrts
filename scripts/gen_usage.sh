#! /bin/bash
# Used to generate usage text from markdown
cat README.md                        \
    | sed 's/^#\+ //g'                \
    | sed 's/^\t/\\t/g'              \
    | sed 's/^    /\\t/g'            \
    | sed ':a;N;$!ba;s/\n/\\n\n/g'   \
    | sed 's/^/\"/'                  \
    | sed 's/$/\"/'                  \
    > $1
