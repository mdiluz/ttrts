#! /bin/bash
# Used to a man page from markdown



echo ".\" Man page for the ttrts project" > $4
echo ".\" this man page is auto-generated, do not edit directly" >> $4

echo ".TH TTRTS\ v$1.$2.$3 6 $(date +%Y-%m-%d) http://mdiluz.github.io/ttrts/" >> $4

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
 | sed -r 's/^# (\w+)/.SH \1/g'					\
 | sed -r 's/^##+ (\w+)/.SS \1/g'  	    		\
 | sed -r 's/^    (.*)$/\n\t\1\n/g'  			\
 | sed -r 's/^\t//g'  		            		\
 | sed -r 's/\$\{(\w+)\}/\\fB\$\1\\fR/g'		\
 | sed -r 's/\{(\w+)\}/\\fI\1\\fR/g'  			\
 | sed -r 's/-----+//g'  	            		\
 | sed -r 's/`(.*?)`/\\fB\1\\fR/g'  	        \
 | sed -r 's/MAPFILE/\\fImapfile\\fR/g'  	    \
 | sed -r 's/\tttrts -/\tttrts \\-/g'   		>> $4
