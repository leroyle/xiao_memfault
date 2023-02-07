
import os
Import("env")
import fnmatch

print("Enter MAIN extra script")
#print("Dump env")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."


env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env, RDirs, TARGET, SOURCE, affect_signature=False)} $)" % (incdir)

