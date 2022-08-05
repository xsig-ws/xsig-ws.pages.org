import os,re,sys,string,time

host_info = {
    "Matsuken"     : [ ("pad(\d{3})",   "pad%03d",
                        range(0,21) + range(31,116), "") ],
    "Alab"         : [ ("blade(\d{2})", "blade%02d",
                        range(1,31), ".alab.ip.titech.ac.jp") ],
    "Onoken"       : [ ("mp20_(\d{3})", "mp20_%03d",
                        range(1,53), "") ],
    "Sansoken"     : [ ("xcmp(\d{3})",  "xcmp%03d",
                        range(2,44), ".asc.hpcc.jp") ],
    "HPCSken"      : [ ("dennis(\d{2})",  "dennis%02d",
                        range(1,16), ".omni.hpcc.jp"),
                       ("alice(\d{2})",  "alice%02d",
                        range(1,6), ".omni.hpcc.jp") ],
    "Yubahondaken" : [ ("comp(\d{2})",  "comp%02d",
                        range(11,17) + range(21, 27) + range(31, 37) \
                        + range(41, 47), ""),
                       ("comp(\d{3})",  "comp%03d",
                        range(110, 126), "") ],
    "Hirakken"     : [ ("suidobashi(\d{2})",  "suidobashi%02d",
                        range(1, 42), ".logos.ic.i.u-tokyo.ac.jp") ],
    "Chikayamaken" : [ ("sheep(\d{2})", "sheep%02d",
                        range(1,64), ".cl.logos.k.u-tokyo.ac.jp") ]
    "Tauken"       : [ ("istbs(\d{3})", "istbs%03d",
                        range(1,110), "i.u-tokyo.ac.jp") ]
    }

def Es(s):
    sys.stderr.write(s)

def compile_info(info):
    cinfo = {}
    for key,vals in info.items():
        comp_vals = []
        for pat,template,idx,dn in vals:
            comp_vals.append((re.compile(pat),template,idx,dn))
        cinfo[key] = comp_vals
    return cinfo

def reduce_hosts(hosts):
    reduced = []
    cur_cluster = None
    cur_template = None
    cur_idxs = None
    info = compile_info(host_info)
    counts = {}
    for host in hosts:
        found = 0
        for cluster,vals in info.items():
            for pat,template,_,dn in vals:
                m = pat.match(host)
                if m is not None:
                    counts[cluster] = counts.get(cluster, 0) + 1
                    found = 1
                    break
            if found: break
        if found == 0:
            Es("warning : bad hostname %s (skipped)\n" % host)
    for host in hosts:
        # look for the cluster host belongs to
        for cluster,vals in info.items():
            for pat,template,_,dn in vals:
                m = pat.match(host)
                if m is not None:
                    idx = string.atoi(m.group(1))
                    found = 1
                    break
            if found: break
        else:
            continue
        if cluster == cur_cluster and cur_idxs[-1] + 1 == idx:
            cur_idxs.append(idx)
        else:
            if cur_cluster is not None:
                if len(cur_idxs) > 1:
                    row = ['<td class="host_l">%s%s</td>' % \
                           (cur_template % cur_idxs[0], cur_dn),
                           '<td class="host_m">--</td>',
                           '<td class="host_r">%s%s</td>' \
                           % (cur_template % cur_idxs[-1], cur_dn)]
                else:
                    row = ['<td colspan=3">%s%s</td>' \
                           % (cur_template % cur_idxs[0], cur_dn) ]
                reduced.append(row)
            if cluster != cur_cluster:
                row = [ '<th align="left" colspan=3>%s cluster (%d nodes)</th>' \
                          % (cluster, counts[cluster]) ]
                reduced.append(row)
            cur_cluster = cluster
            cur_template = template
            cur_dn = dn
            cur_idxs = [ idx ]

    if cur_cluster is not None:
        if len(cur_idxs) > 1:
            row = ['<td class="host_l">%s%s</td>' % \
                   (cur_template % cur_idxs[0], cur_dn),
                   '<td class="host_m">--</td>',
                   '<td class="host_r">%s%s</td>' \
                   % (cur_template % cur_idxs[-1], cur_dn)]
        else:
            row = ['<td colspan=3">%s%s</td>' \
                   % (cur_template % cur_idxs[0], cur_dn) ]
        reduced.append(row)
    return reduced
            

def mk_all_hosts():
    a = {}
    for pat,temp,idxs,_ in host_info.values():
        for i in idxs:
            a[temp % i] = 1
    all = a.keys()
    all.sort()
    return all

def mk_live_hosts(file):
    fp = open(file)
    hosts = []
    for line in fp.readlines():
        hosts.append(string.strip(line))
    fp.close()
    hosts.sort()
    return hosts

def calc_dead_hosts(all, lives):
    # all - lives
    dead = {}
    for host in all:
        dead[host] = 1
    # remove all live hosts from all
    for host in lives:
        for pat,template,idxs,_ in compile_info(host_info).values():
            m = pat.match(host)
            if m is not None:
                live = m.group(0)
                if dead.has_key(live):
                    del dead[live]
                break
    deadlist = dead.keys()
    deadlist.sort()
    return deadlist

