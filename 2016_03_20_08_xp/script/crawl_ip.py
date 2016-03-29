# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-03-03 21:08:55
# @Last Modified by:   obsidian
# @Last Modified time: 2016-03-04 10:04:11
import urllib2

import time
import socket

ip = []

# true_socket = socket.socket  
   
# ipbind='217.202.165.12'  
   
# def bound_socket(*a, **k):  
#     sock = true_socket(*a, **k)  
#     sock.bind((ipbind, 0))  
#     return sock  
f = open('url','r')
s = open('ip','w')
s.close()
k = eval(f.read(-1))
#socket.socket = bound_socket
r = dict(zip(k,[[] for i in range(len(k))]))


print len(r)
def get_ip(st):
    global ip
    content = urllib2.urlopen('http://www.cnrobocon.com/thread'+ st +'html').read()
    for i in  content.split('匿名 <em>')[1:]:
        if(len(i.split('</em></a>')[0]) < 20):
            oi = i.split('</em></a>')[0].split('.x')[0] + '.4'
            if len(ip) > 1:
                if oi != ip[len(ip) - 1]:
                    ip.append(oi)
                    r[st].append(oi)
            else :
                ip.append(oi)
                r[st].append(oi)                
i = 0
for st in k:
    i = i + 1
    try:
        get_ip(st)
        print 'over' + str(i)
        ip = list(set(ip))
        time.sleep(1)
        s = open('ip','a')
        s.write(str(ip[len(ip) - 1]) + '\n')
        s.close()
    except Exception:
        print 'except'
urls = open('urls','w')
urls.write(str(r))
urls.close()
s = open('ips','w')
s.write(str(ip))
s.close()
print 'over'
# 匿名 <em>221.193.217.x:4411</em></a>
