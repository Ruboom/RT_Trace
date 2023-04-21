from building import *

# The set of source files associated with this SConscript file.
cwd = GetCurrentDir()
src = []
CPPPATH = []
LIBS = []
LIBPATH = []

if GetDepend('PKG_USING_RT_TRACE'):
    src += ['src/rt_trace_set_hook.c']
    CPPPATH += [GetCurrentDir() + '/lib/include']
    LIBS             = ['rttrace_gcc'] 
    LIBPATH          = [GetCurrentDir() + '/lib'] 

group = DefineGroup('rtt_trace', src, depend = ['PKG_USING_RT_TRACE'], CPPPATH = CPPPATH, LIBS=LIBS, LIBPATH = LIBPATH)

Return('group')
