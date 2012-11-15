#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import re
import xml.parsers.expat
from xml.etree import ElementTree

def removeComments(string):
    string = re.sub(re.compile("/\*.*?\*/",re.DOTALL ) ,"" ,string)
    string = re.sub(re.compile("//.*?\n" ) ,"" ,string)
    return string

def stripWhitespaces(string):
	# get which quote is used primarily: the one that appears first
	quote = "'" if len(string.split("'")[0]) < len(string.split('"')[0]) else '"'

	sections = string.split(quote)
	j = 0;
	string = ""
	for section in sections:
		if j % 2 == 0:
			string = string + section.replace(" ", "")
		else:
			string = string + quote + section + quote
		j = j + 1

	return string;

files = []
classes = {}
data_points = []
for filename in os.listdir('./structure'):
	if ".css" in filename and ".css~" not in filename and "cssclasses" not in filename and "datapoints" not in filename:
		files.append(filename)

for filename in files:
	f = open('structure/' + filename,'r')
	data = f.read()
	data = removeComments(data)

	blocks = []
	holder = data.split("{")
	for part in holder:
		sections = part.split("}")
		for section in sections:
			blocks.append(section)

	i = 0
	name_holder = []
	points_holder = []
	for block in blocks:
		if i % 2 == 0:
			name_holder = []
			names = block.split('.')
			for name in names[1:]:
				name = name.strip()
				name = name.strip(',')
				name_holder.append(name)
				if name not in classes:
					classes[name] = set()
		else:
			points_holder = []
			points = block.split(';')
			holder = []
			for point in points:
				point = point.strip()
				if len(point) > 0:
					point = point + ";"
					point = stripWhitespaces(point)
					if point not in data_points:
						data_points.append(point)
						points_holder.append(len(data_points) - 1)
					else:
						points_holder.append(data_points.index(point))	
					holder.append(point)
			for name in name_holder:
				classes[name]|=set(points_holder)


		i = i + 1
files = []
for filename in os.listdir('./structure'):
	if ".xml" in filename and ".xml~" not in filename and "xmlelements" not in filename:
		files.append(filename)

datas = []
for filename in files:
	f = open('structure/' + filename,'r')
	data = f.read()
	tree = ElementTree.parse('structure/' + filename)
	m = set(re.findall('\s*<.*?ref="(.*?)".*?>', data, re.MULTILINE))
	for match in m:
		elem = tree.findall(".//*[@id='%s']" % (match))[0]
		parents = set(tree.findall(".//*[@ref='%s']/.." % (match)))
		for parent in parents:
			refs = parent.findall(".//*[@ref='%s']" % (match))
			children = parent.findall("*")
			for ref in refs:
				index = 0
				position = 0
				for element in children:
					if ref != element:
						index = position
					position = position + 1
				parent.insert(index, elem)
				parent.remove(ref)

			parent.set('updated', 'yes')

	datas.append(ElementTree.tostring(tree.findall(".")[0], encoding="utf-8", method="xml"))


elements = []
relations = []
ids = {}
previous = [-1]
parent = [-1]
next = -1
child = -1
count = [0]
depth = [0]
# 3 handler functions
def start_element(name, attrs):
	count[0] = count[0] + 1
	depth[0] = depth[0] + 1

	name = str(name)
	points = []

	tag = 'tag="%s";' % (name)
	if tag not in data_points:
		data_points.append(tag)
	points.append("[%d]"% (data_points.index(tag)) );

	for atr_name, atr_value in attrs.iteritems():
		atr_name = str(atr_name)
		atr_value = str(atr_value)

		point = atr_name + "=" + '"' + atr_value + '"'
		if atr_value in classes:
			point = point + "[%d]" % (classes.keys().index(atr_value))

		point = point + ';'
		if point not in data_points:
			data_points.append(point)
		points.append("[%d]"% (data_points.index(point)) );

	points = "[%d]:%s," % ( len(points), "".join(points) )
	relation = []
	relation.append(previous[len(previous) - 1])
	relation.append(parent[len(parent) - 1])
	relation.append(next)
	relation.append(child)

	points = points + " - %d - " % (count[0] - 1)

	elements.append(points)
	relations.append(relation)

	if -1 != parent[len(parent) - 1] and -1 == relations[parent[len(parent) - 1]][3]:
		relations[parent[len(parent) - 1]][3] = elements.index(points)

	if -1 != previous[len(previous) - 1] and -1 == relations[previous[len(previous) - 1]][2]:
		relations[previous[len(previous) - 1]][2] = elements.index(points)

	previous.append(-1)
	parent.append(elements.index(points))

	#print 'Start element:' + name + str(attrs)
def end_element(name):
	depth[0] = depth[0] - 1

	del previous[len(previous) - 1]
	previous.append(parent[len(parent) - 1])

	del parent[len(parent) - 1]
	w = 2
	#print 'End element:' + (name)
def char_data(data):
	w = 2
	#print 'Character data:' + repr(data)



for data in datas:
	p = xml.parsers.expat.ParserCreate()
	p.StartElementHandler = start_element
	p.EndElementHandler = end_element
	p.CharacterDataHandler = char_data
	p.Parse(data, 1)

c = open('structure/' + 'cssclasses.txt','w')
c.write('[%d]\n' % len(classes))
for name, points in classes.iteritems():	
	c.write('- %s: [%d]:' % (name, len(points)) )
	for item in points:
		c.write('[%d]' % (item) )
	c.write(';\n')

d = open('structure/' + 'datapoints.txt','w')
d.write('[%d]\n' % len(data_points))
for point in data_points:	
	d.write('- %s' % (point) )
	d.write('\n')

e = open('structure/' + 'xmlelements.txt','w')
e.write('[%d]\n' % len(elements))
for elem in elements:	
	rel = ""
	for relation in relations[elements.index(elem)]:
		rel = rel + "[%d]" % (relation)	
	e.write('- %s,%s' % (elem.split(',')[0], rel) )
	e.write(';\n')
