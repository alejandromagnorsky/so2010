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
    
    BASE_PROTO = "cmd_t new%C(%args);"
       
    BASE_CREATOR = """
cmd_t new%C(%args) {
    cmd_%c_t ret = (cmd_%c_t) malloc(sizeof(struct cmd_%c_t));
    ret->type = CMD_%U;
    %assigns
    
    return (cmd_t) ret;
}
"""

    BASE_SA_ENTRY = "%a[CMD_%U] = sizeof(struct cmd_%c_t);"
    
    def struct(self):
        return self.BASE_STRUCT.replace("%c", self.name).replace(
                                        "%args", "\n".join(
                                         [i + ";" for i in self.args]))
        
    def typedef(self):
        return self.BASE_TYPEDEF.replace("%c", self.name)

    def creatorproto(self):
        c = self.name
        C = self.name.replace("_", " ").title().replace(" ", "")
        args = ", ".join(self.args)
        
        return self.BASE_PROTO.replace("%c", c).replace(
                                       "%C", C).replace(
                                       "%args", args)
                                       
    def creator(self):
        c = self.name
        C = self.name.replace("_", " ").title().replace(" ", "")
        U = self.name.upper()
        args = ", ".join(self.args)
        
        assigns = "\n".join(["ret->%s = %s;" % (i[-1], i[-1])
                              for i in map(str.split, self.args)])
        
        return self.BASE_CREATOR.replace("%c", c).replace(
                                         "%C", C).replace(
                                         "%U", U).replace(
                                         "%args", args).replace(
                                         "%assigns", assigns)
    
    def saentry(self, a):
        c = self.name
        U = self.name.upper()
        return self.BASE_SA_ENTRY.replace("%c", c).replace(
                                          "%a", a).replace(
                                          "%U", U)

    def __init__(self, name, *args):
        self.name = name
        self.args = args
        
if __name__ == "__main__":
     
    start    = Command("start")
    turn     = Command("turn")
    movereq  = Command("move_req", "int dir")
    moveres  = Command("move_res", "int status")
    smellreq = Command("smell_req")
    smellres = Command("smell_res", "tile_t* tiles")
    pickreq  = Command("pick_req", "int dir")
    pickres  = Command("pick_res", "int status")
    aidreq   = Command("aid_req", "int dir")
    aidres   = Command("aid_res", "int status")
    yellreq  = Command("yell_req")
    yellres  = Command("yell_res")
    yellnot  = Command("yell_not")
    stop     = Command("stop")
    
    commands = [ c for c in globals().values() if isinstance(c, Command) ]
            
    print "/* TYPE DEFINITIONS: */"        
            
    for i in commands:
        print i.typedef()
            
    print "/* STRUCTURE DEFINITIONS: */"            
            
    for i in commands:
        print i.struct()
        
    print "/* CREATION FUNCTIONS: */"    

    for i in commands:
        print i.creatorproto()
        
    for i in commands:
        print i.creator()

    print "/* SIZE ARRAY ENTRIES: */"

    for i in commands:
        print i.saentry("CMD_SIZE")
