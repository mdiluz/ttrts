#! /bin/bash
# Used to generate usage text from markdown

cat README.md                        \
    | sed -E 's/^#+ //g'               \
    | sed -E 's/^	/\\t/g'              \
    | sed -E 's/^    /\\t/g'            \
    | sed -E 's/^/\"/'                  \
    | sed -E 's/$/\\n\"/'               \
    > $1
