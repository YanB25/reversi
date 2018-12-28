d = {}

def dfs(ls):
    if len(ls) == 8:
        xl = 0
        yl = 0
        xr = 0
        yr = 0
        if ls[0] != ' ':
            tmp = 0
            for i in ls:
                if i != ls[0]:
                    break
                tmp += 1
            if ls[0] == 'X':
                xl += tmp
            else:
                yl += tmp
        if ls[-1] != ' ':
            tmp = 0
            for i in ls[::-1]:
                if i != ls[-1]:
                    break
                tmp += 1
            if ls[-1] == 'X':
                xr += tmp
            else:
                yr += tmp
        tag = 0
        for i in ls:
            tag *= 3
            if i == 'X':
                tag += 1
            if i == 'O':
                tag += 2
        d[tag] = (xl+xr)  + (yl+yr) * 10000
    else:
        for i in [' ', 'X', 'O']:
            dfs(ls + i)
dfs('')
# print(d
out = '''
#include <iostream>
using namespace std;
int main() {
    printf("hello world\\n");
}
double VALUE_MAP[] = {
    '''
for key in sorted(d.keys()):
    # print("{}:{}".format(key, d[key]))
    out += str(d[key]) + ','
out = out[:-1]
out += '};'
print(out)