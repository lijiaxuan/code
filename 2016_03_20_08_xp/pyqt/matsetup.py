#Used successfully in PythonXY2.7.3 with py2exe ,wxPython 2.8.12 and matplotlib 1.2.0
from distutils.core import setup
import  matplotlib
import py2exe

#----------------------------------------------------------------------------
includes=[
"sip",
"matplotlib",
"matplotlib.figure",
"matplotlib.pyplot",
"matplotlib.backends.backend_tkagg",
"matplotlib.animation",
"numpy",
]
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
dll_excludes=[]
#----------------------------------------------------------------------------
options= {
		'py2exe':{
				"includes" : includes,
				'dll_excludes':dll_excludes,
                "bundle_files": 1,
				}
}
#----------------------------------------------------------------------------
data_files=matplotlib.get_py2exe_datafiles()
#----------------------------------------------------------------------------

#for console program
console = [
{"script": "plot3d.py"}
]
#----------------------------------------------------------------------------
setup(
console=console,
options=options,
data_files=data_files,
zipfile=None
)
