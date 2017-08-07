from distutils.core import setup, Extension
from setuptools import find_packages

peg_markdown_module = Extension(
	'pmh4python',
	sources = ['pmh4python/pmh4python.c', 'pmh4python/pmh_parser.c']
)

setup(name = 'pmh4python',
	version = '0.1',
	description = 'Python wrapper for peg-markdown-highlight by Ali Rantakari',
	keywords='peg markdown highlight',
	ext_modules = [peg_markdown_module],
	test_suite='nose.collector',
    tests_require=['nose'],

	url='https://github.com/emohedano',
	author='Eduardo Mohedano',
	packages=find_packages(),
	package_data={'pmh4python': ['assets/*.md']},
	author_email='emohedanomonter@gmail.com')