# 
# 
# 

def write_header(wp):
    wp.write('<h2 class="top">ホスト情報(更新: %s)</h2>\n' \
             % (time.ctime()))
def write_trailer(wp):
    pass
    
def write_txt(entries, filename):
    wp = open(filename, "wb")
    for host in entries:
        wp.write("%s\n" % host)
    wp.close()
    
def write_gxp(entries, filename):
    wp = open(filename, "wb")
    for host in entries:
        wp.write("des %s\n" % host)
    wp.close()
    
def write_heading(title, wp):
    wp.write('<h3>%s</h3>\n' % title)
    
def write_link(file, text, wp):
    wp.write('<p><a href=%s>%s</a></p>\n' % (file, text))
    
def write_table(entries, wp, show_numbers=0):
    rentries = reduce_hosts(entries)
    wp.write("<table border=1>\n")
    for entry in rentries:
        wp.write("<tr>\n")
        for e in entry:
            wp.write("%s\n" % e)
        wp.write("</tr>\n")
        
    wp.write("</table>\n")

def mk_files(infile, outdir, livefile_txt, deadfile_txt, deadfile_gxp, html):
    lives = mk_live_hosts(infile)
    all = mk_all_hosts()
    dead = calc_dead_hosts(all, lives)
    write_txt(lives, os.path.join(outdir, livefile_txt + ".new"))
    write_txt(dead, os.path.join(outdir, deadfile_txt + ".new"))
    write_gxp(dead, os.path.join(outdir, deadfile_gxp + ".new"))
    wp = open(os.path.join(outdir, html), "wb")
    write_heading('更新日: %s\n' % time.ctime(), wp)

    write_heading('全計算ホスト\n', wp)
    wp.write(r'''<div>
<ul>
<li>期間中計算ノードとして使ってよいマシンすべてです</li>
<li>委員へのメモ: 予選期間開始までは「利用可能ノード」が宣言した台数を満たすように
相談の上可能ならば調整
予選が始まったら決して更新されない(期間中は台数は単調減少)</li>
<li>以下には故障などで一時的(もしかするとずっと)
利用不可のノードも含まれます(下記、抜け番情報参照)</li>
<li>入口、コンパイル用ホストは含まれていません</li>
</ul>
''')
    write_table(all, wp)
    wp.write('</div>\n')

    write_heading('抜け番(使用不可)ホスト情報\n', wp)
    wp.write(r'''<div>
<ul>
<li>上記「全計算ホスト」に含まれるもののうち、現在利用不可(sshログイン失敗する)のノードです</li>
<li>情報が最新である保証はありません(1日1度更新予定です)が、必要ならこの情報を使って
故障ノードを避けて下さい
<li>作成/利用するソフトウェアが自動的に故障を判断するのであれば、
この情報を使う必要はありません</li>
</ul>
''')
    write_link(deadfile_txt, 'txtファイル(故障ノード名をtxtファイルで列挙)\n', wp)
    write_link(deadfile_gxp, 'gxpファイル(故障ノード名をgxpのコマンドファイルとして列挙)\n', wp)
    write_table(dead, wp)
    wp.write('</div>\n')

    write_heading('利用可能ホスト\n', wp)
    wp.write(r'''<div>
<ul>
<li>上記「全計算ホスト」に含まれるもののうち、現在利用可(sshログイン成功する)のノードです</li>
<li>情報が最新である保証はありません(1日1度更新予定です)が、必要ならこの情報を使って
利用するホストを決めて下さい
<li>作成/利用するソフトウェアが自動的にノードを割り当てる機能を持つのであれば、
この情報を使う必要はありません</li>
</ul>
''')
    
    write_link(livefile_txt, 'txtファイル(利用可能ノード名をtxtファイルで列挙)\n', wp)
    write_table(lives, wp, show_numbers=1)
    wp.write('</div>\n')
    
    wp.close()
    
# ------------------

def r(s):
    return rcmd(s)

def setup():
    r("r ~tau/grch/conf.gxp")
    r("show plan")
    r("explore")
    r("desf")
    r("explore")
    r("desf")
    r("explore")

def update_livehosts(old_file, new_file):
    r("r ~tau/grch/sel_nodes.gxp")
    r("e {{ hostname }} sort > %s" % new_file)
    x,a,b,c = r("l diff %s %s" % (old_file, new_file))

def main():
    old_file = "livehosts"
    new_file = "livehosts.new"
    if 1:
        setup()
        update_livehosts(old_file, new_file)
    else:
        os.system("cp %s %s" % (old_file, new_file))
    mk_files(new_file, "..",
             "live.txt", "dead.txt", "dead.gxp",
             "orig/info-hosts.html.orig")
    os.rename("livehosts.new", "livehosts")
    os.rename("../live.txt.new",  "../live.txt")
    os.rename("../dead.txt.new",  "../dead.txt")
    os.rename("../dead.gxp.new",  "../dead.gxp")
    os.system("cd ..; make")

main()
