# Python script to generate some boilerplate code.

#struct cmd_move_res_t {
#/* MOVE RESPONSE: Control tells Ant whether its move was accepted. */
#    int type;
#    int status;
#};


class Command(object):

    BASE_STRUCT = """
struct cmd_%c_t {
    int type;
    %args
};
"""

    BASE_TYPEDEF = "typedef struct cmd_%c_t* cmd_%c_t;"
    
    BASE_CREAT0R = """
cmd_t new%C(%args) {
    cmd_%c_t ret = (cmd_%c_t) malloc(sizeof(struct cmd_%c_t));
    %assigns
}
"""

    def struct(self):
        return self.BASE_STRUCT.replace("%c", self.name).replace(
                                        "%args", ";\n".join(self.args))
        
    def typedef(self):
        return self.BASE_TYPEDEF.replace("%c", self.name)

    def creator(self):
        ret = 

    def __init__(self, name, *args):
        self.name = name
        self.args = args
        
if __name__ == "__main__":
     
    start    = Command("start")
    turn     = Command("turn")
    movereq  = Command("move_req", "int dir")
    moveres  = Command("move_res", "int status")
    smellreq = Command("smell_req")
    smellres = Command("smell_res", "tile_t tiles[3][3]")
    pickreq  = Command("pick_req", "int dir")
    pickres  = Command("pick_res", "int status")
    aidreq   = Command("aid_req", "int dir")
    aidres   = Command("aid_res", "int status")
    yellreq  = Command("yell_req")
    yellres  = Command("yell_res")
    yellnot  = Command("yell_not")
    stop     = Command("stop")
    
    commands = [ c for c in globals().values() if isinstance(c, Command) ]
            
    for i in commands:
        print i.typedef()
            
    for i in commands:
        print i.struct()


