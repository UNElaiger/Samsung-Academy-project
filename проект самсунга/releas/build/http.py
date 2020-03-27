import requests
import sys
import urllib
print('Sending number:', sys.argv[1])
arg = sys.argv[1]
f=open('output.txt', 'r')
s = urllib.unquote(f.readline()).decode('utf-8')
f.close
a1 = "A333AA"
print("arg=",arg)
s=s.replace(' ','').replace('!','').replace('"','').replace('%','').replace('#','').replace('.','').replace('*','').replace('&','').replace('^','').replace('$','').replace('?','').replace('/','').replace('|','').replace(':','').replace(';','').replace('`','').replace('~','').replace('_','').replace('-','').replace('+','').replace('=','').replace('[','').replace(']','').replace('{','').replace('}','').replace('(','').replace(')','').replace(':','')
out=(s[0]+s[1]+s[2]+s[3]+s[4]+s[5])
r = requests.post("http://p533226.ihc.xyz/conect.php/", data={'number': out}, params='format=json')
print(r.status_code, r.reason)
print(r.text[:300] + '...')
