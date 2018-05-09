#!/usr/bin/env python3

import fileinput
import re
import os

class WrongStuffException(Exception):
    def __init__(self, file, none):
        self.file = file
    def __str__(self):
        return repr(self.file)
        
class MessedUpFileNameException(Exception):
	def __init__(self, file, none):
        self.file = file
    def __str__(self):
        return repr(self.file)

class Formatter:
	opt = {'FT': False,'LW': 0,'LM': 0,'LS': 0}
	state = {'line_length':0}
	
	def __init__(self, filename , none):
		self.filename = filename
		self.none = none
		self.out = ""
		self.start()

	def start(self):
		try:
			if self.filename:
				if os.path.isfile(self.filename):
					txt = open(self.filename, "r")
		except IOError:
			print("Sorry, couldn't find " + self.filename)
		else:
						name = self.filename.split(".")[0]
						extension = self.filename.split(".")[1]
						if not re.findall(r'[^A-Za-z0-9_\-\\]',name):
							raise MessedUpFileNameException("How'd you even manage to name this file?")
						if extension != 'txt':
							raise WrongStuffException("Wait, what are we using then?")
					self.transform(txt)
			elif self.none:
				self.transform(self.none)

	def extract(self, l):
		strtok = l.split()
		if len(strtok) is not 0:
			if strtok[0] == ".FT":
				if strtok[1] == "off":
					Formatter.opt['FT'] = False
				elif strtok[1] == "on":
					Formatter.opt['FT'] = True
				if strtok != "off" or strtok != "on":
					raise WrongStuffException(".FT has something other than on/off as its parameter Line: " + l)
			elif strtok[0] == ".LW":
				if isinstance(int(strtok[1]), int):
					Formatter.opt['FT'] = True
					Formatter.opt['LW'] = int(strtok[1])
				elif strtok[1][0] == "-":
					Formatter.opt['FT'] = True
				try:
					Formatter.opt['LW'] -= int(strtok[1][1:])
				except WrongStuffException:
					raise WrongStuffException("Needs to be a number")
				elif strtok[1][0] == "+":
					Formatter.opt['FT'] = True
				try:
					Formatter.opt['LW'] += int(strtok[1][1:])
				except WrongStuffException:
					raise WrongStuffException("Needs to be a number")
			elif strtok[0] == ".LM":
				if strtok[1][:1] == "-":
				try:
					Formatter.opt['LM'] -= int(strtok[1][1:])
				except WrongStuffException:
					raise WrongStuffException("Needs to be a number")
					if Formatter.opt['LM'] < 0:
						Formatter.opt['LM'] = 0
				elif strtok[1][:1] == "+":
				try:
					Formatter.opt['LM'] += int(strtok[1][1:])
				except WrongStuffException:
					raise WrongStuffException("Needs to be a number")
				else:
					Formatter.opt['LM'] = int(strtok[1])
				if Formatter.opt['LM'] > Formatter.opt['LW'] - 20:
					Formatter.opt['LM'] = Formatter.opt['LW'] - 20
			elif strtok[0] == ".LS":
				try:
					Formatter.opt['LS'] = int(strtok[1])
				except ValueError:
					raise ValueError("Needs to be a legit number")
				except WrongStuffException:
					raise WrongStuffException("Needs to be a number")

	def transform(self, txt):
		space = "".join(["\n" for i in range(Formatter.opt['LS'])])
		word = ""
		for l in txt:
			strtok = l.split()
			if self.extract(l):
				continue
			if l.startswith(".FT") or l.startswith(".LM") or l.startswith(".LW") or l.startswith(".LS"):
				continue
			if Formatter.opt['FT'] == False:
				self.out  += l
				continue
			elif l.startswith("\n") and l.endswith("\n"):
				Formatter.state['line_length'] = 0
				if Formatter.opt['LS'] == 0:
					self.out  = self.out + '\n\n'
				elif Formatter.opt['LS'] == 1:
					self.out  = self.out + '\n\n\n' + "".join(["\n" for i in range(Formatter.opt['LS'])])
				else:
					self.out  = self.out + '\n\n\n\n' + "".join(["\n" for i in range(Formatter.opt['LS'])])
				continue
			l.rstrip()
			for word_num, next in enumerate(strtok):
				if word_num != 0:
					self.add_new_word(word)
				word = next
			self.add_new_word(word)

	def add_new_word(self, word):
		space = "".join(["\n" for i in range(Formatter.opt['LS'])])
		margin = "".join([" " for i in range(Formatter.opt['LM'])])
		if self.state['line_length'] == 0:
			self.out = self.out + margin
			Formatter.state['line_length'] = Formatter.opt['LM']
		elif Formatter.state['line_length'] + len(word) >= Formatter.opt['LW']:
			self.out = self.out + '\n' + space + margin
			Formatter.state['line_length'] = Formatter.opt['LM']
		elif Formatter.state['line_length'] < Formatter.opt['LW']:
			Formatter.state['line_length'] += 1
			self.out += " "
		self.out  += word
		Formatter.state['line_length'] += len(word)

	def get_lines(self):
		send_lines = iter(self.out.splitlines())
		return send_lines

if __name__ == "__main__":
	formatter = Formatter()
	lines = Formatter.get_lines()
	print (''.join(lines))