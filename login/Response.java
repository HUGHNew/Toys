package login;

import java.nio.charset.StandardCharsets;

public class Response extends ReProtocol{
    boolean status;
    String desc;
    static final String[] DESCRIPTION= {
            "The User Name exists!", //fall_user_name_repeated
            "The User Name doesn't exist!", //fall_user_name_no_exist
            "Incorrect password or UserName", //fall_no_match
            "Registration Successfully!", //succ_registration
            "Login successfully!" //succ_login
    };
    // for desc index
    static int
        fall_user_name_repeated=0,
        fall_user_name_no_exist=1,
        fall_no_match=2,
        succ_registration=3,
        succ_login=4;
    public static class ResMsg extends ReMsgBase{
        int statusCode;
        String desc;
        public ResMsg(Command c,boolean status,String description) {
            super(ResponseLen, c);
            this.statusCode=status?1:0;
            this.desc=description;
        }
        public ResMsg(Command c,boolean status,int index) {
            this(c,status,DESCRIPTION[index]);
        }
    }
    public Response(Command cmd,boolean successful,String description) {
        super(cmd);
        this.status=successful;
        this.desc=description;
    }
    public Response(Command cmd,boolean successful,int index) {
        this(cmd,successful,DESCRIPTION[index]);
    }
    public Response(ResMsg rmsg)throws RePException{
        this(IntToCommand(rmsg.id),rmsg.statusCode==1, rmsg.desc);
    }
    public ResMsg getMsg(){
        return new ResMsg(type,status, desc);
    }
    private byte[] descFill(){
        return fillBase(desc, 64);
    }
    @Override
    public byte[] getBytes() throws RePException{
        byte[] msg=new byte[this.totalLen];
        int offset=prefill(msg, this.type);
        msg[offset++]=(byte)(status?1:0);
        System.arraycopy(descFill(), 0, msg, offset, DescriptionLen);
        return msg;
    }
    public static ResMsg getMsg(byte[] data) throws RePException {
        if(data.length!=ReProtocol.ResponseLen){
            throw new RePException("Wrong-format Response");
        }else{
            byte[]desc=new byte[DescriptionLen];
            System.arraycopy(data, HeaderLen+StatusCodeLen, desc, 0, DescriptionLen);
            return new ResMsg(IntToCommand(getValueFromMsg(data, MsgLen)),data[8]==1,decodeFill(desc));
        }
    }
}
