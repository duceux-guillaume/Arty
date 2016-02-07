
import re
import os

class Features:
	#Path features
	AbsPath = re.compile(r'(^/.*)+')
	RelPath = re.compile(r'(.+/.*)+')
	class Path:
		@classmethod
		def match(kls, line):
			return os.path.isdir(line)

	#File
	class File:
		@classmethod
		def match(kls, line):
			return os.path.isfile(line)

	#Opt
	Opt = re.compile(r'(^-.+)')