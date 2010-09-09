cmds = ["cmd_move",
	"cmd_smell",
	"cmd_pick",
	"cmd_aid",
	"cmd_yell"]
	
for i in cmds:
	print "typedef struct %s_req_t* %s_req_t" % (i, i)
	print "typedef struct %s_res_t* %s_req_t" % (i, i)
