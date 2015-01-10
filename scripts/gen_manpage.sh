#! /bin/bash
# Used to a man page from markdown

FILE="$4"
TEMP="$FILE.tmp"

echo ".\" Man page for the ttrts project" > $TEMP
echo ".\" this man page is auto-generated, do not edit directly" >> $TEMP

echo ".TH TTRTS\ v$1.$2.$3 6 $(date +%Y-%m-%d) http://mdiluz.github.io/ttrts/" >> $TEMP

# NOTE: For the OSX version of sed we use -E, which on linux appears be an undocumented switch for -r
# we also have to use [A-Za-z] instead of \w for some reason
# as well as escaped new line characters and literal tabs instead of \n and \t

# sections to section headers
# sub-sections in man page sub-sections
# 4-space lines to tabs
# tab starts removed
# Environment variables in bold
# User variables in italics
# remove all line-widths
# Put all back-ticks quotes in bold
# underline mapfile opt
# ensure name section uses correct
cat "$5" \
 | sed -E 's/^# ([A-Za-z]+)/.SH \1/g'					\
 | sed -E 's/^##+ ([A-Za-z]+)/.SS \1/g'  	    		\
 | sed -E 's/^    (.*)$/\
	\1\
/g'  			                                        \
 | sed -E 's/^	//g'  		            		    \
 | sed -E 's/\$\{([A-Za-z]+)\}/\\fB\$\1\\fR/g'		    \
 | sed -E 's/\{([A-Za-z]+)\}/\\fI\1\\fR/g'  			\
 | sed -E 's/-----+//g'  	            		        \
 | sed -E 's/`(.*)`/\\fB\1\\fR/g'  	                    \
 | sed -E 's/MAPFILE/\\fImapfile\\fR/g'  	            \
 | sed -E 's/HOSTNAME/\\fIhostname\\fR/g'  	            \
 | sed -E 's/	ttrts -/	ttrts \\-/g'   		>> $TEMP


if [ ! -e $FILE ] || [ ! -z "$( diff $FILE $TEMP )" ]; then
	mv -f $TEMP $FILE
fi
