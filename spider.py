'''# -*- coding:utf-8 -*-'''

import urllib2
import re

#p = re.compile('((\w+)\s+\w+)')
#p = re.compile('[http]{4}\\:\\/\\/[a-z]*(\\.[a-zA-Z]*)*(\\/([a-zA-Z]|[0-9])*)*\\s?')
#p = 'http://(([-a-zA-Z0-9]*)|([-./!@]*))*'
p = re.compile('http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]'
               '|[!*\(\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+')

#p = re.compile('http[s]?://(([a-zA-Z0-9]*)|([$-_@&+]*)|([!*\(\),]*))*')#这个正则表达不行
text = urllib2.urlopen("http://lilydjwg.vim-cn.com/"
                       "articles/smart-questions.html#forum").read()
print re.findall(p,text)#打印列表。
for url in re.findall(p,text):
    print url




