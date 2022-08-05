import os

clusters = "(pad|blade|mp20|xcmp|dennis|alice|comp|suidobashi|sheep|istbs)"


## problems is an array of [problem#, file-x, file-y].
#problems = [ [11, 16, 16] ]  
#problems = [ [12, 32, 32] ] 
#problems = [ [13, 128, 128] ]
#problems = [ [14, 128, 64] ] 
#problems = [ [15, 128, 64] ] 
#problems = [ [16, 16, 16] ] 
#problems = [ [17, 32, 32] ] 
#problems = [ [18, 128, 128] ]
#problems = [ [19, 128, 64] ]
#problems = [ [20, 128, 64] ]

#problems = [ [10, 32, 32] ]
#problems = [ [9, 64, 64] ]

problems = [ [21, 100, 100] ]
#problems = [ [22, 120, 120] ]
#problems = [ [23, 100, 100] ]
#problems = [ [24, 100, 100] ]
#problems = [ [25, 120, 120] ]
#problems = [ [26, 100, 100] ]

GXP="/usr/local/src/gxp"                       # must be valid on local host
TOOLPATH="~/yosen-tools-inst" # must be valid on all hosts
PYTHON="~tau/local/Linux/bin/python" # must be valid on all hosts (?)

worker_py = "%s/apps/mw2/worker2.py" % GXP
master_py = "%s/apps/mw2/master2.py" % GXP
#data_dir = "~/yd/xet"                 # must be valid on all hosts
data_dir = "~/kd/avaj"                 # must be valid on all hosts
#data_dir = "~/grchdata/data"                 # must be valid on all hosts
tasks_dir = "~/grchtasks"               # must be valid on local host

for ptuple in problems:
    [p, nx, ny] = ptuple
    tasks = "%s/%02d" % (tasks_dir, p)
    if not os.path.exists(os.path.expanduser(tasks)):
        sys.stderr.write("%s does not exist\n" % tasks)
        continue
    prob_dir = "%s/%02d" % (data_dir, p)
    rcmd("rmask")
    rcmd("export F=0")
    rcmd("e hostname | egrep -e '%s' > /dev/null" % clusters)
    rcmd("smask")
    rcmd("export F=1")
    rcmd("e mkdir    %s/hogehoge%02d 2> /dev/null" % (data_dir, p))
    rcmd("smask")
    rcmd("export F=2")
    rcmd("e rmdir %s/hogehoge%02d" % (data_dir, p))
    rcmd("e rm -rf %s" % prob_dir)
    #rcmd("e mkdir -p %s/data" % data_dir)
    rcmd("e mkdir -p %s" % prob_dir)
    rcmd("e cat %s {{ cat > %s/__w.py }}" % (worker_py, prob_dir))
    rcmd("rmask")
    rcmd("e [ $F -ge 1 ]")
    rcmd("smask")
    rcmd("export PATH=%s:$PATH" % TOOLPATH)
    rcmd("export GRCHDATA=%s" % data_dir)
    rcmd("l rm status")
    rcmd("e ls %s/__w.py > /dev/null || hostname" % prob_dir)
    rcmd("mw %s %s %s {{ %s %s/__w.py }}" \
         % (PYTHON, master_py, tasks, PYTHON, prob_dir))
    rcmd("rmask")
    rcmd("e [ $F = 2 ]")
    rcmd("smask")

    #rcmd("e {{ cat %s/*.ans }} sort -u > %s/%02d.ans" % (prob_dir, tasks_dir, p))

    localans = "%s/%02d.ans" % (tasks_dir, p)
    rcmd("l rm %s" % localans);
    rcmd("l echo %d %d %d 0 >> %s" % (p, nx, ny, localans))
    for y in range(ny):
        rcmd("e {{ cat %s/%02d???%03d.ans }} sort -u >> %s" % (prob_dir, p, y, localans))

    #rcmd("e {{ cat %s/*.ans }} sort -u > %s/%02d.bdy" % (prob_dir, tasks_dir, p))
    #rcmd("l cat %s/%02d.hdr %s/%02d.bdy > %s/%02d.ans" % (tasks_dir, p, tasks_dir, p, tasks_dir, p))
    #rcmd("l rm -f %s/%02d.bdy" % (tasks_dir, p))

    rcmd("l mkdir -p %s" % prob_dir)
    rcmd("l cp %s %s/%02d.ans" % (localans, prob_dir, p))
    rcmd("e cat %s {{ cat > %s/%02d.ans}}" % (localans, prob_dir, p))
