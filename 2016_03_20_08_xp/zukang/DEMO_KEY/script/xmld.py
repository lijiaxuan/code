#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-21 20:05:09
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-21 20:37:46

from xml.dom.minidom import parse
import xml.dom.minidom

# 使用minidom解析器打开 XML 文档
DOMTree = xml.dom.minidom.parse("/home/obsidian/virtualbox/xp/pdata.xml")
collection = DOMTree.documentElement
devices = collection.getElementsByTagName("DEVICE_1")
items = devices[0].getElementsByTagName("item")
for item in items:
	item.setAttribute('D_VALUE','0')
for item in items:
	print item.getAttribute('D_VALUE')
f = open("/home/obsidian/virtualbox/xp/pdata.xml",'w')
f.write(DOMTree.toxml())
f.close()
