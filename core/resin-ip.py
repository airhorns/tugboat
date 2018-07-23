import json
import urllib2
import os

data = json.load(urllib2.urlopen("{}/v1/device?apikey={}".format(os.environ['RESIN_SUPERVISOR_ADDRESS'], os.environ['RESIN_SUPERVISOR_API_KEY'])))

print(data['ip_address'].split(' ')[0])
