import sys,string

def read_clusters(file):
	n_nodes = 0
	clusters = []
	f = open(file)
	for line in f.readlines():
		if (line[0] == '#'):
			continue
		l = string.split(line)
		n = string.atoi(l[1])
		clusters.append((l[0], n, l[2]))
		n_nodes += n
	f.close()
	return (clusters, n_nodes)

def make_one_task(prob_num, x, y, config_str, cl_no, clusters):
	cluster = clusters[cl_no]
	return "%s make-pic1 %s/%02d/%02d%03d%03d.mdt %s" %\
		(cluster[0], cluster[2], prob_num, prob_num,
		x, y, config_str)
#	return "%s ./make-pic1 %s/%02d/%02d%03d%03d.mdt %s" %\
#		(cluster[0], cluster[2], prob_num, prob_num,
#		x, y, config_str)

#plan: return [(cluster_id, x, y), ...]
def make_flat_plan(clusters, n_nodes, worldx, worldy):
	plan = []
	n_files = worldx * worldy
	cl_id = 0
	done_nodes = 0
	done_files = 0
	end_file = (done_nodes + clusters[cl_id][1]) * n_files / n_nodes
	for x in range(0, worldx):
		for y in range(0, worldy):
			if (done_files >= end_file):
				done_nodes += clusters[cl_id][1]
				cl_id += 1
				end_file = (done_nodes + clusters[cl_id][1]) *\
						n_files / n_nodes
			plan.append((cl_id, x, y))
			done_files += 1
	return plan

def make_tasks(prob_no, worldx, worldy, filex, filey, matagi, clusters, plan):
	tasks = []
	fstr = "%d %d %d %d %d" % (worldx, worldy, filex, filey, matagi)
	for p in plan:
		tasks.append(make_one_task(prob_no, p[1], p[2], fstr, p[0],
				clusters))
	return tasks

def print_tasks(tasks):
	lineno = 0
	for t in tasks:
		print "%d %s" % (lineno, t)
		lineno += 1

def make_plan(mode, clusters, n_nodes, worldx, worldy):
	if mode == 0:
		return make_flat_plan(clusters, n_nodes, worldx, worldy)
	elif mode == 1:
		eq_clusters = []
		eq_n_nodes = 0
		for t in clusters:
			eq_clusters.append((t[0], 10, t[2]))
			eq_n_nodes += 10
		return make_flat_plan(eq_clusters, eq_n_nodes, worldx, worldy)
	else:
		return []

def main():
	prob_no = string.atoi(sys.argv[3])
	worldx = string.atoi(sys.argv[4])
	worldy = string.atoi(sys.argv[5])
	filex = string.atoi(sys.argv[6])
	filey = string.atoi(sys.argv[7])
	matagi = string.atoi(sys.argv[8])
	clusters, n_nodes = read_clusters(sys.argv[2])
	plan = make_plan(string.atoi(sys.argv[1]), clusters, n_nodes, worldx, worldy)
	tasks = make_tasks(prob_no, worldx, worldy, filex, filey, matagi,
				clusters, plan)
	print_tasks(tasks)
	return

main()
