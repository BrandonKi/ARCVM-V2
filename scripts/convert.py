# finds the instruction enum in ARCVM.h
# then converts it to a format that can be pasted into a spreadsheet

content = open("../include/ARCVM.h").read()
start = content.find('enum instruction')

result = " ";
count = 0
while result[-1:] != '}':
    result += content[start + count]
    count += 1

print('\n'.join([el.strip() for el in result[result.find('{')+1:-1].split(',') if len(el.strip()) > 0]))