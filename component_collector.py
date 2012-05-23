#!/usr/bin/python

import os, os.path, sys
import glob

path = sys.argv[1]

out = sys.stdout

out.write('void __init_builtins_%s() {\n' % os.path.basename(path))
out.write('\tvoid* ctx;\n')
out.write('\tctx = component_ctx_new();\n')
for f in glob.glob('%s/comp_*.c' % path):
    out.write('\t__init_%s(ctx);\n' % os.path.basename(path))
out.write('}\n')

