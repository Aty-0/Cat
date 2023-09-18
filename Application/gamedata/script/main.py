"""
Main file for "Cat" 
There you can run your scripts

Example:

core.run("Test") 

Or like this 
  if not core.run("Test"):
        raise Exception

Where function argument it's a only file name ! Without extension !


Otherwise, you can use this file as main file where you can script 
"""

from CAT_API import *

try:
    core = script_core.get_instance()
    
    if not core.run("game"):
        raise Exception

except Exception as ex:
    fatal(f"{ex}")


