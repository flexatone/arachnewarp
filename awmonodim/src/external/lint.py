


import cpplint

import sys, os


DIR_SRC = '/Volumes/xdisc/_sync/_x/src/arachneWarp/arachnewarp/src'


def main():
    files = []
    for fn in os.listdir(DIR_SRC):
#         if not fn.endsiwth(''):
        fp = os.path.join(DIR_SRC, fn)
        files.append(fp)
    
    
    #print files
        print('\n\n%s' % fn.upper())
        cpplint.main([fp])


if __name__ == '__main__':
    main()