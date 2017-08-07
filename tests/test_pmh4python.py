import os

from pkg_resources import resource_string
from unittest import TestCase

import pmh4python

class TestPEGParser(TestCase):
	
	def test_parse_markdown(self):

		text = resource_string('assets', 'huge.md').decode('utf-8')

		data = pmh4python.parse_markdown(text)

		print('data', data.keys())