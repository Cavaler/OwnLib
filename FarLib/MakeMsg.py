import sys

def split_by_tabs(line:str):
  return list([l for l in line.strip().split("\t") if len(l) > 0])

msgfile = sys.argv[1]
hdrfile = sys.argv[2]
lngfiles = sys.argv[3:]

msg = open(msgfile, encoding='utf-8-sig')
hdr = open(hdrfile, "w")
lng = list([open(f, "w", encoding='utf-8-sig') for f in lngfiles])

hdr.write("enum eStringTable {\n")

header = split_by_tabs(msg.readline())

if len(header)-1 != len(lng):
  print("Language file count does not match header")
  sys.exit(1)

for lang in range(0, len(lng)):
  lng[lang].write(header[0] + "=" + header[lang+1] + "\n\n")

for line in msg.readlines():
  values = split_by_tabs(line)
  if values:
    hdr.write("    " + values[0] + ",\n")
    for lang in range(0, len(lng)):
      item = values[lang+1] if lang+1<len(values) else values[1]
      lng[lang].write(item + "\n")
  else:
    hdr.write("\n")
    for lang in lng:
      lang.write("\n")

hdr.write("};\n")
