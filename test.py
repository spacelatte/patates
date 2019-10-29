#!/usr/bin/env python3

import io, re, os, sys, time, subprocess

from urllib import request

PORT = 9000

class Tests(object):
	
	httpreq = lambda self, path, method="GET", headers={}, data=None: (
		request.urlopen(
			request.Request(
				"http://0:{}{}".format(self.port, path or "/"),
				data=data and data.encode(),
				headers=headers,
				method=method,
			)
		).read().decode()
	)

	def __init__(self, proc, port):
		self.tests = filter(lambda name: name.startswith("test_"), dir(self))
		self.port  = port
		self.proc  = proc
		return

	def runAll(self):
		return dict([
			(test, getattr(self, test)())
			for i, test in enumerate(self.tests)
		])

	def test_http_get(self):
		return self.httpreq("/data")

def main(args):
	with subprocess.Popen("./exe") as proc:
		print(Tests(proc, PORT).runAll())
		pass
	return False

if __name__ == '__main__':
	sys.exit(main(sys.argv))
