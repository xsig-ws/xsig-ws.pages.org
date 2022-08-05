invoke_host = "ermine"

def r(s):
    return rcmd(s)

# read config
r("reset")
r("r concheck.gxp")

# explore and cd
r("explore")
r("cd ~/sproj/gxp/apps/net/")
r("smask")

# select relevant hosts
r("e hostname | grep %s" % invoke_host)
r("smask -")

r("e {{ python accept.py }} cat {{ python connect.py }} python mkg.py > G")
r("l neato -Tps -o G.ps G")
