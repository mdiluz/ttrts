#! /usr/bin/python
# python ttrts module

# Import the python regex module
import re 

headerRegex = re.compile(
	"==== ttrts v(\\d+)\\.(\\d+)\\.(\\d+)+ ====\nNAME:(.+)\nSIZE:\\[(\\d+),(\\d+)\\]\nTURN:(\\d+)\n(WALL:.*?)"
	,re.MULTILINE )