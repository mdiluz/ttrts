#! /bin/bash
# Used to generate usage text from markdown

FILE="$1"
TEMP="${FILE}_tmp"

cat README.md                        \
    | sed -E 's/^#+ //g'               \
    | sed -E 's/^	/\\t/g'              \
    | sed -E 's/^    /\\t/g'            \
    | sed -E 's/^/\"/'                  \
    | sed -E 's/$/\\n\"/'               \
    > $TEMP

# If no difference
if [ ! -e $FILE ] || [ ! -z "$( diff $TEMP $FILE )" ]; then
	mv -f $TEMP $FILE
fi
