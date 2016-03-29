# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-03-03 22:05:52
# @Last Modified by:   obsidian
# @Last Modified time: 2016-03-04 11:10:12
import urllib2
f = open('ips','r')
ip = eval(f.read(-1))
urls = open('urls','r')

for i in ip:
    content = urllib2.urlopen('http://geoiplookup.net/ip/'+i).read()
    for i in content.split('class="ipdata">')[1:]:
        print i.split('</div>')[0].split('<')[0].split('(')[0]
