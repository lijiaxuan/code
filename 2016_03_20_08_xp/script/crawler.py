# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-03-03 20:28:09
# @Last Modified by:   obsidian
# @Last Modified time: 2016-03-03 21:03:19
import urllib2
import signal
import urllib
import json
import sys,os,re
import socket

import time

url = []
ipurl = "http://ip.taobao.com/service/getIpInfo.php?ip="
f = open('url','w')

def get_url(i):
    global url
    content = urllib2.urlopen('http://www.cnrobocon.com/forum-72-'+ str(i) +'.html').read()
    for i in content.split('<a href="thread')[1:]:
        try:
            if i.split('html')[0] != url[len(url) - 1]:
                url.append(i.split('html')[0])
        except Exception:
            url.append(i.split('html')[0])
def get():
    try:
        for i in range(30):
            get_url(i)
            time.sleep(0.5)
            print 'over' + str(i)
    except Exception:
        pass
    f.write(str(url))
    f.close()
get()
# if len(sys.argv) <= 1 :
#     print "Please input ip address !"
#     sys.exit(0)



#查找IP地址
def ip_location(ip):
    data = urllib.urlopen(ipurl + ip).read()
    datadict=json.loads(data)
 
    for oneinfo in datadict:
        if "code" == oneinfo:
            if datadict[oneinfo] == 0:
                return datadict["data"]["country"] + datadict["data"]["region"] + datadict["data"]["city"] + datadict["data"]["isp"]
 
#定义IP与域名正则
re_ipaddress = re.compile(r'^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$')
re_domain = re.compile(r'[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\.?')
def getip():
    ip_address = '221.193.217.4'
    if re_ipaddress.match(ip_address):  #如果参数是单个IP地址
        city_address = ip_location(ip_address)
        print ip_address + ":" + city_address
    elif(re_domain.match(ip_address)):  #如果参数是域名
        result = socket.getaddrinfo(ip_address, None)
        ip_address = result[0][4][0]
        city_address = ip_location(ip_address)
        print ip_address.strip() + ":" + city_address
