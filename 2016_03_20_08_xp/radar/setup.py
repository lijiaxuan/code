from distutils.core import setup
import py2exe

setup(windows=[{"script" : "radar.py"}], options={"py2exe" : {"includes" : ["sip"],"bundle_files": 1}},zipfile=None)