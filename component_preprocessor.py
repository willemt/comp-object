#!/usr/bin/python


"""

Pre-process the component C definition source

"""

import os, os.path, sys
import re

target = sys.argv[2]
event_prefix = "__COMPEV_"
headers = ["component.h", "component_messenger.h"]
area = ""
funcs = []
events = []

# Base compname off filename
compname = sys.argv[1]
compname = os.path.basename(compname).replace("comp_","").replace(".c","")

out = sys.stdout

# Headers
for h in headers:
    out.write('#include %s\n' % h)

ignoring = False

for line in open(sys.argv[1]):
    if line.startswith("#if 0"):
        ignoring = True
        continue
    elif line.startswith("#endif") and ignoring:
        ignoring = False
        continue
    elif ignoring:
        continue
    elif line.startswith("#"):
        new_area = re.sub(r'#([a-zA-Z_]*).*',r'\1',line)
        new_area = new_area.strip()
        if new_area in ("define", "include", "if", "endif"):
            pass
        elif new_area == "end":
            area = ""
            continue
        else:
            area = new_area
            continue

    if area == target or area == "":
        out.write(line)
        if re.match(r'.*static void %s[A-Z0-9_]*.*' % event_prefix,line):
            event = re.sub(r'.*static void %s([A-Z0-9_]*).*'% event_prefix, r'\1',line)
            event = event.strip()
            events.append(event)


# Create the component type
out.write('\n')
out.write('void __init_%s(void* ctx) {\n' % (compname))
out.write('\tvoid* msg;\n\n')
out.write('\tcomponent_ctx_type_new(ctx, "%s", %s);\n' % (compname, 'COMPONENT_TYPE'))
out.write('\tmsg = component_messenger_new(ctx);\n')

# Write out all events
for event in events:
    eventfunc = event_prefix+event
    out.write('\tcomponent_messenger_append(msg,%s,"%s",%s);\n' %\
            ("COMPONENT_TYPE", event, eventfunc))

out.write("}\n")

